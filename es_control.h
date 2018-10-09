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

#include "comms/comms.h"
#include "crp/crp.h"
#include "crp/crp_generateFilename.h"
#include "mde/mde.h"
#include "telemetry/telemetry.h"


// System header files
#include <stdio.h>
#include <stdlib.h>

// Constants
#define MAX_COMMS_PACKET_SIZE	1024


/**** Function es_generateCommsPacket ****
 * Generates a packet to be sent to COMMS using data from telemetry sensors and
 * metadata from the payloads
 */
int es_generateCommsPacket(char *packet, int bufferLength,
		int *tempData, int pressureData,
		char *crpMetadata, char *mdeMetadata);

#endif // EAGLESAT_CONTROL_H

