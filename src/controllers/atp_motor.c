/*
 * atp_motor.c
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */
#include "atp_motor.h"


#ifdef COMPILE_A2212_MOTORS_AND_ESC

#define MIN_SIGNAL_CALIBRATE 700
#define MAX_SIGNAL_CALIBRATE 2000

#define MIN_SIGNAL_WORK 700
#define MAX_SIGNAL_WORK 1700



em_uint32 set_value(em_uint16 value,em_uint8 pin_number)
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

	//try 100 times then return error
	//
	for(index=0;index<100;++index){
		err=em_io_i2c_write(EM_USE_BSC1,slave,datapwm,5);
		        	if(!err)
		        		break;
		        err=0;
	}

	if(index==100){
	           return ATP_ERROR_HARDWARE_COMMUNICATION;
	        }
	return ATP_SUCCESS;
}


/// for updating system input table for motors.
/**
 * value can be negative
 * negative values have different meanings like
 * -1 means created,
 * -2 means destroyed ....
 */
inline void update_input_motor_table(atp_input *input, em_uint8 index,em_int16 value){
	 atp_motor_data data;
     data.motor_index=index;
	 data.motor_value=value;
	 atp_input_update_motor(input,data);
}



em_uint32 atp_motor_create(atp_motor **motor,atp_input *input, em_uint8 number,em_uint8 raspi_pin_number,em_uint8 pwm_pin_number){
	atp_motor *temp= atp_malloc(sizeof(atp_motor));
	temp->number=number;
	temp->raspi_pin_number=raspi_pin_number;
	temp->pwm_pin_number=pwm_pin_number;
	temp->input=input;
	*motor=temp;
	em_io_gpio_mode(temp->raspi_pin_number,EM_MODE_GPIO_OUT);
	em_io_gpio_write(temp->raspi_pin_number,EM_GPIO_LOW);

	/*update input table*/
	update_input_motor_table(input,number,-1);
	return ATP_SUCCESS;
}
em_uint32 atp_motor_destroy(atp_motor *motor){
	update_input_motor_table(motor->input,motor->number,-2);
	atp_free(motor);
	return ATP_SUCCESS;
}
em_uint32 atp_motor_calibrate(atp_motor *motor){
	em_uint32 err;
	     em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_LOW);
	     err=set_value(MAX_SIGNAL_CALIBRATE,motor->pwm_pin_number);
	          if(err)
	         	 return err;
	     em_io_delay_microseconds(2000000);

	     em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_HIGH);

	     em_io_delay_microseconds(3000000);
	     err=set_value(MIN_SIGNAL_CALIBRATE,motor->pwm_pin_number);
	          if(err)
	         	 return err;
	     em_io_delay_microseconds(3000000);
	     //err=set_value(800,motor->pwm_pin_number);
	     update_input_motor_table(motor->input,motor->number,-3);

	     return ATP_SUCCESS;
}

em_uint32 atp_motor_start(atp_motor *motor){
	em_uint32 err;

     em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_LOW);

     err=set_value(MIN_SIGNAL_CALIBRATE,motor->pwm_pin_number);
          if(err)
         	 return err;
     em_io_delay_microseconds(2000000);

    // em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_HIGH);

     //update input table, -4 means start
     update_input_motor_table(motor->input,motor->number,-4);
     return ATP_SUCCESS;

}
em_uint32 atp_motor_stop(atp_motor *motor){
	em_uint32 err;
		     err=set_value(MIN_SIGNAL_WORK,motor->pwm_pin_number);
		     if(err)
		    	 return err;
		     em_io_gpio_mode(motor->raspi_pin_number,EM_GPIO_LOW);

		     //update motor value,-5 means stoped
		     update_input_motor_table(motor->input,motor->number,-5);


		     return ATP_SUCCESS;
}
em_uint32 atp_motor_set_power(atp_motor *motor,em_uint16 power_level){
	         em_uint32 err;
	         if(power_level>1000)
	        	 power_level=1000;
             power_level=(MAX_SIGNAL_WORK-MIN_SIGNAL_WORK)*power_level/1000.0f;
#ifdef COMPILE_TEST_CODES
             printf("setting motor %d value:%d\n",motor->pin_number,power_level);
#else
		     err=set_value(power_level,motor->pwm_pin_number);
		     if(err)
		    	 return err;
#endif
		     //update input table
		     update_input_motor_table(motor->input,motor->number,power_level);
		     return ATP_SUCCESS;
}
#endif
