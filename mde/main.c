/*****************************************************************************
 *
 * File:
 * 	main.c
 *
 * Description:
 * 	The main function for testing the functionality of the MDE interface 
 * 	software.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created.
 *
 ****************************************************************************/

#include "mde.h"

#include <stdio.h>
#include <stdlib.h>

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

	// Objects for each UART device
	UART_DEVICE mdeDevice;

	// Time variables
	struct timespec missionCurrentTime, missionStartTime, missionElapsedTime, currentLoopTime;

	/***** Check device connections *****/

	// Ensure watchdog timer operating

	/***** Get mission start time *****/
	clock_gettime(CLOCK_REALTIME, &missionStartTime);

	/***** Initialize all communication interfaces *****/

	// Initialize UART to CRP sensor
// 	crpDevice.uart_fd = crp_sensorInit();
// 	if (crpDevice.uart_fd) {
// 		// Sensor initialization failed
// #ifdef	ES_DEBUG_MODE
// 		printf("Couldn't initialize CRP sensor.\n");
// 		return crpDevice.uart_fd;
// #endif
// 	}

	// Initialize UART to COMMS transceiver
	commsDevice.uart_fd = comms_init();
	if (commsDevice.uart_fd) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize COMMS UART.\n");
		return commsDevice.uart_fd;
#endif
	}

	// Initialize UART to MDE board
	mdeDevice.uart_fd = mde_init();
	if (mdeDevice.uart_fd) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize MDE UART.\n");
		return mdeDevice.uart_fd;
#endif
	}

	/***** Initialize log filenames *****/

	es_generateFilename(crpDevice.logFilename, LOG_FILENAME_LENGTH, "CRP");
	es_generateFilename(commsDevice.logFilename, LOG_FILENAME_LENGTH, "COMMS");
	es_generateFilename(mdeDevice.logFilename, LOG_FILENAME_LENGTH, "MDE");

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

		/***** Services to execute each 1-second cycle *****/

// 		// Read from CRP sensor
// 		rc = crp_sensorRead(imageDataBuf, CRP_IMAGE_BUF_SIZE);
// 		if (rc < CRP_IMAGE_SIZE) {
// 
// 			// Not entire image was read
// #ifdef	ES_DEBUG_MODE
// 			printf("Only read %d bytes.\n", rc);
// #endif
// 
// 		}
// 
// 		// Store image that was read
// 		rc = crp_imageStore(imageDataBuf, rc);
// 		if (rc) {
// #ifdef	ES_DEBUG_MODE
// 			printf("Didn't successfully store image data.\n");
// 			return rc;
// #endif
// 		}

		// Read CRP sensor using python script


		// Read telemetry data
		rc = telemetry_allRead(&telemetry);
		if(rc) {
#ifdef	ES_DEBUG_MODE
			printf("Failed\n");
			return rc;
#endif
		}

		// 		// Read telemetry sensors
		// 		rc = telemetry_tempRead(&pressure);
		// 		if (rc) {
		// #ifdef	ES_DEBUG_MODE
		// 			printf("Didn't successfully read temp sensors.\n");
		// 			return rc;
		// #endif
		// 		}
		// 
		// 		rc = telemetry_tempRead(temperature, TELEMETRY_NUM_TEMP_SENSORS);
		// 		if (rc) {
		// #ifdef	ES_DEBUG_MODE
		// 			printf("Didn't successfully read temp sensors.\n");
		// 			return rc;
		// #endif
		// 		}


		/***** Services to execute every 30 seconds *****/
		if (cycleCounter % 30 == 0 && cycleCount > 0) {

			// Get current time
			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			// Get elasped time since start of mission
			es_timeDifference(&missionCurrentTime, &missionStartTime, &missionElapsedTime);

			// Generate packet to send to comms
			packetSize = es_generateCommsPacket(char *commsPacket, MAX_PACKET_SIZE,
					&telemetry, 
					&(crpDevice.metadata),
					&(mdeDevice.metadata));
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

		} // 30 second cycle counter
		else {
			// If no data sent, send heartbeat
			comms_sendPacket(".\r", 2);

		/***** Services to execute every 15 minutes *****/
		if (cycleCounter % (15 * 60) == 0 && cycleCount > 0) {

			// Get MDE health data
			mde_requestHealthPacket(mdeDevice.uart_fd);

		} // 15 minute cycle counter

		// Increment cycle counter for task scheduling
		cycleCount++;

		do {

			/***** Delay or poll UART channels *****/

			// Check for crp input
			while(serialDataAvail(crpDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(crpDevice.uart_fd);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from CRP channel\n");
					return rc;
#endif
				}
			}

			// Check for comms input
			while(serialDataAvail(commsDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(commsDevice.uart_fd);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from COMMS channel\n");
					return rc;
#endif
				}
			}

			// Check for MDE input
			while(serialDataAvail(mdeDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(mdeDevice.uart_fd);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from MDE channel\n");
					return rc;
#endif
				}
			}


			/***** Parse all input data *****/

			rc = crp_parseData(&crpDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}

			rc = comms_parseData(&commsDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}

			rc = mde_parseData(&mdeDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}

			currentLoopTime = missionCurrent;
			currentLoopTime.tv_sec += cycleCount;

			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			es_timeDifference(&missionCurrentTime, &currentLoopTime, &currentLoopDuration);

		} while(currentLoopDuration.tv_sec < 1);

	} // while(!abortTest)

	// End a successful test
	return 0;

} // Function main()


