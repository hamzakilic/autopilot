/*
 * atp_task_emergency.c
 *
 *  Created on: Feb 27, 2016
 *      Author: root
 */


#include "atp_task_emergency.h"



typedef struct {
	atp_task_share *share;
	atp_motor_controller *motor_controller;
	atp_input *input;

}atp_task_emergency;

struct atp_task * atp_task_emergency_create(em_uint8 *bytes,em_uint32 lenght,atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input){

  struct atp_task *task=atp_new(struct atp_task);
  atp_fill_zero(task,sizeof(struct atp_task));
  task->func_exec=atp_task_emergency_exec;
  task->func_free=atp_task_emergency_free;
  task->func_kill=atp_task_emergency_kill;
  task->is_finished=0;
  task->is_started=0;
  task->thread_id=0;
  task->name="Emergency";

  atp_task_emergency  *data=atp_new(atp_task_emergency);
  atp_fill_zero(data,sizeof(atp_task_emergency));
  data->motor_controller=motor_controller;
  data->share=task_share;
  data->input=input;

  task->parameter=data;

  return task;
}


void * atp_task_emergency_exec(void *parameter){

   struct atp_task *task=atp_convert(parameter,struct atp_task*);
   atp_task_emergency *data=atp_convert(task->parameter,atp_task_emergency*);
   atp_log(atp_log_create_string(ATP_LOG_INFO,"%s Task Execution Starting\n",task->name));
   atp_task_share_killall_set(data->share,1);//say all tasks kill
   //while(atp_task_share_task_count_get(data->share)>1);//wait all tasks to finish
   em_uint32 err;
   em_uint32 index;
   atp_motor_data motor_data;
   for(index=0;index<ATP_MOTORS_COUNT;++index){
	    motor_data.motor_index=index;
       	atp_motor_controller_stop_motor(data->motor_controller,index);

   }
   while(atp_task_share_task_count_get(data->share)>1) em_io_delay_microseconds(100);
   atp_task_share_killall_set(data->share,0);
   task->is_finished=1;
   return ATP_SUCCESS;

}
void atp_task_emergency_free(void * parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_emergency *echo=atp_convert(task->parameter,atp_task_emergency*);
	atp_free(echo);
	atp_free(task);


}
void atp_task_emergency_kill(void *parameter){


}
