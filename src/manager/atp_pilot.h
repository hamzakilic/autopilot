/*
 * atp_pilot.h
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_MANAGER_ATP_PILOT_H_
#define AUTOPILOTC_SRC_MANAGER_ATP_PILOT_H_

#include "../common/atp_common.h"
#include "../input/atp_input.h"
#include "../common/atp_system_lock.h"
#include "../settings/atp_settings.h"
#include "../services/atp_service_system.h"
#include "../controllers/atp_motor_controller.h"
#include "../log/atp_log.h"
#include "../task/atp_task_manager.h"
#include "../task/atp_task_listener.h"





typedef struct
{
	void *private_data;
}atp_pilot;

em_uint32 atp_pilot_create(atp_pilot **pilot);
em_uint32 atp_pilot_start(atp_pilot *pilot);
em_uint32 atp_pilot_stop(atp_pilot *pilot);
em_uint32 atp_pilot_destroy(atp_pilot *pilot);

#endif /* AUTOPILOTC_SRC_MANAGER_ATP_PILOT_H_ */
