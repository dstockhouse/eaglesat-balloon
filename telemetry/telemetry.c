/*****************************************************************************
 *
 * File:
 * 	telemetry.c
 *
 * Description:
 * 	Functions related to interfacing to the telemetry on the balloon.
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function telemetry_pressureRead
 *		and telemetry_tempRead
 *
 ****************************************************************************/

#include "telemetry.h"

#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <mcp3004.h>


int telemetry_init(){

	wiringPiSetup();
	mcp3004Setup(ADC_CHAN, SPI_CHAN);

#ifdef ES_DEBUG_MODE
	printf("Starting ADC test:\n\n");
#endif
}


/**** Function telemetry_pressureRead ****
 *
 */
int telemetry_allRead(TELEMETRY_DATA* telemetry) {

	int i;
	int temp[TELEMETRY_NUM_TEMP_SENSORS];
	int pressureValue; 
	float pressurePercent, pressurePSI, pressure_mBar;
	float tempActual[TELEMETRY_NUM_TEMP_SENSORS];
	float resist[TELEMETRY_NUM_TEMP_SENSORS];

	// Read pressure on the next channel (i is one higher than temperature sensors)
	pressureValue = analogRead(ADC_CHAN | 7);
	// pressureValue = 512;

	// According to the pressure sensor datasheet, the pressure
	// analog output ranges from 10% to 90% the analog range, so the
	// analog input needs to be adjusted to reflect the actual
	// pressure
	pressurePercent = pressureValue * 100.0 / 1023;

	// Mapping analog range to pressure range
	// 10% -> 0% (0 PSI)
	// 90% -> 100% (15 PSI, or MAX_PRESSURE_RANGE_PSI)
	// Subtract 10, divide by 80%, then multiply by max PSI range
	pressurePSI = (pressurePercent - 10) / 80 * MAX_PRESSURE_RANGE_PSI;

	// Get pressure in mBar, easier conversion
	pressure_mBar = pressurePSI * 68.9476;

	// Print pressure as ADC value, percentage, and PSI
#ifdef ES_DEBUG_MODE
	printf("\tPressure: %d, %.2f PSI, %.2f mBar (%.2f%% ADC range)\n",
			pressureValue, pressurePSI, pressure_mBar, pressurePercent);
#endif

	telemetry->pressure = pressure_mBar;


	// steinhardt calibration values
	float stein[TELEMETRY_NUM_TEMP_SENSORS][3] = {
		{0.00241409184975829,0.000224111843917994,4.97260868024085e-07},
		{0.0023879237299818,0.00026438625004672,-7.42051520546572e-08},
		{0.00243606594938504,.000217493053844692,5.38164336988555e-07},
		{0.00253711187789608,0.0001973097888103,6.2020032809658e-07},
		{0.00234436652096637,0.000287470479136726,-3.4923616722838e-07},
		{0.00241409184975829,0.000224111843917994,4.97260868024085e-07},
		{0.00246753814301977,0.000207579597802424,6.30014785922159e-07}};


	printf("Starting read cycle\n");

	// Read all temperature sensors connected on first channels
	for(i = 0; i < TELEMETRY_NUM_TEMP_SENSORS; i++) {

		//resistance
		temp[i] = analogRead(ADC_CHAN | i);
		// temp[i] = 500; //test value for thermistors
#ifdef ES_DEBUG_MODE
		printf("\tTemp %d: %d\n", i, temp[i]);
#endif

		resist[i] = (((float)temp[i])/ADC_MAX*10.0)/(1-((float)temp[i])/ADC_MAX);

		//calibration using steinhardt equation
		tempActual[i] = 1/((double) stein[i][0] + (double) stein[i][1]*log((double) resist[i]) + 
				(double) stein[i][2]*pow(log((double) resist[i]),3));

		//moving actual temperature values to the structure
		telemetry->temperature[i] = tempActual[i] - 273.15;

#ifdef ES_DEBUG_MODE
		printf("\tAcutal Temp %d: %f\n", i, telemetry->temperature[i]);
#endif
	}



	return 0;

} // Function telemetry_pressureRead




