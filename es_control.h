/*****************************************************************************
 *
 * File:
 * 	es_control.h
 *
 * Description:
 * 	Contains control function prototypes and constants for controlling 
 * 	system-level aspects of the satellite (balloon), not specific to one 
 * 	subsystem (other than OBC)
 *
 * Revision 1.0
 * 	Date: 10/6/18
 *	Author: David Stockhouse
 *	Changes: Created from template
 *
 ****************************************************************************/

#ifndef	EAGLESAT_CONTROL_H
#define	EAGLESAT_CONTROL_H


// User application header files

// #include "comms/comms.h"
// #include "crp/crp.h"
// #include "crp/crp_generateFilename.h"
// #include "mde/mde.h"
#include "telemetry/telemetry.h"


// System header files
#include <stdio.h>
#include <stdlib.h>


// Constants
#define ES_DEBUG_MODE
// #define ES_DEBUG_NO_CRP
#define ES_DEBUG_NO_COMMS
#define ES_DEBUG_NO_MDE

#define MAX_COMMS_PACKET_SIZE	1024

#define INPUT_BUFFER_LENGTH	16384

#define LOG_FILENAME_LENGTH	64


// Metadata for each UART device
typedef struct {
	// CRP
// 	int crp_imageCount;
// 	int crp_receivingImage;
// 	char crp_imageBuf[CRP_IMAGE_SIZE];
// 	int crp_imageBufSize;
// 	char crp_imageFilename[CRP_FILENAME_SIZE];
// 	int crp_image_fd;
// 	int crp_synced;
	// MDE
	int mde_chipsInactive;
	int mde_cycleStart;
	int mde_cycleOffset;
	int mde_cycleCount;
	// COMMS
} METADATA;

// Structure for every object that is a UART device
typedef struct {
	int uart_fd;
	char inputBuffer[INPUT_BUFFER_LENGTH];
	char logFilename[LOG_FILENAME_LENGTH];
	int inputBufferSize;
	char outputBuffer[INPUT_BUFFER_LENGTH];
	int outputBufferSize;
	int active; // Only used for CRP
	METADATA metadata;
} UART_DEVICE;


/**** Function es_generateCommsPacket ****
 * Generates a packet to be sent to COMMS using data from telemetry sensors and
 * metadata from the payloads
 */
int es_generateCommsPacket(char *packet, int bufferLength,
		struct timespec *missionTime,
		TELEMETRY_DATA *telemetry,
		METADATA *crpData,
		METADATA *mdeData);

int es_timeDifference(struct timespec *stop, struct timespec *start,
		struct timespec *delta_t);

int es_nsWait(int ns);

int es_uartGetChar(UART_DEVICE *device);

int es_logString(UART_DEVICE *device, char *string);

int es_generateLogFilename(char *buf, int bufSize, char *subsystem);


#endif // EAGLESAT_CONTROL_H

