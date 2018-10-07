/*****************************************************************************
 *
 * File:
 * 	serial.c
 *
 * Description:
 * 	Demonstration of UART on the pi
 *
 * Revision 1.0
 * 	Date: 10/6/2018
 *	Author: David Stockhouse
 *	Changes: Created file
 *
 * Revision 1.1
 * 	Date: 10/6/2018
 *	Author: David Stockhouse
 *	Changes: Converted into loopback test. Must have a USB FTDI serial 
 *		converter at /dev/USB0 and plugged into serial0
 *
 ****************************************************************************/

// User application header files

// System header files
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <wiringSerial.h>

#define UART0_DEV	"/dev/ttyUSB0"
#define UART1_DEV	"/dev/ttySOFT0"
#define BAUD_RATE	1200

int main(void) {

	int uart0_fd, uart1_fd;
	int input;

	// Open serial interfaces identified by the device name
	uart0_fd = serialOpen(UART0_DEV, BAUD_RATE);

	if(uart0_fd == -1) {
		perror("Failed to open serial device 0: ");
		return -1;
	}

	uart1_fd = serialOpen(UART1_DEV, BAUD_RATE);

	if(uart1_fd == -1) {
		perror("Failed to open serial device 1: ");
		return -1;
	}


	// Print test string to UART
	serialPuts(uart0_fd, "Shim sham\n\r");
	serialPuts(uart1_fd, "Test\n\r");

	// Get strings from output, blocking up to ten seconds
	do {

		// input = serialGetchar(uart0_fd);

		// Print input to console if valid
		// if(input != -1) {
		// 	printf("0x%02x: %c\n", input, (char) input);
		// } else {
		// 	printf("Timeout 0\n");
		// }

		input = serialGetchar(uart1_fd);

		// Print input to console if valid
		if(input != -1) {
			printf("0x%02x: %c\n", input, (char) input);
		} else {
			printf("Timeout 1\n");
		}

		usleep(200000);

	} while(serialDataAvail(uart1_fd));

	// Close serial ports
	serialClose(uart0_fd);
	serialClose(uart1_fd);

}

