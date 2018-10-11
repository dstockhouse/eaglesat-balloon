/*****************************************************************************
 *
 * File:
 * 	telemetry_test.c
 *
 * Description:
 * 	Demonstration of reading temperature and pressure sensors connected to
 * 	the MCP3008 ADC using SPI and wiringPi
 *
 * Revision 1.0
 * 	Date: 10/6/2018
 *	Author: David Stockhouse
 *	Changes: Created file from adc.c
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <mcp3004.h>

#define ADC_CHAN 100
#define SPI_CHAN 0
#define NUM_TEMP_SENSORS	7

// Maximum range of the pressure sensor, in PSI
#define MAX_PRESSURE_RANGE_PSI	15

int main()
{
	int temperature[NUM_TEMP_SENSORS];
	int pressureValue; 
	float pressurePercent, pressurePSI, pressure_mBar;
	int i;

	// Initialize wiringPi SPI driver
	wiringPiSetup();
	mcp3004Setup(ADC_CHAN, SPI_CHAN);

	printf("Starting ADC test:\n\n");

	// Infinite loop. Use ctrl-c to exit
	while (1)
	{
		printf("Starting read cycle\n");

		// Read all temperature sensors connected on first channels
		for(i = 0; i < NUM_TEMP_SENSORS; i++) {

			temperature[i] = analogRead(ADC_CHAN | i);
			printf("\tTemp %d: %d\n", i, temperature[i]);

		}

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
		printf("\tPressure: %d, %.2f PSI, %.2f mBar (%.2f%% ADC range)\n",
				pressureValue, pressurePSI, pressure_mBar, pressurePercent);

		// Wait 200 ms (5 Hz)
		usleep(200000);
	}

	// Will never reach here
	return 0;
}

