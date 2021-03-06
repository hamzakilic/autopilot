/*
 * atp_common.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_COMMON_ATP_COMMON_H_
#define AUTOPILOTC_SRC_COMMON_ATP_COMMON_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <em_io.h>
#include <em_error.h>
#include <em_types.h>
#include <em_common.h>


#define COMPILE_TEST_CODES2
#define COMPILE_DEBUG_CODES2

#define COMPILE_MODULE_GPS_NEO_6V
#define COMPILE_A2212_MOTORS_AND_ESC
#define COMPILE_LOG_UDP
#define COMPILE_LSM303
#define COMPILE_L3GD20
#define COMPILE_BMP085
#define COMPILE_PCA9685

#define ATP_SUCCESS 0x00

#define ATP_ERROR_CREATE_GPS 0x01
#define ATP_ERROR_CREATE_INPUT 0x02
#define ATP_ERROR_START_SERVICE_SYSTEM 0x04
#define ATP_ERROR_HARDWARE_INITIALIZE 0x08
#define ATP_ERROR_CREATE_THREAD 0x10
#define ATP_ERROR_CREATE_THREAD_MUTEX 0x20
#define ATP_ERROR_HARDWARE_I2C_START 0x40
#define ATP_ERROR_HARDWARE_COMMUNICATION 0x80
#define ATP_ERROR_HARDWARE_UART_START 0x100
#define ATP_ERROR_START_MOTOR_CONTROLLER_SYSTEM 0x200
#define ATP_ERROR_PILOT_CREATED_ALLREADY 0x400
#define ATP_ERROR_START_COMMANDMANAGER 0x800
#define ATP_ERROR_START_COMMANDLISTENER 0x1000
#define ATP_ERROR_MUTEX_LOCK 0x2000
#define ATP_ERROR_MUTEX_UNLOCK 0x4000
#define ATP_ERROR_MUTEX_DESTROY 0x8000
#define ATP_ERROR_CREATE_AHRS 0x10000
#define ATP_ERROR_CREATE_SOCKET 0x20000






#define atp_malloc(a)   malloc(a)
#define atp_free(a)   free(a)
#define atp_fill_zero(src,size) memset(src,0,size)
#define atp_copy(dst,src,size) memcpy(dst,src,size)
#define atp_new(a)  malloc(sizeof(a))
#define atp_convert(src,dst) ((dst)src)










#endif /* AUTOPILOTC_SRC_COMMON_ATP_COMMON_H_ */
