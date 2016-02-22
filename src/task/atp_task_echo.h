/*
 * atp_task_echo.h
 *
 *  Created on: Feb 22, 2016
 *      Author: root
 */

#ifndef ATP_TASK_ECHO_H_
#define ATP_TASK_ECHO_H_

#include "atp_task.h"

struct atp_task * atp_task_echo_create(em_uint8 *bytes);
void * atp_task_echo_exec(void *parameters);
void atp_task_echo_free(struct atp_task * task);
void atp_task_echo_kill(void *parameters);



#endif /* ATP_TASK_ECHO_H_ */
