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

// Constants
#define MAX_PACKET_SIZE	1024

/**** Function es_generateCommsPacket ****
 * Generates a packet to be sent to COMMS using data from telemetry sensors and
 * metadata from the payloads
 */
int es_generateCommsPacket(char *packet, int bufferLength,
		int *tempData, int pressureData,
		char *crpMetadata, char *mdeMetadata) {

	// Length of the packet to be generated
	int packetLength;

	if (packet == NULL) {
		// Error, can't store anything
		return -1;
	}

	return 0;
}

