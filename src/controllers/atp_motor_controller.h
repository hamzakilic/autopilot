/*
 * atp_motor_controller.h
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_CONTROLLER_H_
#define AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_CONTROLLER_H_

#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "atp_motor.h"
#include "../modules/module_Adafruit_PCA9685.h"




#define ATP_MOTORS_COUNT 4
#define ATP_MOTOR_FRONT_LEFT 0x0
#define ATP_MOTOR_FRONT_RIGHT 0x1
#define ATP_MOTOR_BACK_RIGHT 0x2
#define ATP_MOTOR_BACK_LEFT 0x3


#define ATP_MOTOR_MAX_POWER 1000
#define ATP_MOTOR_MIN_POWER 0

typedef struct{
	void *private_data;
}atp_motor_controller;


em_uint32 atp_motor_controller_create(atp_input *input,atp_motor_controller **motor_controller);
em_uint32 atp_motor_controller_destroy(atp_motor_controller *motor_controller);
em_uint32 atp_motor_controller_set_values(atp_motor_controller *motor_controller,em_uint16 *values);
em_uint32 atp_motor_controller_set_value(atp_motor_controller *motor_controller,em_uint32 index,em_uint16 value);
em_uint32 atp_motor_controller_start_motors(atp_motor_controller *motor_controller);
em_uint32 atp_motor_controller_start_motor(atp_motor_controller *motor_controller,em_uint32 index);
em_uint32 atp_motor_controller_stop_motor(atp_motor_controller *motor_controller,em_uint32 index);
em_uint32 atp_motor_controller_calibrate(atp_motor_controller *motor_controller);





#endif /* AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_CONTROLLER_H_ */
