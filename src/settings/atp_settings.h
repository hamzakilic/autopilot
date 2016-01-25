/*
 * atp_settings.h
 *
 *  Created on: Jan 19, 2016
 *      Author: root
 */

#ifndef ATP_SETTINGS_H_
#define ATP_SETTINGS_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"

/**
 * @brief system settings table
 * forexample gravity of location
 *
 */

typedef struct{
	void *private_data;
}atp_settings;

em_uint32 atp_settings_create(atp_settings **settings);
em_uint32 atp_settings_destroy(atp_settings *settings);

/**
 * @brief returns default gravitiy for location
 * return gravity or if settings or private_data is null returns -1
 */
em_float32 atp_settings_get_gravity(atp_settings *settings);


/**
 * @brief for making dof calibration
 * @returns >0 if making dof calibration else returns 0
 *
 */
em_int32 atp_settings_get_dof_calibration(atp_settings *settings);


/**
 * @brief get sea level pressure hPa
 * @returns pressure in hPa, returns -1 if settings or ... is null
 */
em_float32 atp_settings_get_sea_level_pressure(atp_settings *settings);






#endif /* ATP_SETTINGS_H_ */