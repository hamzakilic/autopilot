/*
 * atp_input_table.c
 *
 *  Created on: May 12, 2015
 *      Author: root
 */
#include "atp_input.h"




typedef struct{
   em_uint16 values[4];
}atp_input_motor;



typedef struct{
  atp_input_motor *motor;
  void *motor_lock_key;
  void *gps_lock_key;
}atp_input_data;

em_uint32 atp_input_update_gps(atp_input *input,const atp_gps_data * const data){
	return ATP_SUCCESS;
}
em_uint32 atp_input_update_motor (atp_input *input,const atp_motor_data * const data){
    atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
    atp_thread_lock(input_data->motor_lock_key);//lock for multi threads

    input_data->motor->values[data->motor_index]=data->motor_value;

    //create input motor log and send it
    atp_log_data *log_data=atp_new(atp_log_data);
    log_data->type=ATP_LOG_DATA;
    log_data->data_type=ATP_LOG_DATA_TYPE_INPUT_MOTOR;
    log_data->data_len=4*sizeof(em_uint16);
    log_data->data=atp_malloc(log_data->data_len);
    atp_copy(log_data->data,input_data->motor->values,log_data->data_len);
    atp_log(log_data);

    atp_thread_unlock(input_data->motor_lock_key);
	return ATP_SUCCESS;
}


em_uint32 atp_input_create(atp_input **address){
	atp_input *input=atp_new(atp_input);
	atp_fill_zero(input,sizeof(atp_input));

    atp_input_data *input_data=atp_new(atp_input_data);
    atp_fill_zero(input_data,sizeof(atp_input_data));
    atp_thread_create_lock(&input_data->motor_lock_key);
    atp_thread_create_lock(&input_data->gps_lock_key);
    input->private_data=input_data;

    atp_input_motor *input_motor=atp_new(atp_input_motor);
    atp_fill_zero(input_motor,sizeof(atp_input_motor));
    input_data->motor=input_motor;


    *address=input;
    return ATP_SUCCESS;



}
em_uint32 atp_input_destroy(atp_input *input){
    if(input){
    	if(input->private_data){
    		atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
    		if(input_data->motor){
    			if(input_data->gps_lock_key)
    				atp_thread_destory_lock(input_data->gps_lock_key);
    			if(input_data->motor_lock_key)
    				atp_thread_destory_lock(input_data->motor_lock_key);
    			atp_free(input_data->motor);
    		}
    		atp_free(input->private_data);
    	}
    	atp_free(input);

    }

	return ATP_SUCCESS;
}

