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


/**** Function mde_receivePacket ****
 * Requests a health packet from MDE over UART channel
 */
int mde_receivePacket(int fd, MDE_METADATA *metadata) {

	int i;
	char inputBuffer[MDE_PACKET_LENGTH];

	if(metadata == NULL) {
		printf("Invalid buffer for health packet\n");
		return -1;
	}

	if(serialDataAvail(fd) < 6) {

	// Read 6 bytes into UART
	for(i = 0; i < MDE_PACKET_LENGTH; i++) {
		// Blocks for 10 seconds - bad
		// Try a direct read for better results
		buffer[i] = serialGetChar(fd);
	}

	return 0;

} // Function mde_receivePacket


