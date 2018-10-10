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
		pressureValue = analogRead(ADC_CHAN | i);

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
    float stein[TELEMETRY_NUM_TEMP_SENSORS][3] = {{.0026,1.8757e-4,7.7293e-7},
    {.0024,2.6750e-4,-6.4439e-8},
    {.0026,1.7505e-4,8.8288e-7},
    {.0025,2.1041e-4,5.7530e-7},
    {.0024,2.7323e-4,-1.7083e-7},
    {.0026,1.7572e-4,8.7425e-7},
    {.0027,1.5948e-4,1.0168e-6}};
    
  
    printf("Starting read cycle\n");
    
		// Read all temperature sensors connected on first channels
		for(i = 0; i < TELEMETRY_NUM_TEMP_SENSORS; i++) {
      
      //resistance
			temp[i] = analogRead(ADC_CHAN | i);
      #ifdef ES_DEBUG_MODE
			printf("\tTemp %d: %d\n", i, temp[i]);
      #endif
      
      resist[i] = (((float)temp[i]) * 10/ADC_MAX)/(1-((float)temp[i])/ADC_MAX);
      
      //calibration using steinhardt equation
      tempActual[i] = 1/((double) stein[i][0] + (double) stein[i][1]*log((double) resist[i]) + 
      (double) stein[i][2]*pow(log((double) resist[i]),3));
      
      //moving actual temperature values to the structure
      telemetry->temperature[i] = tempActual[i] - 273.15;
      }
      
      
      
	return 0;

} // Function telemetry_pressureRead




