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
 * Test for the MDE software.
 */
int main() {
// Initialize UART to MDE board
        mdeDevice->uart_fd = mde_init();
        if (mdeDevice->uart_fd) {
                // Initialization failed
#ifdef  ES_DEBUG_MODE
                printf("Couldn't initialize MDE UART.\n");
                return mdeDevice->uart_fd;
#endif          
        }
 	
 	     
		// Check for MDE input
		while(serialDataAvail(mdeDevice->uart_fd)) {
			// Read serial input
			rc = es_uartGetChar(mdeDevice->uart_fd);
			if(rc) {
#ifdef	ES_DEBUG_MODE
				printf("Failed to read from MDE channel\n");
				return rc;
#endif
			}
		}
             
		rc = mde_parseData(&mdeDevice);
		if(rc) {
#ifdef	ES_DEBUG_MODE
			printf("Failed\n");
			return rc;
#endif
		}
              
 	     return 0;
              
		/***** Services to execute every 15 minutes *****/
		if (cycleCounter % (15 * 60) == 0 && cycleCount > 0) {

			// Get MDE health data
			mde_requestHealthPacket(mdeDevice->uart_fd);

		} // 15 minute cycle counter

		// Increment cycle counter for task scheduling
		cycleCount++;

	} // while(!abortTest)

	// End a successful test
	return 0;

 } // Function main()
              
            
