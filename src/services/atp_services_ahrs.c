/*
 * atp_services_ahrs.c
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */
#include "atp_services_ahrs.h"



typedef struct {
   em_int32 work;
   atp_thread_id thread_id;
   atp_input *input_table;

}atp_services_ahrs_data;


void * start_communication_ahrs(void *data){
	atp_services_ahrs_data *ahrs_data=atp_convert(data,atp_services_ahrs_data*);
	em_uint32 err;

#ifdef COMPILE_LSM303
	 err=adafruit_lsm303_accel_start(NULL);
	 if(err){
		 atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting LSM303 Accelerometer Failed Error:%u\n", err));
		 return (void*)ATP_ERROR_HARDWARE_INITIALIZE;
	 }

	 err=adafruit_lsm303_mag_start(NULL);
	 	 if(err){
	 		 atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting LSM303 Magnetormeter Failed Error:%u\n", err));
	 		 return (void*)ATP_ERROR_HARDWARE_INITIALIZE;
	 	 }

#endif
#ifdef COMPILE_L3GD20
	 	 err=adafruit_l3gd20_gyro_start(NULL);
	 	 if(err){
	 			 atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting L3GD20 Gyroscope Failed Error:%u\n", err));
	 			 return (void*)ATP_ERROR_HARDWARE_INITIALIZE;
	 		 }

#endif


    float accel_values[3];
	float mag_values[3];
	float gyro_values[3];
	while(ahrs_data->work){
#ifdef COMPILE_LSM303
      adafruit_lsm303_accel_read(accel_values);
      adafruit_lsm303_mag_read(mag_values);
#endif
#ifdef COMPILE_L3GD20
      adafruit_l3gd20_gyro_read(gyro_values);
#endif
      printf("values %f %f %f # %f %f %f # %f %f %f\n",accel_values[0],accel_values[1],accel_values[2],mag_values[0],mag_values[1],mag_values[2],gyro_values[0],gyro_values[1],gyro_values[2]);
      em_io_delay_microseconds(100000);
	}
	return ATP_SUCCESS;
}


em_uint32 atp_services_ahrs_create(atp_services_ahrs **address,atp_input *input){
  em_uint32 err;
  atp_services_ahrs *ahrs=atp_new(atp_services_ahrs);
  atp_fill_zero(ahrs,sizeof(atp_services_ahrs));
  atp_services_ahrs_data *ahrs_data=atp_new(atp_services_ahrs_data);
  atp_fill_zero(ahrs_data,sizeof(atp_services_ahrs_data));
  ahrs->private_data=ahrs_data;


  ahrs_data->work=1;
  ahrs_data->input_table=input;
  err=atp_thread_create(&ahrs_data->thread_id,start_communication_ahrs,ahrs_data);
  if (err) {
  			atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Thread For AHRS Communication Failed Error:%u\n", err));

  			return ATP_ERROR_CREATE_GPS;
  		}

  return ATP_SUCCESS;


}
em_uint32 atp_services_ahrs_destroy(atp_services_ahrs *address){

	if(address && address->private_data){
		atp_services_ahrs_data  *ahrs_data=atp_convert(address->private_data,atp_services_ahrs_data *);
		ahrs_data->work=0;
		if(ahrs_data->thread_id)
			atp_thread_join(&ahrs_data->thread_id);


	}

	return ATP_SUCCESS;

}

