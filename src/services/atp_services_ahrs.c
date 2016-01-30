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
   atp_settings *settings_table;


}atp_services_ahrs_data;


#ifdef COMPILE_BMP085
/**
 * @brief Calculates the altitude (in meters) from the specified atmospheric \
 * pressure (in hPa), and sea-level pressure (in hPa).
 * @param  seaLevel      Sea-level pressure in hPa
 * @param  atmospheric   Atmospheric pressure in hPa
*/

inline em_float32 pressure_to_altitude(em_float32 seaLevel, em_float32 atmospheric)
{
  // Equation taken from BMP180 datasheet (page 16):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude.  See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}
#endif

volatile float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;


inline em_int32 do_ahrs(em_float32 *accel_values, em_float32 *accel_bias_values, em_float32 *accel_scale_values, em_float32 *mag_values,em_float32 *gyro_values,em_float32 *temperature,em_float32 *pressure,em_float32 *altitude,em_float32 gravity,em_float32 sea_level_pressure, em_uint64 *last_read, atp_services_ahrs_data *ahrs_data){

em_int32 err=ATP_SUCCESS;

#ifdef COMPILE_LSM303
      err |= adafruit_lsm303_accel_read(accel_values,accel_bias_values,accel_scale_values);

      err |= adafruit_lsm303_mag_read(mag_values);


#endif
#ifdef COMPILE_L3GD20
      err |= adafruit_l3gd20_gyro_read(gyro_values);

#endif
#ifdef COMPILE_BMP085
      err |= adafruit_bmp085_temp_press_read(temperature,pressure);
      *altitude=pressure_to_altitude(sea_level_pressure,*pressure);

#endif




      if(!err){
#define PI 3.14159f
    	  atp_dof_data dof_data_temp;
    	  dof_data_temp.accx=accel_values[0]*gravity;
    	  dof_data_temp.accy=accel_values[1]*gravity;
    	  dof_data_temp.accz=accel_values[2]*gravity;

    	  dof_data_temp.gyrox=gyro_values[0];
    	  dof_data_temp.gyroy=gyro_values[1];
    	  dof_data_temp.gyroz=gyro_values[2];

    	  dof_data_temp.magx=mag_values[0];
    	  dof_data_temp.magy=mag_values[1];
    	  dof_data_temp.magz=mag_values[2];

      em_uint64 delta_t=atp_datetime_as_microseconds()- (*last_read);

      //MahonyAHRSupdate(dof_data_temp.gyrox*PI/180.0f,dof_data_temp.gyroy*PI/180.0f,dof_data_temp.gyroz*PI/180.0f,dof_data_temp.accx,dof_data_temp.accy,dof_data_temp.accz,dof_data_temp.magx,dof_data_temp.magy,dof_data_temp.magz);
      MadgwickAHRSupdate(dof_data_temp.gyrox*PI/180.0f,dof_data_temp.gyroy*PI/180.0f,dof_data_temp.gyroz*PI/180.0f,dof_data_temp.accx,dof_data_temp.accy,dof_data_temp.accz,dof_data_temp.magx,dof_data_temp.magy,dof_data_temp.magz);

      *last_read=atp_datetime_as_microseconds();


	   atp_ahrs_data ahrs_data_temp;
	   ahrs_data_temp.altitude=*altitude;
	   ahrs_data_temp.temperature=*temperature;
       ahrs_data_temp.pressure=*pressure;

       em_float32 test = q0*q1 + q2*q3;


	       ahrs_data_temp.yaw=atan2(2.0f * (q0 * q3 + q1 * q2), 1-2*(q2*q2+q3*q3));
	       ahrs_data_temp.pitch=asin(2.0f * (q0 * q2 - q3 * q1));
	       ahrs_data_temp.roll=atan2(2.0f * (q0 * q1 + q2 * q3), 1-2*(q1*q1+q2*q2));
	       ahrs_data_temp.pitch *= 180.0f / PI;
	       ahrs_data_temp.yaw   *= 180.0f / PI;

	       ahrs_data_temp.roll  *= 180.0f / PI;





	       atp_input_update_dof(ahrs_data->input_table,dof_data_temp);
	       atp_input_update_ahrs(ahrs_data->input_table,ahrs_data_temp);

	       printf("roll pitch yaw %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n",ahrs_data_temp.roll,ahrs_data_temp.pitch,ahrs_data_temp.yaw,ahrs_data_temp.pressure,ahrs_data_temp.temperature,ahrs_data_temp.altitude);


      }else{
    	  //todo:what will happen if error occures,continues too much
    	  //printf("ahrs system exception\n");
      }
}


inline em_int32 write_calibration_values(FILE *output, float *accel_values,float *mag_values,float *gyro_values,float *temperature,float *pressure){
	em_int32 err=ATP_SUCCESS;

	#ifdef COMPILE_LSM303
	       err |= adafruit_lsm303_accel_read_raw(accel_values);
	       err |= adafruit_lsm303_mag_read_raw(mag_values);

	#endif
	#ifdef COMPILE_L3GD20
	      err |= adafruit_l3gd20_gyro_read_raw(gyro_values);

	#endif
	#ifdef COMPILE_BMP085
	      err |= adafruit_bmp085_temp_press_read_raw(temperature,pressure);


	#endif



 fprintf(output,"%8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f\n",accel_values[0],accel_values[1],accel_values[2],mag_values[0],mag_values[1],mag_values[2],gyro_values[0],gyro_values[1],gyro_values[2],*pressure,*temperature);

	      return err;
}

void * start_communication_ahrs(void *data){
	atp_services_ahrs_data *ahrs_data=atp_convert(data,atp_services_ahrs_data*);
	em_uint32 err=ATP_SUCCESS;

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




#ifdef COMPILE_BMP085

	 	err=adafruit_bmp085_temp_pres_start(NULL);
	 		 	 if(err){
	 		 			 atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting BMP085 Temprature Pressure Failed Error:%u\n", err));
	 		 			 return (void*)ATP_ERROR_HARDWARE_INITIALIZE;
	 		 		 }

#endif


		em_int32 get_calibration_values=atp_settings_get_dof_calibration(ahrs_data->settings_table);


	 	FILE *output;
	 	output=fopen("dof.calibration","w");
	 	if(output==NULL){
	 		printf("could not open output dof.calibration file\n");
	 	}
	 	em_byte buffer[1024];





    em_float32 accel_values[3];
    em_float32 accel_bias_values[3];
    em_float32 accel_scale_values[3];
	em_float32 mag_values[3];
	em_float32 gyro_values[3];
	em_float32 temprature;
	em_float32 pressure;
	em_float32 altitude;
	em_float32 gravity_location=atp_settings_get_gravity(ahrs_data->settings_table);
	em_float32 sea_level_pressure_location=atp_settings_get_sea_level_pressure(ahrs_data->settings_table);
	em_int32 check_settings_counter=0;
	em_uint64 read_start,read_end,last_read=atp_datetime_as_microseconds();

	while(ahrs_data->work){
    read_start=atp_datetime_as_microseconds();
		err=ATP_SUCCESS;
      if(!get_calibration_values)
      err=do_ahrs(accel_values,accel_bias_values,accel_scale_values,  mag_values,gyro_values,&temprature,&pressure,&altitude,gravity_location,sea_level_pressure_location, &last_read, ahrs_data);
      else
    	 err= write_calibration_values(output,accel_values,mag_values,gyro_values,&temprature,&pressure);




      //sometimes reload some values from settings again
      if(check_settings_counter%10000==0){
    	  gravity_location=atp_settings_get_gravity(ahrs_data->settings_table);
    	  sea_level_pressure_location=atp_settings_get_sea_level_pressure(ahrs_data->settings_table);
    	  accel_bias_values[0]=atp_settings_get_acceleration_biasx(ahrs_data->settings_table);
    	  accel_bias_values[1]=atp_settings_get_acceleration_biasy(ahrs_data->settings_table);
    	  accel_bias_values[2]=atp_settings_get_acceleration_biasz(ahrs_data->settings_table);
    	  accel_scale_values[0]=atp_settings_get_acceleration_scalex(ahrs_data->settings_table);
    	  accel_scale_values[1]=atp_settings_get_acceleration_scaley(ahrs_data->settings_table);
    	  accel_scale_values[2]=atp_settings_get_acceleration_scalez(ahrs_data->settings_table);
    	  check_settings_counter=0;
      }
      check_settings_counter++;

      em_int32 valoftime=1000000/25-(atp_datetime_as_microseconds()-read_start);
      if(valoftime>0)
      em_io_delay_microseconds(valoftime);
	}

	if(output!=NULL)
		fclose(output);

	return ATP_SUCCESS;
}


em_uint32 atp_services_ahrs_create(atp_services_ahrs **address,atp_input *input,atp_settings *settings){
  em_uint32 err;
  atp_services_ahrs *ahrs=atp_new(atp_services_ahrs);
  atp_fill_zero(ahrs,sizeof(atp_services_ahrs));
  atp_services_ahrs_data *ahrs_data=atp_new(atp_services_ahrs_data);
  atp_fill_zero(ahrs_data,sizeof(atp_services_ahrs_data));

  ahrs->private_data=ahrs_data;


  ahrs_data->work=1;
  ahrs_data->input_table=input;
  ahrs_data->settings_table=settings;
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

