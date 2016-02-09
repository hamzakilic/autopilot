/*
 * atp_motor.c
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */
#include "atp_motor.h"


#ifdef COMPILE_A2212_MOTORS_AND_ESC

#define MIN_SIGNAL_CALIBRATE 800
#define MAX_SIGNAL_CALIBRATE 1800

#define MIN_SIGNAL_WORK 800
#define MAX_SIGNAL_WORK 1800

#endif



/*
 * @brief for updating system input table for motors.
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
#ifdef COMPILE_PCA9685
	     err=adafruit_pca9685_set(MAX_SIGNAL_CALIBRATE,motor->pwm_pin_number);
#endif
	          if(err)
	         	 return err;

	     em_io_delay_microseconds(2000000);

	     em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_HIGH);


	     em_io_delay_microseconds(5000000);
	     err=adafruit_pca9685_set(MIN_SIGNAL_CALIBRATE,motor->pwm_pin_number);
	          if(err)
	         	 return err;

	     em_io_delay_microseconds(5000000);

	     update_input_motor_table(motor->input,motor->number,-3);

	     return ATP_SUCCESS;
}

em_uint32 atp_motor_start(atp_motor *motor){
	em_uint32 err;
	em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_LOW);
#ifdef COMPILE_PCA9685
	 err=adafruit_pca9685_set(MIN_SIGNAL_WORK,motor->pwm_pin_number);
#endif
	          if(err)
	         	 return err;
	 em_io_delay_microseconds(10000);
     em_io_gpio_write(motor->raspi_pin_number,EM_GPIO_HIGH);
     em_io_delay_microseconds(6000000);

     //update input table, -4 means start
     update_input_motor_table(motor->input,motor->number,-4);
     return ATP_SUCCESS;

}


/*! \brief to stop a motor
 * stops a motor suddenly, poweroff and also update input table with -5 value
 *
 */

em_uint32 atp_motor_stop(atp_motor *motor){
	em_uint32 err;
#ifdef COMPILE_PCA9685
		     err=adafruit_pca9685_set(MIN_SIGNAL_WORK,motor->pwm_pin_number);
#endif
		     if(err)
		    	 return err;
		     em_io_gpio_mode(motor->raspi_pin_number,EM_GPIO_LOW);

		     //update input table,-5 means stoped
		     update_input_motor_table(motor->input,motor->number,-5);


		     return ATP_SUCCESS;
}


/*! \brief to set motor power
 * set motor power
 * also updates input table motor values with power_level
 */

em_uint32 atp_motor_set_power(atp_motor *motor,em_uint16 power_level){
	         em_uint32 err;
	         if(power_level>1000)
	        	 power_level=1000;
            em_uint16  calc_power_level=(MAX_SIGNAL_WORK-MIN_SIGNAL_WORK)*power_level/1000.0f+MIN_SIGNAL_WORK;
#ifdef COMPILE_TEST_CODES
            atp_log(atp_log_create_string(ATP_LOG_INFO,"setting motor %d value:%d\n",motor->number,calc_power_level));

#else
#ifdef COMPILE_PCA9685
		     err=adafruit_pca9685_set(calc_power_level,motor->pwm_pin_number);
#endif
		     if(err)
		    	 return err;
#endif
		     //update input table
		     update_input_motor_table(motor->input,motor->number,power_level);
		     return ATP_SUCCESS;
}

