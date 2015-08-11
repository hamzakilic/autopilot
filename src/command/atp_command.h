/*
 * atp_command.h
 *
 *  Created on: Jun 24, 2015
 *      Author: root
 */

#ifndef ATP_COMMAND_H_
#define ATP_COMMAND_H_

#include "../common/atp_common.h"


typedef void (*destroy_command_func )(void *);
typedef struct {
    em_uint16 type;
    void *data;
    destroy_command_func destroy;
}atp_command;


typedef struct {
	em_byte *data;
}atp_command_test;

typedef struct{
  em_uint16 values[4];
}atp_command_motor;

typedef struct{

}atp_command_camera_control;


#define ATP_COMMAND_TEST 1
#define ATP_COMMAND_MOTOR 2

#endif /* ATP_COMMAND_H_ */
