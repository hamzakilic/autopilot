/*
 * atp_task_echo.h
 *
 *  Created on: Feb 22, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_ATP_TASK_ECHO_H_
#define AUTOPILOTC_SRC_ATP_TASK_ECHO_H_

#include "atp_task.h"



struct atp_task * atp_task_echo_create(em_uint8 *bytes,em_uint32 length);
void * atp_task_echo_exec(void *parameter);
void atp_task_echo_free(void *parameter);
void atp_task_echo_kill(void *parameter);



#endif /* ATP_TASK_ECHO_H_ */
