/*
 * atp_task_manager.h
 *
 *  Created on: Feb 21, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_TASK_ATP_TASK_MANAGER_H_
#define AUTOPILOTC_SRC_TASK_ATP_TASK_MANAGER_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"
#include "../common/atp_queue.h"
#include "../log/atp_log.h"
#include "atp_task.h"
#include "../input/atp_input.h"
#include "../controllers/atp_motor_controller.h"
#include "../settings/atp_settings.h"





typedef struct{
    void *private_data;
}atp_task_manager;

em_uint32 atp_task_manager_create(atp_task_manager **task_manager,atp_input *input,atp_motor_controller *motor_controller,atp_settings *settings);
em_uint32 atp_task_manager_destroy(atp_task_manager *task_manager);
em_uint32 atp_task_manager_add_task(em_uint8 *data,em_uint32 data_length,atp_task_manager *task_manager);



#endif /* ATP_TASK_MANAGER_H_ */
