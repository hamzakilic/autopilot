/*
 * atp_input_table.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef ATP_INPUT_H_
#define ATP_INPUT_H_
#include "../common/atp_common.h"
#include "../log/atp_log.h"


typedef struct {

}atp_gps_data;

typedef struct{

}atp_gyro_data;

typedef struct{
 em_uint8 motor_index;
 em_uint16 motor_value;
}atp_motor_data;



typedef struct {
    void *private_data;
}atp_input;


em_uint32 atp_input_create(atp_input **address);
em_uint32 atp_input_destroy(atp_input *address);
em_uint32 atp_input_update_gps(atp_input *input, const atp_gps_data * const data);
em_uint32 atp_input_update_motor(atp_input *input,const atp_motor_data * const data);




#endif /* ATP_INPUT_H_ */
