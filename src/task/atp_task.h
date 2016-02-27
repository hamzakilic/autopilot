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
#include "../task/atp_task_share.h"





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
#define ATP_TASK_EMERGENCY 0x0002
#define ATP_TASK_START_MOTORS 0x0003
#define ATP_TASK_STOP_MOTORS 0x0004
#define ATP_TASK_TAKEOFF 0x0005
#define ATP_TASK_MOTORCALIBRATE 0x0006
#define ATP_TASK_MOTORVALUE 0x0007





#endif /* ATP_BASE_TASK_H_ */
