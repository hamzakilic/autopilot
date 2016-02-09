/*
 * atp_log.h
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_LOG_ATP_LOG_H_
#define AUTOPILOTC_SRC_LOG_ATP_LOG_H_


#include <stdarg.h>
#include <em_types.h>
#include <stdio.h>
#include "../common/atp_common.h"

#define ATP_LOG_INFO 0x01
#define ATP_LOG_ERROR 0x02
#define ATP_LOG_FATAL 0x04
#define ATP_LOG_DATA 0x08
#define ATP_LOG_DEBUG 0x10


#define ATP_LOG_DATA_TYPE_STRING 0x01
#define ATP_LOG_DATA_TYPE_INPUT 0x02


typedef struct{
	em_uint8 type;
	em_uint16 data_type;// data type
	em_uint8 data_len;
	em_uint8 *data;
}atp_log_data;



void atp_log(atp_log_data *data);
void atp_log_raw(atp_log_data *data);
//helper functions

atp_log_data* atp_log_create_string(em_uint8 log_type, const char *fmt,...);

#endif /* AUTOPILOTC_SRC_LOG_ATP_LOG_H_ */
