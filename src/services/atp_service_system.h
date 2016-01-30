/*
 * atp_service_system.h
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_SERVICES_ATP_SERVICE_SYSTEM_H_
#define AUTOPILOTC_SRC_SERVICES_ATP_SERVICE_SYSTEM_H_
#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "../settings/atp_settings.h"
#include "atp_services_gps.h"
#include "atp_services_ahrs.h"

em_uint32  atp_service_system_start(atp_input *input,atp_settings *settings);
em_uint32 atp_service_system_stop();


#endif /* AUTOPILOTC_SRC_SERVICES_ATP_SERVICE_SYSTEM_H_ */
