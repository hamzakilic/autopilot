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


union {
    int i;
    char c[sizeof(int)];
} endian;



int test(){
#define TRIGER EM_GPIO_16
#define ECHO EM_GPIO_15
	em_io_gpio_mode(EM_GPIO_16,EM_MODE_GPIO_OUT);
	em_io_gpio_mode(EM_GPIO_15,EM_MODE_GPIO_IN);
	em_io_gpio_pull(EM_GPIO_15,EM_PULL_DOWN);
 /*   while(1){
    	printf("highing\n");
    	//em_io_gpio_mode(TRIGER,EM_MODE_GPIO_FUNC2);
    em_io_gpio_write(EM_GPIO_16,EM_GPIO_HIGH);
    int counter=10;
    em_uint8 val=0;
    while(counter--){
	em_io_delay_microseconds(100000*2);

    em_io_gpio_read(EM_GPIO_15,&val);
    if(val & EM_GPIO_HIGH){
    	printf("high\n");
    }else printf("low\n");
    }
    printf("lowing\n");
	em_io_gpio_write(EM_GPIO_16,EM_GPIO_LOW);
	counter=10;
	while(counter--){
	em_io_delay_microseconds(100000*2);
	 em_io_gpio_read(EM_GPIO_15,&val);
	    if(val & EM_GPIO_HIGH){
	    	printf("high\n");
	    }else printf("low\n");
	}
    }*/


	em_io_gpio_write(TRIGER,EM_GPIO_LOW);
	em_io_busy_wait(2);


	while(1){
		em_io_gpio_write(TRIGER,EM_GPIO_HIGH);
		em_io_busy_wait(10);
		em_io_gpio_write(TRIGER,EM_GPIO_LOW);
		em_uint32 diftime;
		if(em_io_gpio_pulsein(ECHO,EM_GPIO_HIGH,50000,&diftime))
			printf("timeoutnot started %d\n");
		else
		{
			em_float32 distance=diftime*10 / 29 / 2;
    		printf("distance is %d %5.2f  mm\n",diftime,distance);
		}
		em_io_delay_microseconds(250000);

	}



}


int main(void) {

	endian.i = 1;
	if(endian.c[0] == 1)
	    printf("cpu is little-endian\n");
	else    printf("cpu big-endian\n");




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

	//test();



    atp_pilot *pilot;
	if(atp_pilot_create(&pilot)){
       return EXIT_FAILURE;
	}
	if(atp_pilot_start(pilot))
	{
		return EXIT_FAILURE;
	}


   getchar();
   printf("print any char to exit\n");
   getchar();
   printf("exiting\n");
   atp_pilot_stop(pilot);
   atp_pilot_destroy(pilot);

	return EXIT_SUCCESS;
}
#endif
