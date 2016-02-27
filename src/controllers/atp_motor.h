/*
 * atp_motor.h
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_H_
#define AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_H_

#include "../common/atp_common.h"
#include "../input/atp_input.h"
#include "../modules/module_Adafruit_PCA9685.h"


#define MOTOR_CREATED -1
#define MOTOR_DESTROYED -2
#define MOTOR_CALIBRATED -3
#define MOTOR_STARTED -4
#define MOTOR_STOPPED -5

#define MOTOR_MAX_VALUE 1000


typedef struct {
	//read-only
	em_uint16 power;
    em_uint8 number;
    em_uint8 raspi_pin_number;
    em_uint8 pwm_pin_number;
    atp_input *input;
}atp_motor;

em_uint32 atp_motor_calibrate(atp_motor *motor);
em_uint32 atp_motor_create(atp_motor **motor,atp_input *input, em_uint8 number,em_uint8 raspi_pin_number,em_uint8 pwm_pin_number);
em_uint32 atp_motor_destroy(atp_motor *motor);
em_uint32 atp_motor_start(atp_motor *motor);
em_uint32 atp_motor_stop(atp_motor *motor);
em_uint32 atp_motor_set_power(atp_motor *motor,em_uint16 power_level);

#endif /* AUTOPILOTC_SRC_CONTROLLERS_ATP_MOTOR_H_ */
