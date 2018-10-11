/*****************************************************************************
 *
 * File:
 * 	main.c
 *
 * Description:
 * 	The main function for testing the functionality of the CRP interface 
 * 	software.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created.
 *
 ****************************************************************************/

#include "crp.h"
#include "crp_generateFilename.h"

#include <stdio.h>
#include <stdlib.h>

#include <wiringSerial.h>

/**** Function main ****
 * Test for the CRP software.
 */
int main() {

	char buf[64];
	char rxBuf[6];
	int byte = 0;
	int i = 0;
	int ackReceived = 0;
	int fd;
	int delay_us = 5000;

	fd = serialOpen("/dev/serial0", 9600);

	for(i = 0; i < 60 && !ackReceived; i++) {

		printf("Sending sync\n");
		printf("%02x ", 0xAA);
		printf("%02x ", 0x0D);
		printf("%02x ", 0x00);
		printf("%02x ", 0x00);
		printf("%02x ", 0x00);
		printf("%02x ", 0x00);
		serialPutchar(fd, 0xAA);
		serialPutchar(fd, 0x0D);
		serialPutchar(fd, 0x00);
		serialPutchar(fd, 0x00);
		serialPutchar(fd, 0x00);
		serialPutchar(fd, 0x00);

		usleep(delay_us);
		delay_us += 1000;

		while (serialDataAvail(fd) > 0 && !ackReceived) {
			rxBuf[i] = serialGetchar(fd);
			byte++;

			if(byte >= 6 && rxBuf[i - byte] == 0xAA) {
				printf("\tR: ");
				for(int j = 0; j < 6; j++) {
					printf("%02x ", rxBuf[j]);
				}
				printf("\n");

				if(rxBuf[0] == 0xAA &&
						rxBuf[1] == 0x0E &&
						rxBuf[2] == 0x0D) {
					// Ack
					printf("Got ack\n");
					ackReceived == 1;
				}
				byte = 0;
			}
		}
	}

	while (serialDataAvail(fd) > 0) {
		rxBuf[i] = serialGetchar(fd);
		byte++;

		if(byte >= 6) {
			printf("\tR: ");
			for(int j = 0; j < 6; j++) {
				printf("%02x ", rxBuf[j]);
			}
			printf("\n");

			if(rxBuf[0] == 0xAA &&
					rxBuf[1] == 0x0D &&
					rxBuf[2] == 0x00) {
				// Send ack
				printf("Sending ack\n");
				printf("%02x ", 0xAA);
				printf("%02x ", 0x0E);
				printf("%02x ", 0x0D);
				printf("%02x ", 0x00);
				printf("%02x ", 0x00);
				printf("%02x ", 0x00);
				serialPutchar(fd, 0xAA);
				serialPutchar(fd, 0x0E);
				serialPutchar(fd, 0x0D);
				serialPutchar(fd, 0x00);
				serialPutchar(fd, 0x00);
				serialPutchar(fd, 0x00);
			}
			byte = 0;
		}
	}

	// Send initial config
	printf("Sending config\n");
	printf("%02x ", 0xAA);
	printf("%02x ", 0x01);
	printf("%02x ", 0x00);
	printf("%02x ", 0x03);
	printf("%02x ", 0x03);
	printf("%02x ", 0x00);
	serialPutchar(fd, 0xAA);
	serialPutchar(fd, 0x01);
	serialPutchar(fd, 0x00);
	serialPutchar(fd, 0x03);
	serialPutchar(fd, 0x03);
	serialPutchar(fd, 0x00);

	usleep(1000);

	while (serialDataAvail(fd) > 0 && !ackReceived) {
		rxBuf[i] = serialGetchar(fd);
		byte++;

		if(byte >= 6) {
			printf("\tR: ");
			for(int j = 0; j < 6; j++) {
				printf("%02x ", rxBuf[j]);
			}
			printf("\n");

			if(rxBuf[0] == 0xAA &&
					rxBuf[1] == 0x0E &&
					rxBuf[2] == 0x0D) {
				// Ack
				printf("Got ack\n");
				ackReceived == 1;
			}
			byte = 0;
		}
	}

	// Send request for image
	printf("Requesting image\n");
	printf("%02x ", 0xAA);
	printf("%02x ", 0x01);
	printf("%02x ", 0x00);
	printf("%02x ", 0x03);
	printf("%02x ", 0x03);
	printf("%02x ", 0x00);
	serialPutchar(fd, 0xAA);
	serialPutchar(fd, 0x01);
	serialPutchar(fd, 0x00);
	serialPutchar(fd, 0x03);
	serialPutchar(fd, 0x03);
	serialPutchar(fd, 0x00);

	usleep(1000);

	int imageSize = 0;
	while(1) {
		while (serialDataAvail(fd) > 0 && !ackReceived) {
			rxBuf[i] = serialGetchar(fd);
			byte++;

			if(byte >= 6) {
				printf("\tR: ");
				for(int j = 0; j < 6; j++) {
					printf("%02x ", rxBuf[j]);
				}
				printf("\n");

				if(rxBuf[0] == 0xAA &&
						rxBuf[1] == 0x0E &&
						rxBuf[2] == 0x04) {
					// Ack
					printf("Got ack\n");
					ackReceived == 1;

				} else if(rxBuf[0] == 0xAA &&
						rxBuf[1] == 0x0E &&
						rxBuf[2] == 0x04) {
					imageSize = (rxBuf[3] << 16) | 
						(rxBuf[4] << 8) | 
						(rxBuf[5]);
					printf("Got image confirmation with size %d\n", imageSize);
				}

				byte = 0;
			}

			// Wait, just in case
			usleep(10);
		}
	}

	return 0;

} // Function main()

