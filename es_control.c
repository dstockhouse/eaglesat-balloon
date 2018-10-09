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
		float *tempData, float pressureData,
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
			"T:%02d:%02d:%02d,", missionHours, missionMinutes, missionSeconds);

	// Put TLM data into packet
	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
			"Pt:%03.1f:%03.1f:%03.1f,Ot:%03.1f,Ct:%03.1f,Mt:%03.1f,Rt:%02f,", 
			tempData[TELEMETRY_TEMP_SENSOR_EPS1],
			tempData[TELEMETRY_TEMP_SENSOR_EPS2],
			tempData[TELEMETRY_TEMP_SENSOR_EPS3],
			tempData[TELEMETRY_TEMP_SENSOR_OBC],
			tempData[TELEMETRY_TEMP_SENSOR_CRP],
			tempData[TELEMETRY_TEMP_SENSOR_MDE],
			tempData[TELEMETRY_TEMP_SENSOR_COMMS]);

	packetLength += snprintf(&packet[packetLength], bufferLength - packetLength,
			"PR:%03.1f", pressureData);

	// Put metadata from CRP and MDE into packet


	return 0;
}



/**** Function es_timeDifference ****
 * Gets the difference between two timespec objects.
 * Adapted from code written by Dr. Sam Siewert for CEC450
 */
int es_timeDifference(struct timespec *stop, struct timespec *start,
		struct timespec *delta_t)
{
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

  return(OK);
}


