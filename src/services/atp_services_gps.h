/*
 * atp_services_gps.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_SERVICES_ATP_SERVICES_GPS_H_
#define AUTOPILOTC_SRC_SERVICES_ATP_SERVICES_GPS_H_
#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "../thread/atp_thread.h"
#include "../common/atp_queue.h"
#include "../modules/module_UBlox_neo6v.h"

typedef struct {
    void * private_data;

}atp_services_gps;

em_uint32 atp_services_gps_create(atp_services_gps **address,atp_input *input);
em_uint32 atp_services_gps_destroy(atp_services_gps *address);

#endif /* AUTOPILOTC_SRC_SERVICES_ATP_SERVICES_GPS_H_ */
