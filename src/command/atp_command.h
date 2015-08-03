/*
 * atp_command.h
 *
 *  Created on: Jun 24, 2015
 *      Author: root
 */

#ifndef ATP_COMMAND_H_
#define ATP_COMMAND_H_

#include "../common/atp_common.h"


typedef struct {
    em_uint16 type;
    void *data;
}atp_command;


#define ATP_COMMAND_TEST 1

#endif /* ATP_COMMAND_H_ */
