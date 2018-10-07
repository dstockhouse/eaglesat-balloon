/*****************************************************************************
 *
 * File:
 * 	main.c
 *
 * Description:
 * 	The main function for the balloon test flight. Refers to functions
 * 	found in other directories.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created
 *
 * Revision 1.1
 * 	Date: 10/6/18
 *	Author: David Stockhouse
 *	Changes: Filled template with skeleton of operation and placeholder
 *		function calls
 *
 ****************************************************************************/

#include "comms/comms.h"
#include "crp/crp.h"
#include "crp/crp_generateFilename.h"
#include "mde/mde.h"
#include "telemetry/telemetry.h"

#include <stdio.h>
#include <stdlib.h>

// For testing program in debug mode
#define	ES_DEBUG_MODE


/**** Function main ****
 * Initializes the two payloads and starts collecting image data. 
 */
int main() {

	// Return code for each function call that might fail
	int rc;

	// Ends main loop if true
	int abortTest = 0;

	// Number of cycles of the main loop executed so far
	int cycleCount = 0;

	// Size of each generated packet to be sent to comms
	int packetSize;
	char commsPacket[MAX_COMMS_PACKET_SIZE];

	// Telemetry variables
	float temperature[TELEMETRY_NUM_TEMP_SENSORS], pressure;

	/***** Check device connections *****/

	// Ensure watchdog timer operating

	// Ensure proper scheduling parameters set


	/***** Initialize all communication interfaces *****/

	// Initialize UART to CRP sensor
	rc = crp_sensorInit();
	if (rc) {
		// Sensor initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize CRP sensor.\n");
		return rc;
#endif
	}

	// Initialize UART to COMMS transceiver
	rc = comms_uartInit();
	if (rc) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize COMMS UART.\n");
		return rc;
#endif
	}

	// Initialize UART to MDE board
	rc = mde_uartInit();
	if (rc) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize MDE UART.\n");
		return rc;
#endif
	}

	// Initialize SPI to ADC for telemetry sensors
	rc = telemetry_spiInit();
	if (rc) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize MCP3008 SPI.\n");
		return rc;
#endif
	}


	/***** Main program loop *****/

	while(!abortTest) {

		/***** Services to execute every 15 minutes *****/
		if (cycleCounter % (15 * 60) == 0 && cycleCount > 0) {

			// Get MDE health data

		}

		/***** Services to execute every 30 seconds *****/
		if (cycleCounter % 30 == 0 && cycleCount > 0) {

		}

		/***** Services to execute each 1-second cycle *****/

		// Read from CRP sensor
		rc = crp_sensorRead(imageDataBuf, CRP_IMAGE_BUF_SIZE);
		if (rc < CRP_IMAGE_SIZE) {

			// Not entire image was read
#ifdef	ES_DEBUG_MODE
			printf("Only read %d bytes.\n", rc);
#endif

		}

		// Store image that was read
		rc = crp_imageStore(imageDataBuf, rc);
		if (rc) {
#ifdef	ES_DEBUG_MODE
			printf("Didn't successfully store image data.\n");
			return rc;
#endif
		}


		// Read telemetry sensors
		rc = telemetry_tempRead(&pressure);
		if (rc) {
#ifdef	ES_DEBUG_MODE
			printf("Didn't successfully read temp sensors.\n");
			return rc;
#endif
		}

		rc = telemetry_tempRead(temperature, TELEMETRY_NUM_TEMP_SENSORS);
		if (rc) {
#ifdef	ES_DEBUG_MODE
			printf("Didn't successfully read temp sensors.\n");
			return rc;
#endif
		}


		// Generate packet to send to comms
		packetSize = es_generateCommsPacket(char *commsPacket, MAX_PACKET_SIZE,
				temperature, pressure, NULL, NULL);
		if (packetSize > 0) {
			// Send data or heartbeat to comms
			rc = comms_sendPacket(commsPacket, packetSize);
			if (rc) {
#ifdef	ES_DEBUG_MODE
				printf("Couldn't send COMMS packet.\n");
				return rc;
#endif
			}
		}



		// Increment cycle counter for task scheduling
		cycleCount++;

	} // while(!abortTest)

	// End a successful test
	return 0;

} // Function main()

