/*****************************************************************************
 *
 * File:
 * 	es_control.c
 *
 * Description:
 * 	Contains functions for controlling system-level aspects of the
 * 	satellite (balloon), not specific to one subsystem (other than OBC)
 *
 * Revision 1.0
 * 	Date: 10/6/18
 *	Author: David Stockhouse
 *	Changes: Created from template
 *
 ****************************************************************************/

// User application header files

// System header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <wiringSerial.h>

#include "es_control.h"

/**** Function es_generateCommsPacket ****
 * Generates a packet to be sent to COMMS using data from telemetry sensors and
 * metadata from the payloads
 */
int es_generateCommsPacket(char *packet, int bufferLength,
		struct timespec *missionTime,
		TELEMETRY_DATA *telemetry,
		METADATA *crpData,
		METADATA *mdeData) {

	// Length of the packet to be generated
	int packetLength = 0;

	// Time variables
	int missionHours, missionMinutes, missionSeconds;

	if (packet == NULL) {
		// Error, can't store anything
		return -1;
	}

	// Setup mission time from struct
	missionHours = missionTime->tv_sec / 3600;
	missionMinutes = (missionTime->tv_sec % 3600) / 60;
	missionSeconds = missionTime->tv_sec % 60;

	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
			"T:%02d:%02d:%02d, ", missionHours, missionMinutes, missionSeconds);

	// Compensate for null character printed
	if(packet[packetLength - 1] == '\0') {
		packetLength--;
	}

// 	// Put TLM data into packet
// 	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
// 			"Pt:%03.1f:%03.1f:%03.1f,Ot:%03.1f,Ct:%03.1f,Mt:%03.1f,Rt:%03.1f,Pr:%03.1f,", 
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_EPS1],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_EPS2],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_EPS3],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_OBC],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_CRP],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_MDE],
// 			telemetry->temperature[TELEMETRY_TEMP_SENSOR_COMMS],
// 			telemetry->pressure);
// 
// 	if(packet[packetLength - 1] == '\0') {
// 		packetLength--;
// 	}

	// Put metadata from CRP and MDE into packet
 	if(crpData != NULL) {
 		packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
 				"CRP:%d, ", crpData->crp_started);
 	} else {
 		packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
 				"nCRP, ");
 	}
//
//	if(packet[packetLength - 1] == '\0') {
//		packetLength--;
//	}
	
	if(mdeData != NULL) {
		char mdeString[32];

		snprintf(mdeString, 32, "in%d:st%d:os%d:cc%d",
				mdeData->mde_chipsInactive,
				mdeData->mde_cycleStart,
				mdeData->mde_cycleOffset,
				mdeData->mde_cycleCount);

		packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
				"MDE:%s,", mdeString);

#ifdef	ES_DEBUG_MODE
			// printf("In packet: Address of metadata: %p\n", mdeData);
#endif	// ES_DEBUG_MODE

	} else {
		packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
				"nMDE,");
	}

	if(packet[packetLength - 1] == '\0') {
		packetLength--;
	}

	// Put CR as last character instead of comma
	packet[packetLength - 1] = '\r';

	packet[packetLength] = '\0';

	return packetLength;
}


/**** Function es_timeDifference ****
 * Gets the difference between two timespec objects.
 * Adapted from code written by Dr. Sam Siewert for CEC450
 */
int es_timeDifference(struct timespec *stop, struct timespec *start,
		struct timespec *delta_t) {
	int dt_sec=stop->tv_sec - start->tv_sec;
	int dt_nsec=stop->tv_nsec - start->tv_nsec;

	if(dt_sec >= 0)
	{
		if(dt_nsec >= 0)
		{
			delta_t->tv_sec=dt_sec;
			delta_t->tv_nsec=dt_nsec;
		}
		else
		{
			delta_t->tv_sec=dt_sec-1;
			delta_t->tv_nsec=1000000000+dt_nsec;
		}
	}
	else
	{
		if(dt_nsec >= 0)
		{
			delta_t->tv_sec=dt_sec;
			delta_t->tv_nsec=dt_nsec;
		}
		else
		{
			delta_t->tv_sec=dt_sec-1;
			delta_t->tv_nsec=1000000000+dt_nsec;
		}
	}

	return 0;
}


/**** Function es_nsWait ****
 * Wrapper for system calls to wait for a number of nanoseconds
 */
