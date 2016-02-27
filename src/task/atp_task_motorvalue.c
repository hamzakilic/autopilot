/*
 * atp_task_motor_value.c
 *
 *  Created on: Feb 27, 2016
 *      Author: root
 */


#include "atp_task_motorvalue.h"



typedef struct {
	atp_task_share *share;
	atp_motor_controller *motor_controller;
	atp_input *input;
	em_uint32 motor_value;

}atp_task_motorvalue;

struct atp_task * atp_task_motorvalue_create(em_uint8 *bytes,em_uint32 lenght,atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input){

  struct atp_task *task=atp_new(struct atp_task);
  atp_fill_zero(task,sizeof(struct atp_task));
  task->func_exec=atp_task_motorvalue_exec;
  task->func_free=atp_task_motorvalue_free;
  task->func_kill=atp_task_motorvalue_kill;
  task->is_finished=0;
  task->is_started=0;
  task->thread_id=0;
  task->name="Motor Value";

  atp_task_motorvalue  *data=atp_new(atp_task_motorvalue);
  atp_fill_zero(data,sizeof(atp_task_motorvalue));
  data->motor_controller=motor_controller;
  data->share=task_share;
  data->input=input;

  if(lenght==4){
  data->motor_value=bytes[0];
  data->motor_value|=bytes[1]<<8;
  data->motor_value|=bytes[2]<<16;
  data->motor_value|=bytes[3]<<24;
  }

  task->parameter=data;

  return task;
}


void * atp_task_motorvalue_exec(void *parameter){

   struct atp_task *task=atp_convert(parameter,struct atp_task*);
   atp_task_motorvalue *data=atp_convert(task->parameter,atp_task_motorvalue*);
   atp_log(atp_log_create_string(ATP_LOG_INFO,"%s Task Execution Starting\n",task->name));

   em_uint32 err=ATP_SUCCESS;
    em_uint32 index;
    atp_motor_data motor_data;
    em_uint16 powers[ATP_MOTORS_COUNT]={0,0,0,0};

    for(index=0;index<ATP_MOTORS_COUNT;++index){
 	     motor_data.motor_index=index;
         atp_input_get_motor(data->input,index,&motor_data);

         powers[index]=data->motor_value;
         if(motor_data.motor_value==MOTOR_STARTED || motor_data.motor_value>=0){

         	err|=ATP_SUCCESS;

         }else err |=1;

    }
    if(!err && data->motor_value<=1000){
    atp_motor_controller_set_values(data->motor_controller,powers);

    }else{

    }




   task->is_finished=1;
   return ATP_SUCCESS;

}
void atp_task_motorvalue_free(void * parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_motorvalue *echo=atp_convert(task->parameter,atp_task_motorvalue*);
	atp_free(echo);
	atp_free(task);


}
void atp_task_motorvalue_kill(void *parameter){


}

