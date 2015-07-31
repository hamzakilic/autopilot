/*
 * atp_motor_controller.h
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#ifndef ATP_MOTOR_CONTROLLER_H_
#define ATP_MOTOR_CONTROLLER_H_

#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "atp_motor.h"



#define ATP_MOTORS_COUNT 4
#define ATP_MOTOR_FRONT_RIGHT 0x0
#define ATP_MOTOR_BACK_RIGHT 0x1
#define ATP_MOTOR_BACK_LEFT 0x2
#define ATP_MOTOR_FRONT_LEFT 0x3

#define ATP_MOTOR_MAX_POWER 1000
#define ATP_MOTOR_MIN_POWER 0

em_uint32  atp_motor_controller_start(atp_input *input);
em_uint32 atp_motor_controller_stop();




#endif /* ATP_MOTOR_CONTROLLER_H_ */
