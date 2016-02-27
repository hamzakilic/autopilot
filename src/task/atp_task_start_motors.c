/*
 * atp_task_start_motors.c
 *
 *  Created on: Feb 26, 2016
 *      Author: root
 */
#include "atp_task_start_motors.h"


typedef struct {
	atp_task_share *share;
	atp_motor_controller *motor_controller;
	atp_input *input;
	em_int32 kill;
}atp_task_start_motors;

struct atp_task * atp_task_start_motors_create(em_uint8 *bytes,em_uint32 lenght,atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input){

  struct atp_task *task=atp_new(struct atp_task);
  atp_fill_zero(task,sizeof(struct atp_task));
  task->func_exec=atp_task_start_motors_exec;
  task->func_free=atp_task_start_motors_free;
  task->func_kill=atp_task_start_motors_kill;
  task->is_finished=0;
  task->is_started=0;
  task->thread_id=0;
  task->name="Start Motors";

  atp_task_start_motors  *data=atp_new(atp_task_start_motors);
  atp_fill_zero(data,sizeof(atp_task_start_motors));
  data->motor_controller=motor_controller;
  data->share=task_share;
  data->input=input;

  task->parameter=data;

  return task;
}


void * atp_task_start_motors_exec(void *parameter){

   struct atp_task *task=atp_convert(parameter,struct atp_task*);
   atp_task_start_motors *data=atp_convert(task->parameter,atp_task_start_motors*);
   atp_log(atp_log_create_string(ATP_LOG_INFO,"%s Task Execution Starting\n",task->name));
   em_uint32 err;
   em_uint32 index;
   atp_motor_data motor_data;
   em_uint16 powers[ATP_MOTORS_COUNT]={0,0,0,0};
   for(index=0;index<ATP_MOTORS_COUNT;++index){
	   if(data->kill)//if process kill set
		   break;
	   if(atp_task_share_killall_get(data->share))//if every process must kill set
		   break;

	    motor_data.motor_index=index;
        atp_input_get_motor(data->input,index,&motor_data);
        if(motor_data.motor_value==MOTOR_CREATED || motor_data.motor_value==MOTOR_STOPPED || motor_data.motor_value==MOTOR_CALIBRATED){
           powers[index]=80;
        	atp_motor_controller_start_motor(data->motor_controller,index);

        }

   }
   //atp_motor_controller_set_values(data->motor_controller,powers);
   task->is_finished=1;
   return ATP_SUCCESS;

}
void atp_task_start_motors_free(void * parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_start_motors *echo=atp_convert(task->parameter,atp_task_start_motors*);
	atp_free(echo);
	atp_free(task);


}
void atp_task_start_motors_kill(void *parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_start_motors *data=atp_convert(task->parameter,atp_task_start_motors*);
	data->kill=1;
}
