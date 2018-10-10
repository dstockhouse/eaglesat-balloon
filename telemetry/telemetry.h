/*****************************************************************************
 *
 * File:
 * 	telemetry.h
 *
 * Description:
 * 	Header file for telemetry.c which contains preprocessor definitions and 
 * 	function headers
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function telemetry_pressureRead
 *		and telemetry_tempRead
 *
 ****************************************************************************/

#ifndef EAGLESAT_TELEMETRY_H
#define EAGLESAT_TELEMETRY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include <mcp3004.h>

#define TELEMETRY_NUM_TEMP_SENSORS	7

// Indices for each temp sensor
#define TELEMETRY_TEMP_SENSOR_EPS1	0
#define TELEMETRY_TEMP_SENSOR_EPS2	1
#define TELEMETRY_TEMP_SENSOR_EPS3	2
#define TELEMETRY_TEMP_SENSOR_OBC	3
#define TELEMETRY_TEMP_SENSOR_CRP	4
#define TELEMETRY_TEMP_SENSOR_MDE	5
#define TELEMETRY_TEMP_SENSOR_COMMS	6

// Indices for each pressure sensor
#define ADC_CHAN 100
#define SPI_CHAN 0

// Max pressure for pressure sensor
#define MAX_PRESSURE_RANGE_PSI	15

// Debug mode
#define ES_DEBUG_MODE

// ADC max voltage
#define ADC_MAX 1023

// Structure that contains all telemetry of the system
typedef struct {
	float temperature[TELEMETRY_NUM_TEMP_SENSORS];
	float pressure;
} TELEMETRY_DATA;

int telemetry_allRead(TELEMETRY_DATA *);

int telemetry_init();

#endif // EAGLESAT_TELEMETRY_H

