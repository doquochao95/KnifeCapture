#ifndef motor_serial0_h
#define motor_serial0_h

#define Motorserial Serial1 

static void motorserial_init ()
{
	Motorserial.begin(9600);      // Define and start Sender serial port
  printf("Initialize motor module");
}
static void motorserial_sendcommand (char *commands)
{
	Motorserial.write(commands);
	printf("motor cmd: %s\r\n", commands);
}

#endif
