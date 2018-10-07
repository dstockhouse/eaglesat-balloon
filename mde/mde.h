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
 *	mde_init
 *
 ****************************************************************************/

#ifndef EAGLESAT_MDE_H
#define EAGLESAT_MDE_H

#include <stdio.h>
#include <stdlib.h>

#define MDE_SERIAL_DEVICE	"/dev/serial1"

int mde_init(void);

int mde_requestPacket(char *buffer, int bufSize);

#endif // EAGLESAT_MDE_H

