/*
 * atp_settings.h
 *
 *  Created on: Jan 19, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_SETTINGS_ATP_SETTINGS_H_
#define AUTOPILOTC_SRC_SETTINGS_ATP_SETTINGS_H_

#include "../common/atp_common.h"
#include "../thread/atp_thread.h"

/**
 * @brief system settings table
 * forexample gravity of location
 * or acceleration standart deviations
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


/**
 * @brief get acceleration  bias  values
 */
em_float32 atp_settings_get_acceleration_biasx(atp_settings *settings);
em_float32 atp_settings_get_acceleration_biasy(atp_settings *settings);
em_float32 atp_settings_get_acceleration_biasz(atp_settings *settings);

/**
 * @brief get accerelation scale values
 */
em_float32 atp_settings_get_acceleration_scalex(atp_settings *settings);
em_float32 atp_settings_get_acceleration_scaley(atp_settings *settings);
em_float32 atp_settings_get_acceleration_scalez(atp_settings *settings);



/**
 * @brief get gyroscope  bias  values
 */
em_float32 atp_settings_get_gyroscope_biasx(atp_settings *settings);
em_float32 atp_settings_get_gyroscope_biasy(atp_settings *settings);
em_float32 atp_settings_get_gyroscope_biasz(atp_settings *settings);

/**
 * @brief get gyroscope scale values
 */
em_float32 atp_settings_get_gyroscope_scalex(atp_settings *settings);
em_float32 atp_settings_get_gyroscope_scaley(atp_settings *settings);
em_float32 atp_settings_get_gyroscope_scalez(atp_settings *settings);


/**
 * @brief get roll pitch yaw bias values
 */
em_float32 atp_settings_get_roll_bias(atp_settings *settings);
em_float32 atp_settings_get_pitch_bias(atp_settings *settings);
em_float32 atp_settings_get_yaw_bias(atp_settings *settings);







#endif /* AUTOPILOTC_SRC_SETTINGS_ATP_SETTINGS_H_ */
