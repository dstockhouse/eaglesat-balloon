/*****************************************************************************
 *
 * File:
 * 	crp.c
 *
 * Description:
 * 	Functions related to interfacing to the CRP system.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function crp_sensorRead and
 *		crp_imageStore	
 *
 * Revision 1.1
 * 	Date 10/3/18
 * 	Author: David Stockhouse
 * 	Changes: Modified file to be composed of wrapper functions for python
 * 		scripts rather than C functions. Instructions for embedding
 * 		python in C applications can be found at 
 * 		https://docs.python.org/2/extending/embedding.html
 *
 ****************************************************************************/

#include "crp.h"
#include "crp_generateFilename.h"

#include <stdio.h>
#include <stdlib.h>

#include <wiringSerial.h>


/**** Function crp_sensorInit ****
 * Initializes the interface to the sensor
 */
int crp_sensorInit(UART_DEVICE *device) {

	int sensor_fd;

	device->uart_fd = sensor_fd;
	device->metadata.crp_imageBufSize = 0;
	device->crp_receivingImage = 0;

	sensor_fd = serialOpen(CRP_SERIAL_DEVICE, CRP_SERIAL_BAUD);

	// Reset line on GPIO pin

	// SYNC with sensor
	crp_sensorSync(sensor_fd);

	// Send initial config
	crp_sendCommand(CRP_COMMAND_INITIAL);

	return sensor_fd;

} // Function crp_sensorInit


/**** Function crp_sensorSync ****
 * Sends sync command to the sensor
 */
int crp_sensorSync(UART_DEVICE *device) {

	int i, j;
	int synced = 0;

	char input;
	char rxBuf[6];
	char ackReceived[] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
	char syncCommand[] = CRP_COMMAND_SYNC_INIT;

	for(i = 0; i < 60; i++) {
		crp_sensorSendCommand(fd, syncCommand);

		// Delay 5 ms
		es_nsWait(5000000);

		// Check for crp input
		while(serialDataAvail(crpDevice.uart_fd)) {
			// Read serial input
			rc = es_uartGetChar(&crpDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("\tFailed to read from CRP channel\n");
				return rc;
#endif	// ES_DEBUG_MODE
			}
		}

		if(crpDevice.inputBufferSize > 0) {
#ifdef	ES_DEBUG_MODE
			printf("%d bytes in crp buffer\n", crpDevice.inputBufferSize);
#endif
			rc = crp_parseData(&crpDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("\tFailed\n");
				return rc;
#endif
			}
		}
	}

	return 0;

} // Function crp_sensorSync


/**** Function crp_parseData ****
 * Parses data received on UART
 */
int crp_parseData(int fd, char *buffer, int numPackets) {

	int packetsReceived = 0;
	char commandId = 0;

	if(device == NULL) {
		printf("Invalid buffer for crp packet\n");
		return -1;
	}

	while(i <= device->inputBufferSize - 6 && packetsReceived < numPackets) {

		int temp, parseIndex;
		char params[4];

		// Determine if current position is start of a packet
		if(device->metadata.crp_receivingImage) {
			if(device->metadata.crp_imageBufSize == 0) {
				crp_generateFilename(device->metadata.crp_imageFilename, CRP_FILENAME_SIZE, 0);
				// Open file to write to
				rc = device->metadata.crp_image_fd = open(device->metadata.crp_imageFilename, O_APPEND | O_CREAT | O_RDONLY, 0666);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					perror("Failed to open and create CRP file");
					return rc;
#endif
				}
			}
			// Receiving image data, just put straight into image buffer
			if(device->metadata.crp_imageBufSize < CRP_IMAGE_SIZE) {

				// Put input byte into buffer
				crp_imageBuf[device->metadata.crp_imageBufSize] = device->inputBuffer[i];

				// Write to image file
				rc = write(device->metadata.crp_image_fd, &(device->inputBuffer[i]), 1);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					perror("Failed to write image data to file");
#endif
				}

				// Increment buffer size counter
				device->metadata.crp_imageBufSize++;
				i++;
			}
			// Cut losses, if it got here as 'else' something went wrong

			// Check if that was last byte in buffer
			if(device->metadata.crp_imageBufSize < CRP_IMAGE_SIZE) {
				device->metadata.crp_receivingImage = 0;
				device->metadata.crp_imageCount++;
				// Close file
				rc = close(device->metadata.crp_image_fd);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to close CRP image file\n");
#endif
				}
				device->metadata.crp_image_fd = -1;
			}

		} else if(device->inputBuffer[i] == 0xAA) {

			packetsReceived++;

			// Copy packet so its easier to work with

			commandId = (device->inputBuffer[i + 1]);
			for(parseIndex = 0; parseIndex < 4; parseIndex++) {
				params[parseIndex] = device->inputBuffer[i + 2 + parseIndex];
			}

#ifdef	ES_DEBUG_MODE
			printf("Command ID is 0x%02x\n", commandId);
			printf("Params are 0x%02x 0x%02x 0x%02x 0x%02x\n", data,
					params[0],
					params[1],
					params[2],
					params[3]);
#endif	// ES_DEBUG_MODE

			// Switch case selecting the current command
			switch(commandId) {

				case 0x0A:
					// Tell device struct its receiving data next
					device->metadata.crp_receivingImage = 1;
					device->metadata.crp_imageBufSize = 0;
					// Get size of image
					temp = (device->inputBuffer[i + 3] << 16) |
						(device->inputBuffer[i + 4] << 8) |
						(device->inputBuffer[i + 5]);
					if(temp != CRP_IMAGE_SIZE) {
						// Well darn
						printf("Camera sending incorrect image size\n");
					}

					break;
				case 0x0E: // ACK
					// Do nothing, but at least its a known command

					break;
				default:
					printf("Unknown command received from image sensor\n");
					break;

			}

			char dataString[64];
			int chipIndex, rc;
			struct timespec currentTime;

			// Adjust pointer into input buffer
			i += 6;
			lastPacket = i;

		} else {
			// Not start of a packet, move on to next potential frame
			i++;
		}

	}

	// Number of bytes in transfer remaining after read
	bytesLeft = device->inputBufferSize - lastPacket;

	if(bytesLeft >= 6) {
#ifdef	ES_DEBUG_MODE
		printf("%d unpacketized bytes received by MDE\n\t", bytesLeft);
		for(i = 0; i < bytesLeft; i++) {
			printf("0x%02x ", device->inputBuffer[bytesLeft]);
		}
#endif
		bytesLeft %= 6;
	}

	// Every whole packet has been read, clear them from buffer
	for(i = 0; i < device->inputBufferSize; i++) {
		device->inputBuffer[i] = device->inputBuffer[lastPacket + i];
	}

	// Update buffer size
	device->inputBufferSize = bytesLeft;

} // Function crp_parseInput


/**** Function crp_sensorSendCommand ****
 * Sends a command through UART to the uCAM-III
 */
int crp_sensorSendCommand(int fd, char *buffer) {

	char command[6];
	int i;

	for(i = 0; i < 6; i++) {
		serialPutchar(fd, buffer[i]);
	}

	return 0;

} // Function crp_sensorRead


/**** Function crp_sensorRead ****
 * Reads an image from the sensor and places it into the buffer.
 */
int crp_sensorRead(int fd, char *buffer, int bufSize) {

	// Send image read command



	return 0;

} // Function crp_sensorRead


/**** Function crp_imageStore ****
 * Stores an image that has been read from a buffer into the file described
 * by df.
 */
int crp_imageStore(char *buffer, int bufSize, FILE *fd) {


	return 0;

} // Function crp_imageStore

