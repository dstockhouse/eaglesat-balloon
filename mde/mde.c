/*****************************************************************************
 *
 * File:
 * 	mde.c
 *
 * Description:
 * 	Functions related to interfacing to the MDE system.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with functions mde_requestPacket and
 *		mde_init
 *
 * Revision 1.1
 * 	Date: 10/9/18
 *	Author: David Stockhouse
 *	Changes: Filled in UART functionality
 *
 ****************************************************************************/

#include "mde.h"

#include "../es_control.h"
#include "../telemetry/telemetry.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <wiringSerial.h>


/**** Function mde_init ****
 * Initializes the UART interface to MDE and returns the UART file descriptor
 */
int mde_init(UART_DEVICE *device) {

	int uart_fd;

	uart_fd = serialOpen(MDE_SERIAL_DEVICE, MDE_SERIAL_BAUDRATE);
#ifdef	ES_DEBUG_MODE
	// printf("Got %d as MDE FD\n", uart_fd);
#endif

	if(device == NULL) {
#ifdef	ES_DEBUG_MODE
		printf("MDE init device cannot be NULL\n");
#endif
	}

	if(uart_fd < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to open serial device %s\n", MDE_SERIAL_DEVICE);
#endif
	}

	device->inputBufferSize = 0;
	device->outputBufferSize = 0;

	device->uart_fd = uart_fd;

	return uart_fd;

} // Function mde_init


/**** Function mde_requestHealthPacket ****
 * Requests a health packet from MDE over UART channel
 */
int mde_requestHealthPacket(int fd) {

	// Send byte to request health packet
#ifdef	ES_DEBUG_MODE
	// printf("Sending 0x%02x to MDE\n", MDE_COMMAND_HEALTH);
#endif	// ES_DEBUG_MODE
	serialPutchar(fd, MDE_COMMAND_HEALTH);

	return 0;

} // Function mde_requestPacket


/**** Function mde_parseData ****
 * Parses a received packet 
 */
int mde_parseData(UART_DEVICE *device) {

	int i, j;
	int lastPacket = 0, bytesLeft = 0;
	int data;

	if(device == NULL) {
		printf("Invalid buffer for mde packet\n");
		return -1;
	}

#ifdef	ES_DEBUG_MODE
	// printf("Inside mde_parse, buffer size is %d\n", device->inputBufferSize);
	// printf("Inside mde_parse: array pointer is %p\n", device->inputBuffer);
	// printf("Inside mde_parse: first element is %x\n", device->inputBuffer[0]);
#endif


	i = 0;
	while(i <= device->inputBufferSize - 6) {
		int temp;

#ifdef	ES_DEBUG_MODE
		// printf("Examining ");
		// printf("%02x %02x %02x %02x %02x %02x \n",
			       	// device->inputBuffer[i],
				// device->inputBuffer[i + 1],
				// device->inputBuffer[i + 2],
				// device->inputBuffer[i + 3],
				// device->inputBuffer[i + 4],
				// device->inputBuffer[i + 5]);

		// printf("\tChecking if %d == 0x80\n", device->inputBuffer[i]);
		// printf("\tChecking if %d == 0x80\n", device->inputBuffer[i + 1]);
		// printf("\tChecking if %d == 0x80\n", device->inputBuffer[i + 2]);
#endif

		// Determine if current position is start of a packet
		if(device->inputBuffer[i] == 0x80 &&
				device->inputBuffer[i + 1] == 0x80 &&
				device->inputBuffer[i + 2] == 0x80) {

			// Copy packet so its easier to work with

#ifdef	ES_DEBUG_MODE
			// printf("Data should be %x%x%x\n",
					// (device->inputBuffer[i + 3] << 16),
					// (device->inputBuffer[i + 4] << 8),
					// (device->inputBuffer[i + 5]));
#endif	// ES_DEBUG_MODE
			data = (device->inputBuffer[i + 3] << 16) |
				(device->inputBuffer[i + 4] << 8) |
				(device->inputBuffer[i + 5]);

#ifdef	ES_DEBUG_MODE
			// printf("Got data as 0x%08x\n", data);
#endif	// ES_DEBUG_MODE

			// if(data & (1 << MDE_PACKET_TYPE_BIT)) {

#ifdef	ES_DEBUG_MODE
			temp = data & (1 << MDE_PACKET_TYPE_BIT);
			// printf("Run if %d\n", temp);
#endif	// ES_DEBUG_MODE

			// Determine if health or data packet
			if(data & (1 << MDE_PACKET_TYPE_BIT)) {
				// Health packet

				char dataString[64];
				int chipIndex, rc;
				struct timespec currentTime;

#ifdef	ES_DEBUG_MODE
			// printf("Run\n");
#endif	// ES_DEBUG_MODE

				device->metadata.mde_chipsInactive = 0;
				// Parse data from packet
				// Count responsive chips
				for(j = 0; j < 5; j++) {
					device->metadata.mde_chipsInactive += (data >> (14 + j)) & 1;
				}

				// Get start point index
				device->metadata.mde_cycleStart += (data >> 11) & 0x0f;

				// Get offset point index
				device->metadata.mde_cycleOffset += (data >> 8) & 0x07;

				// Get cycle count
				device->metadata.mde_cycleCount += data & 0xff;

#ifdef	ES_DEBUG_MODE

				// printf("MDE metadata: %d %d %d %d\n",
						// device->metadata.mde_chipsInactive,
						// device->metadata.mde_cycleStart,
						// device->metadata.mde_cycleOffset,
						// device->metadata.mde_cycleCount);

#endif	// ES_DEBUG_MODE

				rc = clock_gettime(CLOCK_REALTIME, &currentTime);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to get time\n");
					return rc;
#endif
				}

				chipIndex = (data >> 20) & 0x07;

				snprintf(dataString, 64, "%d.%09d: MDE metadata: %d %d %d %d\n",
						(int) currentTime.tv_sec,
						(int) currentTime.tv_nsec,
						device->metadata.mde_chipsInactive,
						device->metadata.mde_cycleStart,
						device->metadata.mde_cycleOffset,
						device->metadata.mde_cycleCount);

				// Log to log file
				es_logString(device, dataString);

			} else {

#ifdef	ES_DEBUG_MODE
			// printf("Don't run\n");
#endif	// ES_DEBUG_MODE

				char dataString[64];
				int chipIndex, rc;
				struct timespec currentTime;

				// Data packet

				rc = clock_gettime(CLOCK_REALTIME, &currentTime);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to get time\n");
					return rc;
#endif
				}

				chipIndex = (data >> 20) & 0x07;

				snprintf(dataString, 64, "%d.%09d: Chip %d: Error packet 0x%06x\n",
						(int) currentTime.tv_sec,
						(int) currentTime.tv_nsec,
						chipIndex, data);

				// Log to log file
				es_logString(device, dataString);

			}

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

	return 0;

} // Function mde_parseData


