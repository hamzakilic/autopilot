/*
 * atp_pilot.h
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#ifndef ATP_PILOT_H_
#define ATP_PILOT_H_

#include "../common/atp_common.h"
#include "../input/atp_input.h"
#include "../services/atp_service_system.h"
#include "../controllers/atp_motor_controller.h"
#include "../log/atp_log.h"
#include "atp_command_manager.h"
#include "atp_command_listener.h"


typedef struct
{
	void *private_data;
}atp_pilot;

em_uint32 atp_pilot_create(atp_pilot **pilot);
em_uint32 atp_pilot_start(atp_pilot *pilot);
em_uint32 atp_pilot_stop(atp_pilot *pilot);
em_uint32 atp_pilot_destroy(atp_pilot *pilot);

#endif /* ATP_PILOT_H_ */
