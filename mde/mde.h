/*****************************************************************************
 *
 * File:
 * 	mde.h
 *
 * Description:
 * 	Header file for mde.c which contains preprocessor definitions and 
 * 	function headers
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function mde_requestPacket and
 *		mde_init
 *
 * Revision 1.1
 * 	Date: 10/9/2018
 * 	Author: David Stockhouse
 * 	Changes: Added UART constants, changed mde_requestPacket to
 * 		mde_requestHealthPacket
 *
 ****************************************************************************/

#ifndef EAGLESAT_MDE_H
#define EAGLESAT_MDE_H

#include "../es_control.h"

#include <stdio.h>
#include <stdlib.h>

// #define MDE_SERIAL_DEVICE	"/dev/ttyUSB0"
// #define MDE_SERIAL_DEVICE	"/dev/ttyACM0"
// #define MDE_SERIAL_DEVICE	"/dev/ttySOFT0"
#define MDE_SERIAL_DEVICE	"/dev/ttyAMA0"
#define MDE_SERIAL_BAUDRATE	9600

#define MDE_PACKET_LENGTH	6
#define MDE_PACKET_HEADER_BYTE	0x80
#define MDE_PACKET_TYPE_BIT	23

#define MDE_COMMAND_HEALTH	0x53

int mde_init(UART_DEVICE *);

int mde_requestHealthPacket(int);

int mde_parseData(UART_DEVICE *);

#endif // EAGLESAT_MDE_H

