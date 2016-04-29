/*
 * atp_task_manager.c
 *
 *  Created on: Feb 21, 2016
 *      Author: root
 */
#include "atp_task_manager.h"



struct task_node{
	struct task_node *prev;
	struct atp_task *task;
	struct task_node *next;
};

typedef struct {
  atp_thread_id thread_id;
  em_uint32 work;
  void *thread_lock;
  struct task_node *head_node;
  em_uint32 count_of_task;
  //these values comes from pilot_data
  atp_input *input;
  atp_motor_controller *motor_controller;
  atp_settings *settings;
  atp_task_share *task_share;
  em_int32 tasks_count;

}task_manager_data;




em_uint32 add_to_task(struct atp_task *task,task_manager_data *data){
	em_uint32 err=atp_thread_lock(data->thread_lock);

	struct task_node *current_node=data->head_node;

	struct task_node *previous=data->head_node;


			while(current_node)
			{
				previous=current_node;
				current_node=current_node->next;

			}

			if(previous){

				struct task_node *node=atp_new(struct task_node);
				atp_fill_zero(node,sizeof(struct task_node));
				node->task=task;
				node->prev=previous;
				node->next=NULL;
				previous->next=node;

			}else{

								struct task_node *node=atp_new(struct task_node);
								atp_fill_zero(node,sizeof(struct task_node));
								node->task=task;
								data->head_node=node;

			}
			data->tasks_count++;
    atp_task_share_count_plus(data->task_share);


    atp_thread_unlock(data->thread_lock);
    return ATP_SUCCESS;

}






void start_task(struct task_node *node,task_manager_data *data){

  atp_log(atp_log_create_string(ATP_LOG_DATA_TYPE_INPUT,"Starting Task:%s",node->task->name));
  node->task->is_started=1;
  em_uint32 err= atp_thread_create(&node->task->thread_id,node->task->func_exec,node->task);
  if(err){
	  node->task->is_started=0;
	  atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting Task Failed Error:%d %s\n",err, node->task->name));
  }
  else
  atp_log(atp_log_create_string(ATP_LOG_INFO,"Starting Task Success %s\n",node->task->name));


}

void finish_task(struct task_node *node,task_manager_data *data,em_int32 lock_data_lock){


   atp_log(atp_log_create_string(ATP_LOG_INFO,"Finishing Task Success %s\n",node->task->name));

   atp_thread_join(&(node->task->thread_id));

   em_uint32 nextcount=0;

   for(nextcount=0;nextcount<node->task->next_task_list_count;++nextcount){

      add_to_task(node->task->next_task_list[nextcount],data);

   }
   if(lock_data_lock)
   atp_thread_lock(data->thread_lock);

   if(node->task->func_free)
      node->task->func_free(node->task);

   if(node->prev)   {

	 node->prev->next=node->next;
	 if(node->next)
	 node->next->prev=node->prev;

   }else{

     if(node->next)
    	 node->next->prev=NULL;
     data->head_node=node->next;

   }

   atp_free(node);
   data->tasks_count--;
   atp_task_share_count_minus(data->task_share);
   if(lock_data_lock)
   atp_thread_unlock(data->thread_lock);







}

void waitalltasks(task_manager_data *data){

	atp_thread_lock(data->thread_lock);
		struct task_node *current_node=data->head_node;
				while(current_node)
				{

					if(!current_node->task->is_finished){

						current_node->task->func_kill(current_node->task);
					}
					current_node=current_node->next;
				}


		   while(data->head_node)
		   {

			 while(!data->head_node->task->is_finished){//burası boş olmalı
				 em_io_delay_microseconds(50);

			 }

				finish_task(data->head_node,data,0);

		   }

	atp_thread_unlock(data->thread_lock);
}


void * process_tasks(void *temp_data){
	task_manager_data *data=atp_convert(temp_data,task_manager_data*);
	em_int32 count=0;

	while(data->work){


		struct task_node *current_node=data->head_node;
		struct task_node *next=NULL;
		count=0;

		while(current_node)
		{

			next=current_node->next;
			if(!current_node->task->is_started){

				start_task(current_node,data);
			}
			if(current_node->task->is_finished){

				finish_task(current_node,data,1);
			}

			current_node=next;
           count++;
		}
		data->count_of_task=count;

        em_io_delay_microseconds(100);

        }

	waitalltasks(data);


	return 0;
}


