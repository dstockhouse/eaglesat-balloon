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
#include <string.h>

#include <wiringSerial.h>

int comms_parseData(UART_DEVICE *device) {

	int i, rc;

	i = 0;
	// Search 8 chars in future for match string
	while(i < device->inputBufferSize - 8) {
		rc = strncmp(&(device->inputBuffer[i]), "<UI>:\r\n", 7);
		if(!rc){
			// Packet is either sent here or from the ground
			if(device->inputBuffer[i + 8] == 'T' || device->inputBuffer[i + 8] == '.' ) {
				// Echoed by sending to the buffer, so ignore
			} else if (device->inputBuffer[i + 8] == 'r') {
				// Reset the board
#ifdef	ES_DEBUG_MODE
				printf("Received 'r' command from comms\n");
				printf("COMMS received ");
				for(i = 0; i < device->inputBufferSize && device->inputBuffer[i] != '<' && device->inputBuffer[i] != '>'; i++) {
					printf("%c", device->inputBuffer[i]);
				}
#else
				// Reset the system
				system("sudo reboot");
#endif
			}
			i += 8;
		} else {
			// Doesn't match start of packet
			i++;
		}
	}

	return 0;
}

/**** Function comms_init ****
 * Initializes UART for the comms module
 */
int comms_init(UART_DEVICE *device) {

	int uart_fd;
	int rc;

	// If on reboot, ensure COMMS soft UART device is active
	rc = comms_softUartInit();
	if(rc) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to initialize software UART\n");
		return rc;
#endif
	}

	uart_fd = serialOpen(COMMS_SERIAL_DEVICE, COMMS_SERIAL_BAUDRATE);

	if(uart_fd == -1) {
		printf("Failed to open serial device %s\n", COMMS_SERIAL_DEVICE);
		return -1;
	}
	device->uart_fd = uart_fd;

	// Read from UART for sensor initialization console (maybe later)
	// Delay to allow radio to boot up
	es_nsWait(10000000);

	// Give UART the start string
	comms_sendPacket(uart_fd, COMMS_COMMAND_START, 2);

	device->inputBufferSize = 0;
	device->outputBufferSize = 0;

	return uart_fd;

} // Function comms_init


/**** Function comms_close ****
 * Close down the file descriptors for COMMS
 */
int comms_close(int fd) {

	serialClose(fd);

	return 0;

} // Function comms_close


/**** Function comms_sendPacket ****
 * Sends a packet to the comms module
 */
int comms_sendPacket(int fd, char *buffer, int packetLen) {

	int i;

#ifdef	ES_DEBUG_MODE
	printf("COMMS output: ");
#endif

	// Send every byte in the packet
	for(i = 0; i < packetLen; i++) {
		serialPutchar(fd, buffer[i]);
#ifdef	ES_DEBUG_MODE
		putchar(buffer[i]);
#endif
	}

#ifdef	ES_DEBUG_MODE
	printf("\n");
#endif

	return 0;

} // Function comms_sendPacket


/**** Function comms_softUartInit ****
 * Initializes the soft_uart module if it is not already started. Not general,
 * this function expects files to be named a certain way for this pi config
 */
int comms_softUartInit(void) {

	int i;
	int rc;

	rc = system("test ! -e /dev/ttySOFT0 -a -e /home/pi/soft_uart/soft_uart.ko");
	if(rc) {
#ifdef	ES_DEBUG_MODE
		printf("Soft UART already initialized\n");
		return 0;
#endif
	}

	rc = system("sudo insmod /home/pi/soft_uart/soft_uart.ko");
	if(rc) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to start SOFT UART\n");
		return rc;
#endif
	}

	return 0;

} // Function comms_softUartInit


