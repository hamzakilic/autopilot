/*
 ============================================================================
 Name        : autopilot.c
 Author      : Hamza Kılıç
 Version     :
 Copyright   : Feel free to use
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <em_io.h>
#include "manager/atp_pilot.h"
#include "log/atp_log.h"

#ifndef COMPILE_TEST_CODES

int main(void) {


	union {
	    int i;
	    char c[sizeof(int)];
	} x;
	x.i = 1;
	if(x.c[0] == 1)
	    printf("little-endian\n");
	else    printf("big-endian\n");


    em_uint32 err;
	if((err=em_io_initialize(0))){
		atp_log(atp_log_create_string(ATP_LOG_FATAL,"Hardware Initialize Failed Error:%u\n",err));
		return ATP_ERROR_HARDWARE_INITIALIZE;
	}

	if((err=em_io_i2c_start(EM_USE_BSC1,0,0)))
		        {
		        	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting I2C failed Errno:%u\n",err));
		        	return ATP_ERROR_HARDWARE_I2C_START;
		        }


    atp_pilot *pilot;
	if(atp_pilot_create(&pilot)){
       return EXIT_FAILURE;
	}
	if(atp_pilot_start(pilot))
	{
		return EXIT_FAILURE;
	}


   getchar();
   printf("print any char to exit");
   getchar();
   atp_pilot_stop(pilot);
   atp_pilot_destroy(pilot);

	return EXIT_SUCCESS;
}
#endif
