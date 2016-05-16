/*
 * atp_task_balance.h
 *
 *  Created on: Apr 24, 2016
 *      Author: root
 */

#ifndef  AUTOPILOTC_SRC_ATP_TASK_BALANCE_H_
#define  AUTOPILOTC_SRC_ATP_TASK_BALANCE_H_

#include "../algorithms/atleastsquare.h"
#include "atp_task.h"
#include "../controllers/atp_motor.h"
#include "../controllers/atp_motor_controller.h"


struct atp_task * atp_task_balance_create(atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input);
void * atp_task_balance_exec(void *parameter);
void atp_task_balance_free(void *parameter);
void atp_task_balance_kill(void *parameter);


#endif /* ATP_TASK_BALANCE_H_ */
