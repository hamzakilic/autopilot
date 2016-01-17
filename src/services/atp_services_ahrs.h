/*
 * atp_services_ahrs.h
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */

#ifndef ATP_SERVICES_AHRS_H_
#define ATP_SERVICES_AHRS_H_

#include <math.h>
#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "../input/atp_input.h"
#include "../thread/atp_thread.h"
#include "../common/atp_queue.h"
#include "../common/atp_time.h"
#include "../modules/module_Adafruit_LSM303.h"
#include "../modules/module_Adafruit_L3GD20.h"
#include "../modules/module_Adafruit_BMP085.h"
#include "../algorithms/MahonyAHRS.h"





typedef struct {
    void * private_data;

}atp_services_ahrs;

em_uint32 atp_services_ahrs_create(atp_services_ahrs **address,atp_input *input);
em_uint32 atp_services_ahrs_destroy(atp_services_ahrs *address);


#endif /* ATP_SERVICES_AHRS_H_ */
