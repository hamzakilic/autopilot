/*
 * atp_common.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef ATP_COMMON_H_
#define ATP_COMMON_H_

#include <em_io.h>
#include <em_error.h>
#include <em_types.h>
#include <string.h>
#include <stdlib.h>

#define COMPILE_MODULE_GPS_NEO_6V
#define COMPILE_A2212_MOTORS_AND_ESC
#define COMPILE_LOG_UDP

#define ATP_SUCCESS 0x00

#define ATP_ERROR_CREATE_GPS 0x01
#define ATP_ERROR_CREATE_INPUT 0x02
#define ATP_ERROR_START_SERVICE_SYSTEM 0x04
#define ATP_ERROR_HARDWARE_INITIALIZE 0x08
#define ATP_ERROR_CREATE_THREAD 0x10
#define ATP_ERROR_CREATE_THREAD_MUTEX 0x20
#define ATP_ERROR_HARDWARE_I2C_START 0x40
#define ATP_ERROR_HARDWARE_COMMUNICATION 0X80
#define ATP_ERROR_HARDWARE_UART_START 0x40
#define ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM 0x80
#define ATP_ERROR_PILOT_CREATED_ALLREADY 0x100
#define ATP_ERROR_START_COMMANDMANAGER 0x200
#define ATP_ERROR_START_COMMANDLISTENER 0X400





#define atp_malloc(a)   malloc(a)
#define atp_free(a)   free(a)




typedef struct {

}gps;

typedef struct{

}gyro;



#endif /* ATP_COMMON_H_ */
