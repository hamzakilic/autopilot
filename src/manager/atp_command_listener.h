/*
 * atp_command_listener.h
 *
 *  Created on: Jun 26, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_MANAGER_ATP_COMMAND_LISTENER_H_
#define AUTOPILOTC_SRC_MANAGER_ATP_COMMAND_LISTENER_H_



#include "../common/atp_common.h"
#include "atp_command_manager.h"

typedef struct{
    void *private_data;
}atp_command_listener;

em_uint32 atp_command_listener_create(atp_command_listener **command_listener,atp_command_manager *command_manager);
em_uint32 atp_command_listener_destroy(atp_command_listener *command_listener);

#endif /* AUTOPILOTC_SRC_MANAGER_ATP_COMMAND_LISTENER_H_ */
