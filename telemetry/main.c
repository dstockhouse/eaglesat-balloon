/*****************************************************************************
 *
 * File:
 * 	main.c
 *
 * Description:
 * 	The main function for testing the functionality of the software for 
 * 	interfacing to the telemetry instruments on the balloon.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created.
 *
 ****************************************************************************/

#include "telemetry.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**** Function main ****
 * Test for the telemetry software.
 */
int main() {

	TELEMETRY_DATA telemetry;
	int i;

	telemetry_init();

	for(i = 0; i < 10; i++) {
		telemetry_allRead(&telemetry);
		usleep(1000000);
	}

	return 0;

} // Function main()

