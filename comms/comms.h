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
 * Revision 1.1
 * 	Date: 10/9/18
 *	Author: David Stockhouse
 *	Changes: Added comms_init function and serial interface commands
 *
 ****************************************************************************/

#ifndef EAGLESAT_COMMS_H
#define EAGLESAT_COMMS_H

#include "../es_control.h"

#include <stdio.h>
#include <stdlib.h>

#define COMMS_SERIAL_DEVICE	"/dev/ttySOFT0"

#define COMMS_COMMAND_START	"K\r"
#define COMMS_COMMAND_START_LEN	2
#define COMMS_COMMAND_HEARTBEAT	".\r"
#define COMMS_COMMAND_HEARTBEAT_LEN	2

int comms_init(void);

int comms_sendPacket(int fd, char *buffer, int bufSize);

#endif // EAGLESAT_COMMS_H

