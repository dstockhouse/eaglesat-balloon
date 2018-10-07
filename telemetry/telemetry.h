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

#define TELEMETRY_NUM_TEMP_SENSORS	7

int telemetry_pressureRead(void);

int telemetry_tempRead(void);

#endif // EAGLESAT_TELEMETRY_H