em_uint32 atp_task_manager_create(atp_task_manager **task_manager,atp_input *input,atp_motor_controller *motor_controller,atp_settings *settings){

	    atp_task_manager * temp= atp_malloc(sizeof(atp_task_manager));
	    task_manager_data *temp_data=  atp_malloc(sizeof(task_manager_data));
	    atp_fill_zero(temp_data,sizeof(task_manager_data));
	    temp_data->input=input;
	    temp_data->motor_controller=motor_controller;
	    temp_data->settings=settings;
	    atp_task_share_create(&temp_data->task_share);
	    temp->private_data=temp_data;
	    *task_manager=temp;
	    temp_data->work=1;
	    atp_thread_create_lock(&temp_data->thread_lock);
	    atp_thread_create(&temp_data->thread_id,process_tasks,temp_data);
		return ATP_SUCCESS;


}
em_uint32 atp_task_manager_destroy(atp_task_manager *task_manager){
    if(task_manager){
    	if(task_manager->private_data){
    		task_manager_data *data=atp_convert(task_manager->private_data,task_manager_data *);
    		data->work=0;
    		waitalltasks(data);
    		if(data->thread_id)
    			atp_thread_join(&data->thread_id);
    		atp_thread_destory_lock(data->thread_lock);
    		atp_task_share_destroy(data->task_share);
    		atp_free(data);
    	}
    	atp_free(task_manager);
    }
    return ATP_SUCCESS;
}


em_uint32 check_hash(em_uint8 *data,em_uint32 lenght){


	em_uint16 data_type=data[0];
	data_type|=data[1]<<8;
	em_uint32 data_length=data[2];
	data_length|=data[3]<<8;
	data_length|=data[4]<<16;
	data_length|=data[5]<<24;
	em_uint32 sended_hash=data[6];
	sended_hash|=data[7]<<8;
	sended_hash|=data[8]<<16;
	sended_hash|=data[9]<<24;
	em_uint32 calculated_hash=data[0]+data[1]+data[2]+data[3]+data[4]+data[5];
	if(sended_hash==calculated_hash & data_length==lenght-10)
		return 0;
	atp_log(atp_log_create_string(ATP_LOG_ERROR,"Task is not valid"));
	return 1;
}

struct atp_task * create_atp_task(em_uint8* data,em_uint32 length,task_manager_data *manager_data){


	if(length<10)
		return NULL;
     if(check_hash(data,length))
    	 return NULL;
	em_uint16 data_type=data[0];
	data_type|=data[1]<<8;
   switch(data_type){
   case ATP_TASK_ECHO:
	   return atp_task_echo_create(data+10,length-10);
   case ATP_TASK_EMERGENCY:
       return atp_task_emergency_create(NULL,0,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_START_MOTORS:
	   return atp_task_start_motors_create(NULL,0,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_STOP_MOTORS:
   	   return atp_task_stop_motors_create(NULL,0,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_TAKEOFF:
      	   return atp_task_takeoff_create(NULL,0,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_MOTORCALIBRATE:
         	return atp_task_motorcalibrate_create(NULL,0,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_MOTORVALUE:
        return atp_task_motorvalue_create(data+10,length-10,manager_data->task_share,manager_data->motor_controller,manager_data->input);
   case ATP_TASK_BALANCE:
        return atp_task_balance_create(manager_data->task_share,manager_data->motor_controller,manager_data->input);
   default:
	   return NULL;


   }

}

em_uint32 atp_task_manager_add_task(em_uint8 *data,em_uint32 length,atp_task_manager *task_manager){

	task_manager_data *manager_data=atp_convert(task_manager->private_data,task_manager_data *);
	struct atp_task *task=create_atp_task(data,length,manager_data);

	if(task && manager_data->work){

	   em_uint32 err=add_to_task(task,manager_data);

	}
	return ATP_SUCCESS;

}

