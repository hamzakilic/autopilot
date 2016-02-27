/*
 * atp_task_motor_value.h
 *
 *  Created on: Feb 27, 2016
 *      Author: root
 */

#ifndef ATP_TASK_MOTORVALUE_H_
#define ATP_TASK_MOTORVALUE_H_

#include "atp_task.h"
#include "../controllers/atp_motor.h"
#include "../controllers/atp_motor_controller.h"


struct atp_task * atp_task_motorvalue_create(em_uint8 *bytes,em_uint32 length,atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input);
void * atp_task_motorvalue_exec(void *parameter);
void atp_task_motorvalue_free(void *parameter);
void atp_task_motorvalue_kill(void *parameter);



#endif /* ATP_TASK_MOTORVALUE_H_ */
