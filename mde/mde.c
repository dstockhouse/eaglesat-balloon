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

	int i, j, lastPacket;
	char packet[MDE_PACKET_LENGTH];
	int metadata;

	if(device == NULL) {
		printf("Invalid buffer for mde packet\n");
		return -1;
	}

	i = 0;
	while(i < device->inputBufferSize) {

		// Determine if current position is start of a packet
		if(device->inputBuffer[i] == MDE_PACKET_HEADER_BYTE &&
				device->inputBuffer[i + 1] == MDE_PACKET_HEADER_BYTE &&
				device->inputBuffer[i + 2] == MDE_PACKET_HEADER_BYTE) {

			// Copy packet so its easier to work with
			for(j = 0; j < MDE_PACKET_LENGTH; j++) {
				packet[i + j] = device->inputBuffer[i + j];
			}

			metadata = packet[3] << 16 & packet[4] << 8 & packet[5];

			device->metadata->mde_chipsInactive;
			// Parse data from packet
			// Count responsive chips
			for(j = 0; j < 5; j++) {
				device->metadata->mde_chipsInactive += (metadata >> (14 + j)) & 1;
			}

			// Get start point index
			device->metadata->mde_cycleStart += (metadata >> 11) & 0x0f;

			// Get offset point index
			device->metadata->mde_cycleOffset += (metadata >> 8) & 0x07;

			// Get cycle count
			device->metadata->mde_cycleCount += metadata & 0xff;

			// Adjust pointer into input buffer
			lastPacket = i;
			i += 6;

		} else {
			// Not start of a packet, move on to next potential frame
			i++;
		}

	}

	// Every whole packet has been read, clear them from buffer
	for(i = 0; i < device->inputBufferSize; i++) {
		device->inputBuffer[i] = device->inputBuffer[lastPacket + i];
	}

	device->inputBufferSize = device->inputBufferSize - lastPacket;

	return 0;

} // Function mde_receivePacket


