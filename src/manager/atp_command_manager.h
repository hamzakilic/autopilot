/*
 * atp_command_controller.h
 *
 *  Created on: Jun 24, 2015
 *      Author: root
 */

#ifndef ATP_COMMAND_MANAGER_H_
#define ATP_COMMAND_MANAGER_H_

#include "../common/atp_common.h"
#include "../common/atp_queue.h"
#include "../command/atp_command.h"
#include "../thread/atp_thread.h"

typedef struct {
    void *private_data;
}atp_command_manager;

em_uint32  atp_command_manager_create(atp_command_manager ** controller);
em_uint32  atp_command_manager_destroy(atp_command_manager * controller);
em_uint32  atp_command_manager_add(atp_command command,atp_command_manager *to);


#endif /* ATP_COMMAND_MANAGER_H_ */
