/*
 * atp_input_table.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_INPUT_ATP_INPUT_H_
#define AUTOPILOTC_SRC_INPUT_ATP_INPUT_H_
#include "../common/atp_common.h"
#include "../thread/atp_thread.h"
#include "../log/atp_log.h"


typedef struct {
	em_uint16 year;
	em_uint8 month;
	em_uint8 day;
	em_uint8 hour;
	em_uint8 min;
	em_uint8 sec;
}atp_gps_time_data;

typedef struct{
	/**
	 * longtitude value as degrees
	 */
	em_int32 longtitude;
	/**
	 * latitude value as degrees;
	 */
	em_int32 latitude;
	/**
	 * sea level as milimeters
	 */
	em_int32 sealevel;


}atp_gps_location_data;

typedef struct{
	/**
	 * 0x00 = no fix
	 * 0x01 = dead reckoning only
	 * 0x02 = 2D-fix
	 * 0x03 = 3D-fix
	 * 0x04 = GPS + dead reckoning combined
	 * 0x05 = Time only fix
	 */
	em_uint8 gps_fix;

	/**
	 * if zero gpsFixOK
	 */
	em_uint8 flags;

	/**
	 * milisecond since startuup or reset
	 */
	em_uint32 msss;
}atp_gps_location_ex_data;



typedef struct{
	em_float32 roll;
	em_float32 pitch;
	em_float32 yaw;
	em_float32 altitude;
	em_float32 pressure;
	em_float32 temperature;

}atp_ahrs_data;

typedef struct{

    em_float32 accx;
    em_float32 accy;
    em_float32 accz;

    em_float32 gyrox;
    em_float32 gyroy;
    em_float32 gyroz;

    em_float32 magx;
    em_float32 magy;
    em_float32 magz;


}atp_dof_data;





typedef struct{
 em_uint8 motor_index;
 em_int16 motor_value;
}atp_motor_data;



typedef struct {
    void *private_data;
}atp_input;


em_uint32 atp_input_create(atp_input **address);
em_uint32 atp_input_destroy(atp_input *address);
em_uint32 atp_input_update_gps_time(atp_input *input, atp_gps_time_data  data);
em_uint32 atp_input_update_gps_location(atp_input *input, atp_gps_location_data  data);
em_uint32 atp_input_update_gps_location_ex(atp_input *input, atp_gps_location_ex_data  data);
em_uint32 atp_input_update_motor(atp_input *input, atp_motor_data  data);
em_uint32 atp_input_update_dof(atp_input *input,atp_dof_data data);
em_uint32 atp_input_update_ahrs(atp_input *input,atp_ahrs_data data);
em_uint32 atp_input_get_motor(atp_input *input,em_uint8 motor_index,atp_motor_data *data);
em_uint32 atp_input_get_motors(atp_input *input,atp_motor_data *data);
em_uint32 atp_input_get_ahrs(atp_input *input,atp_ahrs_data *data);




#endif /* AUTOPILOTC_SRC_INPUT_ATP_INPUT_H_ */
