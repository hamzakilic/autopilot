/*
 * atp_base_task.h
 *
 *  Created on: Feb 21, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_TASK_ATP_TASK_H_
#define AUTOPILOTC_SRC_TASK_ATP_TASK_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"






typedef void* (*task_exec_func)(void *parameter);
typedef void (*task_free_func)(void *parameter);
typedef void (*task_kill_func)(void *parameter);


 struct atp_task {
    em_int32 is_finished;
    em_int32 is_started;
	em_int32 task_kill;
    const em_int8 *name;
	void * parameter;
	task_exec_func func_exec;
	task_free_func func_free;
	task_kill_func func_kill;
	struct atp_task  ** next_task_list;
	em_uint32 next_task_list_count;
    atp_thread_id thread_id;
};

#define ATP_TASK_ECHO 0x0001




#endif /* ATP_BASE_TASK_H_ */
