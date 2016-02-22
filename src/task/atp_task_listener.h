/*
 * atp_task_listener.h
 *
 *  Created on: Feb 21, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_TASK_ATP_TASK_LISTENER_H_
#define AUTOPILOTC_SRC_TASK_ATP_TASK_LISTENER_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"
#include "../common/atp_queue.h"
#include "../log/atp_log.h"
#include "atp_task_manager.h"

typedef struct{
    void *private_data;
}atp_task_listener;

em_uint32 atp_task_listener_create(atp_task_listener **task_listener,atp_task_manager *task_manager);
em_uint32 atp_task_listener_destroy(atp_task_listener *task_listener);



#endif /* ATP_TASK_LISTENER_H_ */
