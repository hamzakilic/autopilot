/*
 * atp_task_echo.c
 *
 *  Created on: Feb 23, 2016
 *      Author: root
 */

#include "atp_task_echo.h"

typedef struct {
	em_uint8 echo[1024];
}atp_task_echo;

struct atp_task * atp_task_echo_create(em_uint8 *bytes,em_uint32 lenght){
  struct atp_task *task=atp_new(struct atp_task);
  atp_fill_zero(task,sizeof(struct atp_task));
  task->func_exec=atp_task_echo_exec;
  task->func_free=atp_task_echo_free;
  task->func_kill=atp_task_echo_kill;
  task->is_finished=0;
  task->is_started=0;
  task->thread_id=0;
  task->name="Task Echo";

  atp_task_echo  *echo=atp_new(atp_task_echo);
  atp_fill_zero(echo,sizeof(atp_task_echo));
  if(lenght<1024)
  atp_copy(echo->echo,bytes,lenght);
  //task->name=echo->echo;
  task->parameter=echo;
  return task;
}
void * atp_task_echo_exec(void *parameter){

   struct atp_task *task=atp_convert(parameter,struct atp_task*);
   atp_task_echo *echo=atp_convert(task->parameter,atp_task_echo*);
   atp_log(atp_log_create_string(ATP_LOG_INFO,"Echo Task Execution:%s\n",echo->echo));

   task->is_finished=1;
   return ATP_SUCCESS;

}
void atp_task_echo_free(void * parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_echo *echo=atp_convert(task->parameter,atp_task_echo*);
	atp_free(echo);
	atp_free(task);


}
void atp_task_echo_kill(void *parameters){

}
