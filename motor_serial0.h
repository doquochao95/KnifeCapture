#ifndef motor_serial0_h
#define motor_serial0_h

#include "global_config.h"

#define Motorserial Serial1 

static void motorserial_init ()
{
	function_log();
	Motorserial.begin(9600);      // Define and start Sender serial port
	printf("Initialize motor module");
}
static void motorserial_sendcommand (char *commands)
{
	function_log();
	Motorserial.write(commands);
	printf("motor cmd: %s\r\n", commands);
}

#endif
