/*****************************************************************************
 *
 * File:
 * 	comms.c
 *
 * Description:
 * 	Functions related to interfacing to the COMMS system.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function comms_sendPacket
 *
 * Revision 1.1
 * 	Date: 10/9/18
 *	Author: David Stockhouse
 *	Changes: Added comms_init function. Implemented UART receive 
 *		functionality
 *
 ****************************************************************************/

#include "comms.h"

#include <stdio.h>
#include <stdlib.h>


/**** Function comms_init ****
 * Initializes UART for the comms module
 */
int comms_init() {

	int uart_fd;

	uart_fd = serialOpen(COMMS_SERIAL_DEVICE);

	if(uart_fd == -1) {
		printf("Failed to open serial device %s\n", COMMS_SERIAL_DEVICE);
		return -1;
	}

	// Read from UART for sensor initialization console

	// Give UART the start string
	comms_sendPacket(uart_fd, COMMS_COMMAND_START, 2);

	return uart_fd;

} // Function comms_init


/**** Function comms_close ****
 * Close down the file descriptors for COMMS
 */
int comms_init() {

	int uart_fd;

	uart_fd = serialOpen(COMMS_SERIAL_DEVICE);

	if(uart_fd == -1) {
		printf("Failed to open serial device %s\n", COMMS_SERIAL_DEVICE);
		return -1;
	}

	// Read from UART for sensor initialization console

	// Give UART the start string
	comms_sendPacket(uart_fd, COMMS_COMMAND_START, 2);

	return uart_fd;

} // Function comms_init


/**** Function comms_sendPacket ****
 * Sends a packet to the comms module
 */
int comms_sendPacket(int fd, char *buffer, int packetLen) {

	int i;

	// Send every byte in the packet
	for(i = 0; i < bufSize; i++) {
		serialPutchar(fd, buffer[i]);
	}

	return i;

} // Function comms_sendPacket


