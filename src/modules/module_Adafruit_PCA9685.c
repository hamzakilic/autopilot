/*
 * module_Adafruit_PCA9685.c
 *
 *  Created on: Jan 16, 2016
 *      Author: root
 */


#include "module_Adafruit_PCA9685.h"
static em_uint8 slave=0x40;
static em_uint32 pca9685_i2c_write(em_uint8 *data,em_uint32 length)
{
	em_uint32 err;
	em_uint8 index=0;
	atp_system_lock_i2c();
	err =em_io_i2c_write(EM_USE_BSC1,slave,data,length,EM_TIMEOUT_ONE_SECOND);
	atp_system_unlock_i2c();
	return err;

}

static em_uint32 pca9685_i2c_read(em_uint8 *data,em_uint32 length)
{
	em_uint32 err;
	em_uint8 index=0;
	atp_system_lock_i2c();
	err=em_io_i2c_read(EM_USE_BSC1,slave,data,length,EM_TIMEOUT_ONE_SECOND);
	atp_system_unlock_i2c();
	return err;
}

static em_uint32 init_pca9685(){
	em_uint32 err;
	 em_uint32 frequency_scale= (int)(25000000.0f /(4096*200)-1.0f);


	        em_uint8 oldmod=0;
	 		em_uint32 length=1;
	 		em_uint8 data[]={49};//sleep mode

	        err=pca9685_i2c_write(data,length);
	        if(err){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	     	err=pca9685_i2c_read(&oldmod,length);
	        if(err){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }



	        em_uint8 data2[2]={0,0};

	        data2[0]=PCA9685_PRESCALE;
	        data2[1]=frequency_scale;
	        err=pca9685_i2c_write(data2,2);
	        if(err){
	          return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	        em_io_delay_loops(5000);
	        data2[0]=PCA9685_MODE1;
	        data2[1]=0xa1;
	        err=pca9685_i2c_write(data2,2);
	        if(err){
	         return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }

	        em_io_delay_loops(5000);
	        err=pca9685_i2c_read(&oldmod,length);
	        	        if(err){
	        	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        	        }

	        return ATP_SUCCESS;
}






em_uint32 adafruit_pca9685_start(void *parameter){
	 return init_pca9685();

}
em_uint32 adafruit_pca9685_set(em_uint16 value,em_uint8 pin_number)
{
    #define LED0_ON_L 0x6
	em_uint32 err;
	em_uint8 index=0;
	em_uint8 slave=0x40;
	em_uint8 datapwm[5];
  	em_uint32 on=0;
   	em_uint32 off=value;
   	datapwm[0]=LED0_ON_L+pin_number*4;
   	datapwm[1]=on;
  	datapwm[2]=(on>>8);
   	datapwm[3]=off ;
   	datapwm[4]=(off >>8);


   	atp_system_lock_i2c();
		err=em_io_i2c_write(EM_USE_BSC1,slave,datapwm,5,EM_TIMEOUT_ONE_SECOND);
		atp_system_unlock_i2c();

		return err;

}



em_uint32 adafruit_pca9685_stop(void *parameter){
	return ATP_SUCCESS;

}

