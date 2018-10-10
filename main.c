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

#include "es_control.h"
#include "comms/comms.h"
#include "crp/crp.h"
#include "crp/crp_generateFilename.h"
#include "mde/mde.h"
#include "telemetry/telemetry.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <wiringSerial.h>

// For testing program in debug mode
// Defined in es_control.h
// #define	ES_DEBUG_MODE


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
	UART_DEVICE crpDevice, commsDevice, mdeDevice;

	// Telemetry object
	TELEMETRY_DATA telemetry;

	// Time variables
	struct timespec missionCurrentTime, missionStartTime, missionElapsedTime, currentLoopDuration, currentLoopTime;

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

#ifndef	ES_DEBUG_NO_COMMS
	// Initialize UART to COMMS transceiver
	comms_init(&commsDevice);
	if (commsDevice.uart_fd < 0) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize COMMS UART.\n");
		return commsDevice.uart_fd;
#endif 	// ES_DEBUG_MODE
	}
#endif	// ES_DEBUG_NO_COMMS

#ifndef	ES_DEBUG_NO_MDE
	// Initialize UART to MDE board
	mdeDevice.uart_fd = mde_init();
	if (mdeDevice.uart_fd < 0) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("Couldn't initialize MDE UART.\n");
		return mdeDevice.uart_fd;
#endif
	}
#endif	// ES_DEBUG_NO_MDE

	/***** Initialize log filenames *****/

	es_generateLogFilename(crpDevice.logFilename, LOG_FILENAME_LENGTH, "CRP");
	es_generateLogFilename(commsDevice.logFilename, LOG_FILENAME_LENGTH, "COMMS");
	es_generateLogFilename(mdeDevice.logFilename, LOG_FILENAME_LENGTH, "MDE");

#ifdef ES_DEBUG_MODE
	printf("Log filenames:\t\t%s\n\t\t\t%s\n\t\t\t%s\n\n", 
			crpDevice.logFilename,
			commsDevice.logFilename,
			mdeDevice.logFilename);
#endif

	// Initialize SPI to ADC for telemetry sensors
	rc = telemetry_init();
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
		// #ifdef	ES_DEBUG_MODE
		// 		// If debugging use 3 seconds instead
		// 		if (cycleCount % 3 == 0 && cycleCount > 0) {
		// #else
		if (cycleCount % 30 == 0 && cycleCount > 0) {
			// #endif	// ES_DEBUG_MODE
			int packetSize;
			char commsPacket[MAX_COMMS_PACKET_SIZE];

			// Get current time
			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			// Get elasped time since start of mission
			es_timeDifference(&missionCurrentTime, &missionStartTime, &missionElapsedTime);

			// Generate packet to send to comms
			packetSize = es_generateCommsPacket(commsPacket, MAX_COMMS_PACKET_SIZE,
					&missionElapsedTime,
					&telemetry,
					&(crpDevice.metadata),
					&(mdeDevice.metadata));
			comms_sendPacket(commsDevice.uart_fd, commsPacket, packetSize);
			if (rc) {
#ifdef	ES_DEBUG_MODE
				printf("Couldn't send COMMS packet.\n");
				return rc;
#endif
			}

			// 30 second cycle counter
		} else {
			// If no data sent, send heartbeat
			comms_sendPacket(commsDevice.uart_fd, ".\r", 2);
		}

		/***** Services to execute every 15 minutes *****/
		if (cycleCount % (15 * 60) == 0 && cycleCount > 0) {

#ifdef	ES_DEBUG_NO_MDE
			// Get MDE health data
			mde_requestHealthPacket(mdeDevice.uart_fd);
#endif	// ES_DEBUG_NO_MDE

		} // 15 minute cycle counter

		// Increment cycle counter for task scheduling
		cycleCount++;

		do {

			/***** Delay or poll UART channels *****/

#ifdef	ES_DEBUG_NO_CRP
			// Check for crp input
			while(serialDataAvail(crpDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(&crpDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from CRP channel\n");
					return rc;
#endif	// ES_DEBUG_MODE
				}
			}
#endif	// ES_DEBUG_NO_CRP


#ifndef	ES_DEBUG_NO_COMMS
			// Check for comms input
			while(serialDataAvail(commsDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(&commsDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from COMMS channel\n");
					return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_COMMS


#ifndef	ES_DEBUG_NO_MDE
			// Check for MDE input
			while(serialDataAvail(mdeDevice.uart_fd)) {
				// Read serial input
				rc = es_uartGetChar(&mdeDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("Failed to read from MDE channel\n");
					return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_MDE


			/***** Parse all input data *****/

#ifndef	ES_DEBUG_NO_CRP
			// rc = crp_parseData(&crpDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}
#endif	// ES_DEBUG_NO_CRP


#ifndef	ES_DEBUG_NO_COMMS
			rc = comms_parseData(&commsDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}
#endif	// ES_DEBUG_NO_COMMS


#ifndef	ES_DEBUG_NO_MDE
			rc = mde_parseData(&mdeDevice);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed\n");
				return rc;
#endif
			}
#endif	// ES_DEBUG_NO_CRP

			currentLoopTime = missionCurrentTime;
			currentLoopTime.tv_sec += cycleCount;

			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			es_timeDifference(&missionCurrentTime, &currentLoopTime, &currentLoopDuration);

		} while(currentLoopDuration.tv_sec < 1);

	} // while(!abortTest)

	// End a successful test
	return 0;

} // Function main()

