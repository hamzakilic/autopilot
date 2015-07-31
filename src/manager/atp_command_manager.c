/*
 * atp_command_controller.c
 *
 *  Created on: Jun 24, 2015
 *      Author: root
 */

#include "atp_command_manager.h"

typedef struct {
  atp_queue *queue;
  atp_thread_id thread_id;
  em_uint32 work;
  void *thread_lock;

}controller_data;


void process_command(atp_command *command){

}

void* process_queue_start(void *arg){
	controller_data *data=(controller_data *)arg;


	while(data->work){
		atp_thread_lock(&data->thread_lock);
		if(atp_queue_count(data->queue)){
		 	atp_command *queue_item=(atp_command *) atp_queue_pop(data->queue);
		 	atp_thread_unlock(&data->thread_lock);
		 	process_command(queue_item);
		}else{
			atp_thread_unlock(&data->thread_lock);
		em_io_delay_microseconds(1000);
		}
	}

  return ATP_SUCCESS;
}


em_uint32  atp_command_manager_create(atp_command_manager ** controller){
	atp_command_manager * temp= atp_malloc(sizeof(atp_command_manager));
    controller_data *temp_data=  atp_malloc(sizeof(controller_data));
    temp->private_data=temp_data;
    temp_data->queue= atp_queue_create();
    *controller=temp;
    temp_data->work=1;
    atp_thread_create_lock(&temp_data->thread_lock);
    atp_thread_create(&temp_data->thread_id,process_queue_start,temp_data);
	return ATP_SUCCESS;
}




em_uint32  atp_command_manager_destroy(atp_command_manager * controller){
    if(controller){
    	if(controller->private_data){

    		controller_data *data= (controller_data *) controller->private_data;
    		data->work=0;
    		atp_thread_join(&data->thread_id);

    		if(data->queue)
    			atp_free(data->queue);

    		atp_free(controller->private_data);
    	}
    	atp_free(controller);
    }
    return ATP_SUCCESS;
}

