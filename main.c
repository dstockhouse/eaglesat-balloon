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

#include <pthread.h>

#include <wiringSerial.h>

// For testing program in debug mode
// Defined in es_control.h
// #define	ES_DEBUG_MODE

// Function for the CRP thread
void *crp_cameraReadPythonThread(void *params);

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

	int crpThreadDispatched = 0;

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
	// 		printf("\tCouldn't initialize CRP sensor.\n");
	// 		return crpDevice.uart_fd;
	// #endif
	// 	}

	crpDevice.metadata.crp_started = 0;

#ifndef	ES_DEBUG_NO_COMMS
	// Initialize UART to COMMS transceiver
	comms_init(&commsDevice);
	if (commsDevice.uart_fd < 0) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("\tCouldn't initialize COMMS UART.\n");
		return commsDevice.uart_fd;
#endif 	// ES_DEBUG_MODE
	}
#endif	// ES_DEBUG_NO_COMMS

#ifndef	ES_DEBUG_NO_MDE
	// Initialize UART to MDE board
	mde_init(&mdeDevice);
	if (mdeDevice.uart_fd < 0) {
		// Initialization failed
#ifdef	ES_DEBUG_MODE
		printf("\tCouldn't initialize MDE UART.\n");
		return mdeDevice.uart_fd;
#endif
	}
#endif	// ES_DEBUG_NO_MDE

	/***** Initialize log filenames *****/

	es_generateLogFilename(crpDevice.logFilename, LOG_FILENAME_LENGTH, "CRP");
	es_generateLogFilename(commsDevice.logFilename, LOG_FILENAME_LENGTH, "COMMS");
	es_generateLogFilename(mdeDevice.logFilename, LOG_FILENAME_LENGTH, "MDE");

#ifdef ES_DEBUG_MODE
	printf("\tLog filenames:\t%s\n\t\t\t%s\n\t\t\t%s\n\n", 
			crpDevice.logFilename,
			commsDevice.logFilename,
			mdeDevice.logFilename);
#endif

	// Initialize SPI to ADC for telemetry sensors
//	rc = telemetry_init();
//	if (rc) {
//		// Initialization failed
//#ifdef	ES_DEBUG_MODE
//		printf("\tCouldn't initialize MCP3008 SPI.\n");
//		return rc;
//#endif
//	}


	/***** Main program loop *****/

