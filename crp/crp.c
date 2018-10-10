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
int crp_sensorInit(char *buffer, int bufSize) {

	int sensor_fd;

	sensor_fd = serialOpen(CRP_SERIAL_DEVICE, CRP_SERIAL_BAUD);

	// Reset line on GPIO pin

	// SYNC with sensor
	// crp_sensorSync(sensor_fd);
	// serialPutchar(sensor_fd, 

	return sensor_fd;

} // Function crp_sensorInit


/**** Function crp_sensorSync ****
 * Sends sync command to the sensor
 */
int crp_sensorSync(char *buffer, int bufSize) {

	int sensor_fd;

	// crp_sensorSendCommand(CRP_COMMAND_SYNC);

} // Function crp_sensorSync


/**** Function crp_sensorSendCommand ****
 * Sends a command through UART to the uCAM-III
 */
int crp_sensorSendCommand(int fd, char *buffer, int bufSize) {

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


	return 0;

} // Function crp_sensorRead


/**** Function crp_imageStore ****
 * Stores an image that has been read from a buffer into the file described
 * by df.
 */
int crp_imageStore(char *buffer, int bufSize, FILE *fd) {


	return 0;

} // Function crp_imageStore

