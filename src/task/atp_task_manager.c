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
  struct task_node *node;

  //these values comes from pilot_data
  atp_input *input;
  atp_motor_controller *motor_controller;
  atp_settings *settings;

}task_manager_data;




void add_to_task(struct atp_task *task,task_manager_data *data){
    atp_thread_lock(data->thread_lock);
	struct task_node *current_node=data->node;
	struct task_node *previous=data->node;
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
				previous->next=node;
			}
    atp_thread_unlock(data->thread_lock);

}






void start_task(struct task_node *node){

  em_uint32 err= atp_thread_create(&node->task->thread_id,node->task->func_exec,node->task->parameter);
  if(err)
	  atp_log(atp_log_create_string(ATP_LOG_FATAL,"Creating Task Faield Error:%d %s\n",err, node->task->name));
  else
  atp_log(atp_log_create_string(ATP_LOG_INFO,"Creating Task Success %s\n",node->task->name));

}

void finish_task(struct task_node *node,task_manager_data *data){
   atp_log(atp_log_create_string(ATP_LOG_INFO,"Finishing Task Success %s\n",node->task->name));
   atp_thread_join(&node->task->thread_id);
   em_int32 nextcount=0;
   for(nextcount=0;nextcount<node->task->next_task_list_count;++nextcount){
      add_to_task(node->task->next_task_list[nextcount],data);
   }

   if(node->task->func_free)
      node->task->func_free(node->task);
   atp_thread_lock(data->thread_lock);
   if(node->prev)
   {
	 node->prev->next=node->next;
   }else{
     data->node=node->next;
   }
   atp_thread_unlock(data->thread_lock);
   atp_free(node);


}

void waitalltasks(task_manager_data *data){

		struct task_node *current_node=data->node;
				while(current_node)
				{
					if(!current_node->task->is_finished)
						current_node->task->func_kill();
					current_node=current_node->next;
				}


		   while(data->node)
		   {
			 while(!data->node->task->is_finished);//buradaki ; çok önemli
				finish_task(data->node,data);
		   }

}


void * process_tasks(void *temp_data){
	task_manager_data *data=atp_convert(temp_data,task_manager_data*);
	em_int32 index=0;

	while(data->work){

		struct task_node *current_node=data->node;
		while(current_node)
		{

			if(!current_node->task->is_started)
				start_task(current_node);
			if(current_node->task->is_finished)
				finish_task(current_node,data);
			current_node=current_node->next;
		}


        em_io_delay_microseconds(10);

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
    		if(data->thread_id)
    			atp_thread_join(&data->thread_id);
    		atp_thread_destory_lock(data->thread_lock);
    		atp_free(data);
    	}
    	atp_free(task_manager);
    }
    return ATP_SUCCESS;
}

struct atp_task * create_atp_task(em_uint8* data,em_uint32 length){
return NULL;
}

em_uint32 atp_task_manager_add_task(em_uint8 *data,em_uint32 length,atp_task_manager *task_manager){

	task_manager_data *manager_data=atp_convert(task_manager->private_data,task_manager_data *);
	struct atp_task *task=create_atp_task(data,length);
	if(task)
		add_to_task(task,manager_data);
	return ATP_SUCCESS;

}
