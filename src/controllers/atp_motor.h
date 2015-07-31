/*
 * atp_motor.h
 *
 *  Created on: Jun 20, 2015
 *      Author: root
 */

#ifndef ATP_MOTOR_H_
#define ATP_MOTOR_H_

#include "../common/atp_common.h"

struct atp_motor{
	//read-only
	em_uint16 power;
    em_uint8 number;
    em_uint8 pin_number;
};

em_uint32 atp_motor_calibrate(struct atp_motor *motor);
em_uint32 atp_motor_create(struct atp_motor **motor,em_uint8 number,em_uint8 pin_number);
em_uint32 atp_motor_destroy(struct atp_motor *motor);
em_uint32 atp_motor_start(struct atp_motor *motor);
em_uint32 atp_motor_stop(struct atp_motor *motor);
em_uint32 atp_motor_set_power(struct atp_motor *motor,em_uint16 power_level);

#endif /* ATP_MOTOR_H_ */
