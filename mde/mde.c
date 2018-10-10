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

#include <stdio.h>
#include <stdlib.h>


/**** Function mde_init ****
 * Initializes the UART interface to MDE and returns the UART file descriptor
 */
int mde_init(void) {

	int uart_fd;

	uart_fd = serialOpen(MDE_SERIAL_DEVICE);

	if(uart_fd == -1) {
		printf("Failed to open serial device %s\n", MDE_SERIAL_DEVICE);
		return -1;
	}

	return uart_fd;

} // Function mde_init


/**** Function mde_requestHealthPacket ****
 * Requests a health packet from MDE over UART channel
 */
int mde_requestHealthPacket(int fd) {

	int i;
	char inputBuffer[MDE_PACKET_LENGTH];

	if(metadata == NULL) {
		printf("Invalid buffer for health packet\n");
		return -1;
	}

	// Send byte to request health packet
	serialPutchar(MDE_COMMAND_HEALTH);

	// Read 6 bytes into UART
	// for(i = 0; i < MDE_PACKET_LENGTH; i++) {
	// 	// Blocks for 10 seconds - bad
	// 	// Try a direct read for better results
	// 	buffer[i] = serialGetChar(fd);
	// }

	return 0;

} // Function mde_requestPacket


/**** Function mde_parseData ****
 * Parses a received packet 
 */
int mde_parseData(UART_DEVICE *device) {

	int i, j, lastPacket, bytesLeft;
	int data;

	if(device == NULL) {
		printf("Invalid buffer for mde packet\n");
		return -1;
	}

	i = 0;
	while(i < device->inputBufferSize - 6) {

		// Determine if current position is start of a packet
		if(device->inputBuffer[i] == MDE_PACKET_HEADER_BYTE &&
				device->inputBuffer[i + 1] == MDE_PACKET_HEADER_BYTE &&
				device->inputBuffer[i + 2] == MDE_PACKET_HEADER_BYTE) {

			// Copy packet so its easier to work with

			data = device->inputBuffer[i + 3] << 16 & device->inputBuffer[i + 4] << 8 & device->inputBuffer[i + 5] ;

			// Determine if health or data packet
			if(data & (1 << MDE_PACKET_TYPE_BIT)) {
				// Health packet

				device->metadata->mde_chipsInactive;
				// Parse data from packet
				// Count responsive chips
				for(j = 0; j < 5; j++) {
					device->metadata->mde_chipsInactive += (data >> (14 + j)) & 1;
				}

				// Get start point index
				device->metadata->mde_cycleStart += (data >> 11) & 0x0f;

				// Get offset point index
				device->metadata->mde_cycleOffset += (data >> 8) & 0x07;

				// Get cycle count
				device->metadata->mde_cycleCount += data & 0xff;

				// Adjust pointer into input buffer
				lastPacket = i;
				i += 6;

			} else {
				char dataString[64];
				int chipIndex, errorVal, rc;
				struct timeval currentTime;

				// Data packet

				rc = clock_gettime(CLOCK_REALTIME, &currentTime);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to get time\n");
					return rc;
#endif
				}

				chipIndex = (data >> 20) & 0x07;

				snprintf(dataString, 64, "%d.%09d: Chip %d: Error packet 0x%06x\n");

				// Log to log file
				es_logString(device, dataString);
			}

		} else {
			// Not start of a packet, move on to next potential frame
			i++;
		}

	}

	// Number of bytes in transfer remaining after read
	bytesLeft = device->inputBufferSize - lastPacket;

	if(bytesLeft >= 6) {
#ifdef	ES_DEBUG_MODE
		printf("%d unpacketized bytes received by MDE\n", bytesLeft);
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


