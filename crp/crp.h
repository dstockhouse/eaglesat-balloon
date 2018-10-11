/*****************************************************************************
 *
 * File:
 * 	crp.h
 *
 * Description:
 * 	Header file for crp.c which contains preprocessor definitions and 
 * 	function headers
 *
 * Revision 1.0
 * 	Date: 10/2/18
 *	Author: David Stockhouse
 *	Changes: Template file created with function crp_sensorRead and
 *		crp_imageStore	
 *
 ****************************************************************************/

#ifndef EAGLESAT_CRP_H
#define EAGLESAT_CRP_H

#include <stdio.h>
#include <stdlib.h>

#define CRP_SERIAL_DEVICE	"/dev/serial0"
#define CRP_SERIAL_BAUD		115200

#define CRP_IMAGE_SIZE		(120 * 160)

#define CRP_COMMAND_SYNC_INIT		{0xAA, 0x0D, 0x00}
#define CRP_COMMAND_GETPICTURE_INIT	{0xAA, 0x0D, 0x00}

int crp_sensorRead(int fd, char *buffer, int bufSize);

int crp_imageStore(char *buffer, int bufSize, FILE *fd);

#endif // EAGLESAT_CRP_H