int es_nsWait(int ns)
{
	struct timespec delayTime, remainingTime;
	int rv;

	delayTime.tv_sec = ns / 1000000000;
	delayTime.tv_nsec = ns % 1000000000;

	nanosleep(&delayTime, &remainingTime);

	rv = remainingTime.tv_sec * 1000000000 + remainingTime.tv_nsec;

	return rv;

}


/**** Function es_uartGetChar ****
 * Reads a character into the input buffer of a uart device
 */
int es_uartGetChar(UART_DEVICE *device)
{

	if(device == NULL) {
		printf("NULL device for uartGetChar\n");
		return -1;
	}

	if(device->inputBufferSize >= INPUT_BUFFER_LENGTH) {
		printf("Input buffer is full\n");
		return device->inputBufferSize;
	}

	if(!serialDataAvail(device->uart_fd)) {
		printf("No input chars available\n");
		return -3;
	}

	device->inputBuffer[device->inputBufferSize] = serialGetchar(device->uart_fd);
#ifdef	ES_DEBUG_MODE
	// printf("\t\tInput buffer size is %d\n", device->inputBufferSize);
	// printf("\t\tReceived char 0x%02x\n", device->inputBuffer[device->inputBufferSize]);
#endif	// ES_DEBUG_MODE
	device->inputBufferSize++;

	return 0;
}


/**** Function es_syslog ****
 * Logs a string into a device
 */
int es_syslog(char *filename, char *string)
{
	int log_fd;
	int rc;

	if(filename == NULL) {
		printf("NULL filename for syslog\n");
		return -1;
	}

	// Open log file to append to, creating if necessary
	// log_fd = open(device->logFilename, O_APPEND | O_CREAT, 0666);
	log_fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0666);

#ifdef	ES_DEBUG_MODE
	// printf("FD for log file is %d\n", log_fd);
#endif

	if(log_fd < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to open log file %s\n", filename);
		return log_fd;
#endif
	}

	// Write string to file
	rc = write(log_fd, string, strlen(string));
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to write to log file %s\n", filename);
		perror("Failed to write to file");
		return rc;
#endif
	}

	rc = close(log_fd);
	if(rc) {
#ifdef	ES_DEBUG_MODE
		printf("Failed\n");
		return rc;
#endif
	}

	return 0;
}


/**** Function es_logString ****
 * Logs a string into a device
 */
int es_logString(UART_DEVICE *device, char *string)
{
	int log_fd;
	int rc;

	if(device == NULL) {
		printf("NULL device for logString\n");
		return -1;
	}

	// Open log file to append to, creating if necessary
	// log_fd = open(device->logFilename, O_APPEND | O_CREAT, 0666);
	log_fd = open(device->logFilename, O_CREAT | O_APPEND | O_WRONLY, 0666);

#ifdef	ES_DEBUG_MODE
	// printf("FD for log file is %d\n", log_fd);
#endif

	if(log_fd < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to open log file %s\n", device->logFilename);
		return log_fd;
#endif
	}

	// Write string to file
	rc = write(log_fd, string, strlen(string));
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to write to log file %s\n", device->logFilename);
		perror("Failed to write to file");
		return rc;
#endif
	}

	rc = close(log_fd);
	if(rc) {
#ifdef	ES_DEBUG_MODE
		printf("Failed\n");
		return rc;
#endif
	}

	return 0;
}


/**** Function es_generateLogFilename ****
 * Generate a filename that matches the format 
 * "str-mm.dd.yyyy-hh:mm:ss.log"
 */
int es_generateLogFilename(char *buf, int bufSize, char *subsystem) {

	int charsWritten;

	// Time variables
	time_t t;
	struct tm currentTime;

	// Get current time in UTC
	t = time(NULL);
	currentTime = *localtime(&t);

	// Create filename using date/time and exposure length
	charsWritten = snprintf(buf, bufSize, 
			"/home/pi/logs/%s-%02d.%02d.%04d_%02d:%02d:%02d.log",
			subsystem,
			currentTime.tm_mon + 1,
			currentTime.tm_mday,
			currentTime.tm_year + 1900,
			currentTime.tm_hour,
			currentTime.tm_min,
			currentTime.tm_sec);

	// Return length of the new string
	return charsWritten;

} // Function es_generateLogFilename


