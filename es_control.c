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

// Constants
#define MAX_PACKET_SIZE	1024

/**** Function es_generateCommsPacket ****
 * Generates a packet to be sent to COMMS using data from telemetry sensors and
 * metadata from the payloads
 */
int es_generateCommsPacket(char *packet, int bufferLength,
		struct timespec *missionTime,
		int *tempData, int pressureData,
		char *crpMetadata, char *mdeMetadata) {

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
	missionSeconds = missionTime->tv_sec % 3600;

	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
			"T:%d:%d:%d,", missionHours, missionMinutes, missionSeconds);

	// Put TLM data into packet
	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
			"Pt:%d:%d:%d,Ot:%d,Ct:%d,", missionHours, missionMinutes, missionSeconds);

	return 0;
}