#ifdef	ES_DEBUG_BREAK_LOOP
	// Break out of loop early
	while(!abortTest && cycleCount < 45) {
#else
	while(!abortTest) {
#endif

#ifdef	ES_DEBUG_MODE
		printf("\tStarting loop %d\n", cycleCount);
#endif

		/***** Services to execute each 1-second cycle *****/

		// 		// Read from CRP sensor
		// 		rc = crp_sensorRead(imageDataBuf, CRP_IMAGE_BUF_SIZE);
		// 		if (rc < CRP_IMAGE_SIZE) {
		// 
		// 			// Not entire image was read
		// #ifdef	ES_DEBUG_MODE
		// 			printf("\tOnly read %d bytes.\n", rc);
		// #endif
		// 
		// 		}
		// 
		// 		// Store image that was read
		// 		rc = crp_imageStore(imageDataBuf, rc);
		// 		if (rc) {
		// #ifdef	ES_DEBUG_MODE
		// 			printf("\tDidn't successfully store image data.\n");
		// 			return rc;
		// #endif
		// 		}

		// Read CRP sensor using python script


		// Read telemetry data
//		rc = telemetry_allRead(&telemetry);
//		if(rc) {
//#ifdef	ES_DEBUG_MODE
//			printf("\tFailed\n");
//			return rc;
//#endif
//		}

		// 		// Read telemetry sensors
		// 		rc = telemetry_tempRead(&pressure);
		// 		if (rc) {
		// #ifdef	ES_DEBUG_MODE
		// 			printf("\tDidn't successfully read temp sensors.\n");
		// 			return rc;
		// #endif
		// 		}
		// 
		// 		rc = telemetry_tempRead(temperature, TELEMETRY_NUM_TEMP_SENSORS);
		// 		if (rc) {
		// #ifdef	ES_DEBUG_MODE
		// 			printf("\tDidn't successfully read temp sensors.\n");
		// 			return rc;
		// #endif
		// 		}


		/***** Services to execute every 30 seconds *****/
		// #ifdef	ES_DEBUG_MODE
		// 		// If debugging use 3 seconds instead
		// 		if (cycleCount % 3 == 0 && cycleCount > 0) {
		// #else
#ifdef	ES_DEBUG_MODE
		if (cycleCount % 5 == 0 && cycleCount > 0) {
#else
		if (cycleCount % 60 == 0 && cycleCount > 0) {
#endif	// ES_DEBUG_MODE
			// #endif	// ES_DEBUG_MODE
			int packetSize;
			char commsPacket[MAX_COMMS_PACKET_SIZE];

			// Get current time
			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			// Get elasped time since start of mission
			es_timeDifference(&missionCurrentTime, &missionStartTime, &missionElapsedTime);

#ifdef	ES_DEBUG_MODE
			// printf("Address of MDE base and metadata: %p %p\n",
					// &mdeDevice, &(mdeDevice.metadata));
#endif	// ES_DEBUG_MODE

			// Generate packet to send to comms
			packetSize = es_generateCommsPacket(commsPacket, MAX_COMMS_PACKET_SIZE,
					&missionElapsedTime,
					NULL,
#ifndef	ES_DEBUG_NO_CRP
					&(crpDevice.metadata),
#else
					NULL,
#endif
#ifndef	ES_DEBUG_NO_MDE
					&(mdeDevice.metadata));
#else
					NULL);
#endif
			rc = comms_sendPacket(commsDevice.uart_fd, commsPacket, packetSize);
			if (rc) {
#ifdef	ES_DEBUG_MODE
				printf("\tCouldn't send COMMS packet.\n");
				// return rc;
#endif
			}

			// 30 second cycle counter
		} else {
			// If no data sent, send heartbeat
			comms_sendPacket(commsDevice.uart_fd, ".\r", 2);
		}

		/***** Services to execute every 15 minutes *****/
#ifdef	ES_DEBUG_MODE
		if (cycleCount % 4 == 0 && cycleCount > 0) {
#else
		if (cycleCount % (15 * 60) == 0 && cycleCount > 0) {
#endif	// ES_DEBUG_MODE

#ifndef	ES_DEBUG_NO_MDE
			// Get MDE health data
			mde_requestHealthPacket(mdeDevice.uart_fd);
#endif	// ES_DEBUG_NO_MDE

		} // 15 minute cycle counter

		// Increment cycle counter for task scheduling
		cycleCount++;

		int pollCount = 0;
		do {
			int temp;

			/***** Delay or poll UART channels *****/

// #ifdef	ES_DEBUG_NO_CRP
// 			// Check for crp input
// 			while(serialDataAvail(crpDevice.uart_fd) > 0) {
// 				// Read serial input
// 				rc = es_uartGetChar(&crpDevice);
// 				if(rc) {
// #ifdef	ES_DEBUG_MODE
// 					printf("\tFailed to read from CRP channel\n");
// 					return rc;
// #endif	// ES_DEBUG_MODE
// 				}
// 			}
// #endif	// ES_DEBUG_NO_CRP


#ifndef	ES_DEBUG_NO_COMMS
			// Check for comms input
			while(temp = serialDataAvail(commsDevice.uart_fd) > 0) {
				// Read serial input
				printf("%d COMMS bytes available\n", temp);
				rc = es_uartGetChar(&commsDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("\tFailed to read from COMMS channel\n");
					// return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_COMMS


#ifndef	ES_DEBUG_NO_MDE
			// Check for MDE input
			while(serialDataAvail(mdeDevice.uart_fd) > 0) {
#ifdef	ES_DEBUG_MODE
				// printf("\tChars available for MDE\n");
#endif
				// Read serial input
				rc = es_uartGetChar(&mdeDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("\tFailed to read from MDE channel\n");
					// return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_MDE


			/***** Parse all input data *****/

// #ifndef	ES_DEBUG_NO_CRP
// 			if(crpDevice.inputBufferSize > 0) {
// #ifdef	ES_DEBUG_MODE
// 				printf("%d bytes in crp buffer\n", crpDevice.inputBufferSize);
// #endif
// 				rc = crp_parseData(&crpDevice);
// 				if(rc) {
// #ifdef	ES_DEBUG_MODE
// 					printf("\tFailed\n");
// 					return rc;
// #endif
// 				}
// 			}
// #endif	// ES_DEBUG_NO_CRP


#ifndef	ES_DEBUG_NO_COMMS
			if(commsDevice.inputBufferSize > 0) {
#ifdef	ES_DEBUG_MODE
				printf("%d bytes in comms buffer\n", commsDevice.inputBufferSize);
#endif
				rc = comms_parseData(&commsDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("\tFailed\n");
					// return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_COMMS


#ifndef	ES_DEBUG_NO_MDE
			if(mdeDevice.inputBufferSize > 0) {
				// printf("About to parse: first element is %x\n", mdeDevice.inputBuffer[0]);
				// printf("About to parse: array pointer is %p\n", mdeDevice.inputBuffer);
				rc = mde_parseData(&mdeDevice);
				if(rc) {
#ifdef	ES_DEBUG_MODE
					printf("\tFailed\n");
					// return rc;
#endif
				}
			}
#endif	// ES_DEBUG_NO_MDE

			currentLoopTime = missionStartTime;
			currentLoopTime.tv_sec += cycleCount;

			clock_gettime(CLOCK_REALTIME, &missionCurrentTime);

			es_timeDifference(&missionCurrentTime, &currentLoopTime, &currentLoopDuration);

			pollCount++;
#ifdef	ES_DEBUG_MODE
			// printf("\tWaited %d time(s)\n", pollCount);
#endif	// ES_DEBUG_MODE

		} while(currentLoopDuration.tv_sec < 1);


		// If 30 minutes into flight, start CRP
#ifdef	ES_DEBUG_MODE
		if(missionElapsedTime.tv_sec > (30) && !crpThreadDispatched) {
#else
		if(missionElapsedTime.tv_sec > (30 * 60) && !crpThreadDispatched) {
#endif
			pthread_t crpThread;
			pthread_attr_t crpAttr;
			int rc;

#ifdef	ES_DEBUG_MODE
			printf("*******************Starting CRP thread*******************\n");
#endif

			// Initialize attributes. Not really necessary but hey
			rc = pthread_attr_init(&crpAttr);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed to init CRP thread attributes\n");
				// return rc;
#endif
			}

			// Start python thread execution
			rc = pthread_create(&crpThread, &crpAttr, crp_cameraReadPythonThread, NULL);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed to create CRP thread\n");
				// return rc;
#endif
			}

			crpThreadDispatched = 1;
			crpDevice.metadata.crp_started = 1;

			// Bye fam
			rc = pthread_detach(crpThread);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed to detach CRP thread\n");
				// return rc;
#endif
			}
		}
		
	} // while(!abortTest)

	// End a successful test
	return 0;

} // Function main()


void *crp_cameraReadPythonThread(void *params) {

	int rc;

	while(1) {

		rc = system("python /home/pi/balloon/crp/picamera/crp_imageCapture.py");
		if(rc) {
#ifdef	ES_DEBUG_MODE
			printf("Failed to start python script\n");
			// return rc;
#endif
		}
	}

	pthread_exit(NULL);

}

