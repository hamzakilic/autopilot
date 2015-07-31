/*
 * atp_service_system.h
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#ifndef ATP_SERVICE_SYSTEM_H_
#define ATP_SERVICE_SYSTEM_H_
#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "atp_services_gps.h"

em_uint32  atp_service_system_start(atp_input *input);
em_uint32 atp_service_system_stop();


#endif /* ATP_SERVICE_SYSTEM_H_ */
