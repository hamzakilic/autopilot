/*
 * atp_motor_controller.c
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#include "atp_motor_controller.h"

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

em_uint8 slave=0x40;

em_uint32 i2c_write(em_uint8 *data,em_uint32 length)
{
	em_uint32 err;
	em_uint8 index=0;
	//try 100 times then return error
	//
	for(index=0;index<100;++index){
		err=em_io_i2c_write(EM_USE_BSC1,slave,data,length);
		        	if(!err)
		        		break;
		        	else{
		        		atp_log(atp_log_create_string(ATP_LOG_FATAL,"I2C communication failed Errno:\n",err));
		        	}
		        	err=0;
	}

	if(index==100){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	return ATP_SUCCESS;
}

em_uint32 i2c_read(em_uint8 *data,em_uint32 *length)
{
	em_uint32 err;
	em_uint8 index=0;
	for(index=0;index<100;++index){
		err=em_io_i2c_read(EM_USE_BSC1,slave,data,length);
		        	if(!err)
		        		break;
		        	else{
		        		atp_log(atp_log_create_string(ATP_LOG_FATAL,"I2C communication failed Errno:\n",err));
		        	}
		        	err=0;
	}

	if(index==100){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	return ATP_SUCCESS;
}
static em_uint32 init_pca9685(){
	em_uint32 err;
	 em_uint32 frequency_scale= (int)(25000000.0f /(4096*200)-1.0f);


	        em_uint8 oldmod=0;
	        em_uint32 length=1;
	        em_uint8 data[]={PCA9685_MODE1};
	        em_int8  index=0;


	        err=i2c_write(data,length);
	        if(err){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	     	err=i2c_read(&oldmod,&length);
	        if(err){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }


	        em_uint8 newMode=(oldmod &0x7F) |0x10;//sleep
	        em_uint8 data2[2]={PCA9685_MODE1,newMode};


	        err=i2c_write(data2,2);
	        if(err){
	        return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }

	        data2[0]=PCA9685_PRESCALE;
	        data2[1]=frequency_scale;
	        err=i2c_write(data2,2);
	        if(err){
	          return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }

	        data2[0]=PCA9685_MODE1;
	        data2[1]=oldmod;
	        err=i2c_write(data2,2);
	        if(err){
	         return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }

	        em_io_delay_loops(5000);
	        data2[0]=PCA9685_MODE1;
	        data2[1]=oldmod|0xa1;
	        err=i2c_write(data2,2);
	        if(err){
	         return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }

	        return ATP_SUCCESS;
}

#define for_each_motors()  for(index=0;index<ATP_MOTORS_COUNT;++index)

static struct atp_motor ** motors=NULL;
em_uint32  atp_motor_controller_start(atp_input *input){

	em_uint32 err;

	        if((err=em_io_i2c_start(EM_USE_BSC1,0,0)))
	        {
	        	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting I2C failed Errno:%u\n",err));
	        	return ATP_ERROR_HARDWARE_I2C_START;
	        }
	        err=init_pca9685();
	        if(err){
	        	return err;
	        }
        motors=atp_malloc(ATP_MOTORS_COUNT*sizeof(struct atp_motor*));

       err=atp_motor_create(&motors[ATP_MOTOR_FRONT_RIGHT],ATP_MOTOR_FRONT_RIGHT,0);
       if(err){
         atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_FRONT_RIGHT,err));
         return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }

       err=atp_motor_create(&motors[ATP_MOTOR_BACK_RIGHT],ATP_MOTOR_BACK_RIGHT,1);
       if(err){
        atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_BACK_RIGHT,err));
        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }
       err=atp_motor_create(&motors[ATP_MOTOR_BACK_LEFT],ATP_MOTOR_BACK_LEFT,2);
       if(err){
         atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_BACK_LEFT,err));
        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }

       err=atp_motor_create(&motors[ATP_MOTOR_FRONT_LEFT],ATP_MOTOR_FRONT_LEFT,3);
       if(err){
       atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_FRONT_LEFT,err));
        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
      }



        em_uint8 index;
        for_each_motors(){
           err=atp_motor_start(motors[index]);
           if(err){
        	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting Motor %u  failed Errno:%u\n",index,err));
        	   return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
           }
        }

        return ATP_SUCCESS;

}
em_uint32 atp_motor_controller_stop(){



	if(motors!=NULL){
		em_uint8 index;
		for_each_motors(){
			atp_motor_stop(motors[index]);
		}
		for_each_motors(){
					atp_motor_destroy(motors[index]);
				}
       atp_free(motors);
	}
	return ATP_SUCCESS;
}

