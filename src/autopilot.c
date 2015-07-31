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

int main(void) {



    em_uint32 err;
	if((err=em_io_initialize(0))){
		atp_log(atp_log_create_string(ATP_LOG_FATAL,"Hardware Initialize Failed Error:%u\n",err));
		return ATP_ERROR_HARDWARE_INITIALIZE;
	}



	if(atp_pilot_create()){
       return EXIT_FAILURE;
	}
   getchar();

	return EXIT_SUCCESS;
}
