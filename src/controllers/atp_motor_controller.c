/*
 * atp_motor_controller.c
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#include "atp_motor_controller.h"








#define for_each_motors()  for(index=0;index<ATP_MOTORS_COUNT;++index)


em_uint32  atp_motor_controller_create(atp_input *input,atp_motor_controller **motor_controller){

	em_uint32 err;

#ifdef COMPILE_PCA9685
	        err=adafruit_pca9685_start(NULL);
#endif
	        if(err){
	        	return err;
	        }

	   atp_motor ** motors=NULL;
        motors=atp_malloc(ATP_MOTORS_COUNT*sizeof(struct atp_motor*));

       err=atp_motor_create(&motors[ATP_MOTOR_FRONT_RIGHT],input, ATP_MOTOR_FRONT_RIGHT,EM_GPIO_5,4);
       if(err){
         atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_FRONT_RIGHT,err));
         atp_free(motors);

         return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }


       err=atp_motor_create(&motors[ATP_MOTOR_BACK_RIGHT],input,ATP_MOTOR_BACK_RIGHT,EM_GPIO_6,5);
       if(err){
        atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_BACK_RIGHT,err));
        atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
                 atp_free(motors);

        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }
       err=atp_motor_create(&motors[ATP_MOTOR_BACK_LEFT],input, ATP_MOTOR_BACK_LEFT,EM_GPIO_7,6);
       if(err){
         atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_BACK_LEFT,err));
         atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
         atp_free(motors[ATP_MOTOR_BACK_RIGHT]);
         atp_free(motors);
        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
       }

       err=atp_motor_create(&motors[ATP_MOTOR_FRONT_LEFT],input,ATP_MOTOR_FRONT_LEFT,EM_GPIO_8,7);
       if(err){
       atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Motor %u  failed Errno:%u\n",ATP_MOTOR_FRONT_LEFT,err));
       atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
               atp_free(motors[ATP_MOTOR_BACK_RIGHT]);
               atp_free(motors[ATP_MOTOR_BACK_LEFT]);
       atp_free(motors);
        return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
      }



        em_uint8 index;
       /* for_each_motors(){


        	err=atp_motor_calibrate(motors[index]);

           if(err){
        	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting Motor %u  failed Errno:%u\n",index,err));
        	   atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
        	   atp_free(motors[ATP_MOTOR_BACK_RIGHT]);
        	   atp_free(motors[ATP_MOTOR_FRONT_LEFT]);
        	   atp_free(motors[ATP_MOTOR_BACK_LEFT]);
        	   atp_free(motors);
        	   return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
           }


        }*/
        for_each_motors(){



        	       // err=atp_motor_calibrate(motors[index]);
                	//err=atp_motor_start(motors[index]);
                	//err=atp_motor_set_power(motors[index],200);
                   if(err){
                	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting Motor %u  failed Errno:%u\n",index,err));
                	   atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
                	   atp_free(motors[ATP_MOTOR_BACK_RIGHT]);
                	   atp_free(motors[ATP_MOTOR_FRONT_LEFT]);
                	   atp_free(motors[ATP_MOTOR_BACK_LEFT]);
                	   atp_free(motors);
                	   return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
                   }


                }



        	/*for_each_motors(){

                       	err=atp_motor_set_power(motors[index],200);
                          if(err){
                       	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting Motor %u  failed Errno:%u\n",index,err));
                       	   atp_free(motors[ATP_MOTOR_FRONT_RIGHT]);
                       	   atp_free(motors[ATP_MOTOR_BACK_RIGHT]);
                       	   atp_free(motors[ATP_MOTOR_FRONT_LEFT]);
                       	   atp_free(motors[ATP_MOTOR_BACK_LEFT]);
                       	   atp_free(motors);
                       	   return ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM;
                          }


                       }*/










        atp_motor_controller *controller=atp_malloc(sizeof(atp_motor_controller));
       	*motor_controller=controller;
       	controller->private_data=motors;

        return ATP_SUCCESS;

}
em_uint32 atp_motor_controller_destroy(atp_motor_controller *motor_controller){

   atp_motor **motors=(atp_motor**)motor_controller->private_data;

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
	atp_free(motor_controller);
	return ATP_SUCCESS;
}


em_uint32 atp_motor_controller_set_values(atp_motor_controller *motor_controller,em_uint16 *values){

	atp_motor **motors=(atp_motor**)motor_controller->private_data;
	if(motors!=NULL)
	{
		em_uint8 index;
		for_each_motors(){
			atp_motor_set_power(motors[index],values[index]);
		}
	}
	return ATP_SUCCESS;
}

