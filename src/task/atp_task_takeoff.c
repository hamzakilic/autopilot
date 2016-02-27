/*
 * atp_task_takeoff.c
 *
 *  Created on: Feb 27, 2016
 *      Author: root
 */

#include "atp_task_takeoff.h"



typedef struct {
	atp_task_share *share;
	atp_motor_controller *motor_controller;
	atp_input *input;

}atp_task_takeoff;

struct atp_task * atp_task_takeoff_create(em_uint8 *bytes,em_uint32 lenght,atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input){

  struct atp_task *task=atp_new(struct atp_task);
  atp_fill_zero(task,sizeof(struct atp_task));
  task->func_exec=atp_task_takeoff_exec;
  task->func_free=atp_task_takeoff_free;
  task->func_kill=atp_task_takeoff_kill;
  task->is_finished=0;
  task->is_started=0;
  task->thread_id=0;
  task->name="Takeoff";

  atp_task_takeoff  *data=atp_new(atp_task_takeoff);
  atp_fill_zero(data,sizeof(atp_task_takeoff));
  data->motor_controller=motor_controller;
  data->share=task_share;
  data->input=input;

  task->parameter=data;

  return task;
}


void * atp_task_takeoff_exec(void *parameter){

   struct atp_task *task=atp_convert(parameter,struct atp_task*);
   atp_task_takeoff *data=atp_convert(task->parameter,atp_task_takeoff*);
   atp_log(atp_log_create_string(ATP_LOG_INFO,"%s Task Execution Starting\n",task->name));
   //atp_task_share_killall_set(data->share,1);//say all tasks kill
   //while(atp_task_share_task_count_get(data->share)>1);//wait all tasks finish
   //atp_task_share_killall_set(data->share,0);//say all tasks kill
   em_uint32 err;
   em_uint32 index,count;
   atp_motor_data motor_data;
   em_uint16 motor_values[ATP_MOTORS_COUNT];


          em_int32 motors_are_started=0;
          for(index=0;index<ATP_MOTORS_COUNT;++index){
        	  atp_input_get_motor(data->input,index,&motor_data);
        	  motors_are_started|=motor_data.motor_value;
          }

   if(motors_are_started==MOTOR_STARTED)
   for(count=100;count<400;++count){
   for(index=0;index<ATP_MOTORS_COUNT;++index){
	   motor_values[index]=count;
   }
   if(atp_task_share_killall_get(data->share))break;

   atp_motor_controller_set_values(data->motor_controller,motor_values);
   em_io_delay_microseconds(100000);
   }

   if(motors_are_started==MOTOR_STARTED)
   for(count=400;count>10;--count){
      for(index=0;index<ATP_MOTORS_COUNT;++index){
    	  motor_values[index]=count;
      }
      if(atp_task_share_killall_get(data->share)) break;
      atp_motor_controller_set_values(data->motor_controller,motor_values);

       em_io_delay_microseconds(100000);
      }


   //atp_task_share_killall_set(data->share,0);
   task->is_finished=1;
   return ATP_SUCCESS;

}
void atp_task_takeoff_free(void * parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_takeoff *echo=atp_convert(task->parameter,atp_task_takeoff*);
	atp_free(echo);
	atp_free(task);


}
void atp_task_takeoff_kill(void *parameter){


}

