/*
 * atp_input_table.c
 *
 *  Created on: May 12, 2015
 *      Author: root
 */
#include "atp_input.h"




typedef struct{
   em_int16 values[4];
}atp_input_motor;

typedef struct {
	em_uint16 year;
	em_uint8 month;
	em_uint8 day;
	em_uint8 hour;
	em_uint8 min;
	em_uint8 sec;
}atp_input_time;


typedef struct{
	em_int32 longtitude; //degrees
	em_int32 latitude;//degreess;
	em_int32 sealevel;//mm
}atp_input_location;


typedef struct{
	em_uint8 gps_fix;
	em_uint8 status;//if zero gpxFixOK
	em_uint32 msss;//milisecond since startup/reset
}atp_input_location_ex;




typedef struct{
  atp_input_motor motor;
  atp_input_time gps_time;
  atp_input_location gps_location;
  atp_input_location_ex gps_location_ex;

  void *motor_lock_key;
  void *gps_time_lock_key;
  void *gps_location_lock_key;
  void *gps_location_ex_lock_key;
  atp_thread_id thread_id;
  em_int32 work;
}atp_input_data;


void* atp_input_log(void *ptr){

	atp_input_data *input_data=atp_convert(ptr,atp_input_data*);
	em_uint8 buffer[64];
	while(input_data->work){
		//create input motor log and send it


		atp_thread_lock(input_data->gps_location_lock_key);
		buffer[0]=input_data->gps_location.latitude>>24;
		buffer[1]=input_data->gps_location.latitude>>16;
		buffer[2]=input_data->gps_location.latitude>>8;
		buffer[3]=input_data->gps_location.latitude;


		buffer[4]=input_data->gps_location.longtitude>>24;
		buffer[5]=input_data->gps_location.longtitude>>16;
		buffer[6]=input_data->gps_location.longtitude>>8;
		buffer[7]=input_data->gps_location.longtitude;

		buffer[8]=input_data->gps_location.sealevel>>24;
		buffer[9]=input_data->gps_location.sealevel>>16;
		buffer[10]=input_data->gps_location.sealevel>>8;
		buffer[11]=input_data->gps_location.sealevel;
		atp_thread_unlock(input_data->gps_location_lock_key);

		atp_thread_lock(input_data->gps_location_ex_lock_key);
		buffer[12]=input_data->gps_location_ex.gps_fix;
		buffer[13]=input_data->gps_location_ex.status;

		buffer[14]=input_data->gps_location_ex.msss>>24;
		buffer[15]=input_data->gps_location_ex.msss>>16;
		buffer[16]=input_data->gps_location_ex.msss>>8;
		buffer[17]=input_data->gps_location_ex.msss;
		atp_thread_unlock(input_data->gps_location_ex_lock_key);

		atp_thread_lock(input_data->gps_time_lock_key);
		buffer[18]=input_data->gps_time.year>>8;
		buffer[19]=input_data->gps_time.year;

		buffer[20]=input_data->gps_time.month;
		buffer[21]=input_data->gps_time.day;
		buffer[22]=input_data->gps_time.hour;
		buffer[23]=input_data->gps_time.min;
		buffer[24]=input_data->gps_time.sec;
		atp_thread_unlock(input_data->gps_time_lock_key);

		atp_thread_lock(input_data->motor_lock_key);
		buffer[25]=input_data->motor.values[0]>>8;
		buffer[26]=input_data->motor.values[0];

		buffer[27]=input_data->motor.values[1]>>8;
		buffer[28]=input_data->motor.values[1];

		buffer[29]=input_data->motor.values[2]>>8;
		buffer[30]=input_data->motor.values[2];

		buffer[31]=input_data->motor.values[3]>>8;
		buffer[32]=input_data->motor.values[3];
		atp_thread_unlock(input_data->motor_lock_key);





	    atp_log_data *log_data=atp_new(atp_log_data);
	    log_data->type=ATP_LOG_DATA;
	    log_data->data_type=ATP_LOG_DATA_TYPE_INPUT;
	    log_data->data_len=32;
	    log_data->data=atp_malloc(log_data->data_len);
	    atp_copy(log_data->data,buffer,log_data->data_len);
	    atp_log(log_data);
	    em_io_delay_microseconds(1000000);
	    }
	return ATP_SUCCESS;
}


em_uint32 atp_input_update_gps_location_ex(atp_input *input, atp_gps_location_ex_data  data){

	atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
		atp_thread_lock(input_data->gps_location_ex_lock_key);//lock for multi threads
	    input_data->gps_location_ex.gps_fix=data.gps_fix;
	    input_data->gps_location_ex.status=data.flags;
	    input_data->gps_location_ex.msss=data.msss;
		atp_thread_unlock(input_data->gps_location_ex_lock_key);
		return ATP_SUCCESS;

}



em_uint32 atp_input_update_gps_location(atp_input *input, atp_gps_location_data  data){

	atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
		atp_thread_lock(input_data->gps_location_lock_key);//lock for multi threads
	    input_data->gps_location.latitude=data.latitude;
	    input_data->gps_location.longtitude=data.longtitude;
	    input_data->gps_location.sealevel=data.sealevel;
		atp_thread_unlock(input_data->gps_location_lock_key);
		return ATP_SUCCESS;

}


em_uint32 atp_input_update_gps_time(atp_input *input, atp_gps_time_data  data){
	atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
	atp_thread_lock(input_data->gps_time_lock_key);//lock for multi threads
    input_data->gps_time.day=data.day;
    input_data->gps_time.hour=data.hour;
    input_data->gps_time.min=data.min;
    input_data->gps_time.month=data.month;
    input_data->gps_time.sec=data.sec;
    input_data->gps_time.year=data.year;
	atp_thread_unlock(input_data->gps_time_lock_key);

	return ATP_SUCCESS;
}
em_uint32 atp_input_update_motor (atp_input *input,atp_motor_data data){
    atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);

    atp_thread_lock(input_data->motor_lock_key);//lock for multi threads

    input_data->motor.values[data.motor_index]=data.motor_value;

    atp_thread_unlock(input_data->motor_lock_key);
	return ATP_SUCCESS;
}


em_uint32 atp_input_create(atp_input **address){
	atp_input *input=atp_new(atp_input);
	atp_fill_zero(input,sizeof(atp_input));

    atp_input_data *input_data=atp_new(atp_input_data);
    atp_fill_zero(input_data,sizeof(atp_input_data));
    atp_thread_create_lock(&input_data->motor_lock_key);
    atp_thread_create_lock(&input_data->gps_time_lock_key);
    input->private_data=input_data;

    //start logging
    input_data->work=1;
    atp_thread_create(&input_data->thread_id,atp_input_log,(void*)input_data);


   /*atp_input_motor *input_motor=atp_new(atp_input_motor);
    atp_fill_zero(input_motor,sizeof(atp_input_motor));
    input_data->motor=input_motor;*/


    *address=input;
    return ATP_SUCCESS;



}
em_uint32 atp_input_destroy(atp_input *input){
    if(input){
    	if(input->private_data){
    		atp_input_data *input_data=atp_convert(input->private_data,atp_input_data*);
    		input_data->work=0;
    		atp_thread_join(&input_data->thread_id);

    			if(input_data->gps_time_lock_key)
    				atp_thread_destory_lock(input_data->gps_time_lock_key);
    			if(input_data->motor_lock_key)
    				atp_thread_destory_lock(input_data->motor_lock_key);


    		atp_free(input->private_data);
    	}
    	atp_free(input);

    }

	return ATP_SUCCESS;
}

