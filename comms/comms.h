/*****************************************************************************
 *
 * File:
 * 	comms.h
 *
 * Description:
 * 	Header file for comms.c which contains preprocessor definitions and 
 * 	function headers
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function comms_sendPacket
 *
 ****************************************************************************/

#ifndef EAGLESAT_COMMS_H
#define EAGLESAT_COMMS_H

#include <stdio.h>
#include <stdlib.h>

#define COMMS_SERIAL_DEVICE	"/dev/ttySOFT0"

int comms_sendPacket(char *buffer, int bufSize);

#endif // EAGLESAT_COMMS_H

