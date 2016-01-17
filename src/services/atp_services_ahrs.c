/*
 * atp_services_ahrs.c
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */
#include "atp_services_ahrs.h"

#define WRITE_OUTPUTS_TO_FILE2

typedef struct {
   em_int32 work;
   atp_thread_id thread_id;
   atp_input *input_table;


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

#ifdef WRITE_OUTPUTS_TO_FILE
	 	FILE *output;
	 	output=fopen("output.values","w");
	 	if(output==NULL){
	 		printf("could not open output file\n");
	 	}
	 	em_byte buffer[1024];

#endif



    em_float32 accel_values[3];
	em_float32 mag_values[3];
	em_float32 gyro_values[3];
	em_float32 temprature;
	em_float32 pressure;
	em_float32 altitude;
	em_uint64 read_start,read_end,last_read=atp_datetime_as_microseconds();
	while(ahrs_data->work){
    read_start=atp_datetime_as_microseconds();
		err=ATP_SUCCESS;
#ifdef COMPILE_LSM303
      err |= adafruit_lsm303_accel_read(accel_values);
      err |= adafruit_lsm303_mag_read(mag_values);

#endif
#ifdef COMPILE_L3GD20
      err |= adafruit_l3gd20_gyro_read(gyro_values);

#endif
#ifdef COMPILE_BMP085
      err |= adafruit_bmp085_temp_press_read(&temprature,&pressure);
      altitude=pressure_to_altitude(1008.4,pressure);

#endif


#ifdef WRITE_OUTPUTS_TO_FILE
      fprintf(output,"%f %f %f %f %f %f %f %f %f %f %f\n",accel_values[0],accel_values[1],accel_values[2],mag_values[0],mag_values[1],mag_values[2],gyro_values[0],gyro_values[1],gyro_values[2],pressure,temprature);
#endif


      if(!err){
#define PI 3.14159f
    	  atp_dof_data dof_data_temp;
    	  dof_data_temp.accx=accel_values[0];
    	  dof_data_temp.accy=accel_values[1];
    	  dof_data_temp.accz=accel_values[2];

    	  dof_data_temp.gyrox=gyro_values[0];
    	  dof_data_temp.gyroy=gyro_values[1];
    	  dof_data_temp.gyroz=gyro_values[2];

    	  dof_data_temp.magx=mag_values[0];
    	  dof_data_temp.magy=mag_values[1];
    	  dof_data_temp.magz=mag_values[2];

      em_uint64 delta_t=atp_datetime_as_microseconds()- last_read;
      //fprintf(stdout,"%f %f %f %f %f %f %f %f %f %f %f\n",accel_values[0],accel_values[1],accel_values[2],mag_values[0],mag_values[1],mag_values[2],gyro_values[0],gyro_values[1],gyro_values[2],pressure,temprature);
      MahonyAHRSupdate(dof_data_temp.gyrox*PI/180.0f,dof_data_temp.gyroy*PI/180.0f,dof_data_temp.gyroz*PI/180.0f,dof_data_temp.accx,dof_data_temp.accy,dof_data_temp.accz,dof_data_temp.magx,dof_data_temp.magy,dof_data_temp.magz);

      last_read=atp_datetime_as_microseconds();


	   atp_ahrs_data ahrs_data_temp;
	   ahrs_data_temp.altitude=altitude;
	   ahrs_data_temp.temperature=temprature;



	   // Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
	     // In this coordinate system, the positive z-axis is down toward Earth.
	     // Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
	     // Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
	     // Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
	     // These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
	     // Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
	     // applied in the correct order which for this configuration is yaw, pitch, and then roll.
	     // For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
	      /*yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
	       pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
	       roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);*/
	       ahrs_data_temp.yaw=atan2(2.0f * (q0 * q3 + q1 * q2), 1-2*(q2*q2+q3*q3));
	       ahrs_data_temp.pitch=-asin(2.0f * (q0 * q2 - q3 * q1));
	       ahrs_data_temp.roll=atan2(2.0f * (q0 * q1 + q2 * q3), 1-2*(q1*q1+q2*q2));
	       ahrs_data_temp.pitch *= 180.0f / PI;
	       ahrs_data_temp.yaw   *= 180.0f / PI;
	       //yaw   -= 13.8; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
	       ahrs_data_temp.roll  *= 180.0f / PI;





	       atp_input_update_dof(ahrs_data->input_table,dof_data_temp);
	       atp_input_update_ahrs(ahrs_data->input_table,ahrs_data_temp);

	       printf("roll pitch yaw %f %f %f %f %f %f %f\n",ahrs_data_temp.roll,ahrs_data_temp.pitch,ahrs_data_temp.yaw,q0,q1,q2,q3);


      }else{
    	  //todo:what will happen if error occures,continues too much
    	  printf("ahrs system exception\n");
      }

    //  printf("accel gyro mag values %f %f %f # %f %f %f # %f %f %f\n",accel_values[0],accel_values[1],accel_values[2],mag_values[0],mag_values[1],mag_values[2],gyro_values[0],gyro_values[1],gyro_values[2]);
     // printf("temp pressure altitude values %f %f %f \n",temprature,pressure,altitude);
      em_int32 valoftime=1000000/25-(atp_datetime_as_microseconds()-read_start);
      //printf("time:%d %d\n ",valoftime,1000000/10-valoftime);
      if(valoftime>0)
      em_io_delay_microseconds(1000000/25-valoftime);
	}
#ifdef WRITE_OUTPUTS_TO_FILE
	if(output!=NULL)
		fclose(output);
#endif
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

