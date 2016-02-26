/*
 * atp_task_share.h
 *
 *  Created on: Feb 26, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_ATP_TASK_SHARE_H_
#define AUTOPILOTC_SRC_ATP_TASK_SHARE_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"
#include "../common/atp_queue.h"

typedef struct
{
	void *private;
}atp_task_share;

em_uint32 atp_task_share_create(atp_task_share **task_share);
em_uint32 atp_task_share_destroy(atp_task_share *task_share);
em_int32 atp_task_share_count_get(atp_task_share *task_share);
void atp_task_share_count_plus(atp_task_share *task_share);
void atp_task_share_count_minus(atp_task_share *task_share);
em_int32 atp_task_share_killall_get(atp_task_share *task_share);
void atp_task_share_killall_set(atp_task_share *task_share,em_int32 killall);




#endif /* ATP_TASK_SHARE_H_ */
