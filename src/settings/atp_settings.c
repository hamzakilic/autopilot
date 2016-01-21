/*
 * atp_settings.c
 *
 *  Created on: Jan 19, 2016
 *      Author: root
 */

#include "atp_settings.h"

typedef struct{

	void * gravity_lock;
	em_float32 gravity;

	void * dof_calibration_lock;
	em_int32 dof_calibration;

	em_float32 sea_level_pressure;
	void *sea_level_pressure_lock;
}atp_settings_data;


em_uint32 atp_settings_create(atp_settings **address){

	atp_settings *settings=atp_new(atp_settings);
	atp_fill_zero(settings,sizeof(atp_settings));

	atp_settings_data *data=atp_new(atp_settings_data);
	atp_fill_zero(data,sizeof(atp_settings_data));
	settings->private_data=data;
	data->gravity=9.8029f;
	data->dof_calibration=1;
	data->sea_level_pressure=1023.1f;
	atp_thread_create_lock(&data->gravity_lock);
	atp_thread_create_lock(&data->dof_calibration_lock);
	atp_thread_create_lock(&data->sea_level_pressure_lock);


	*address=settings;
	return ATP_SUCCESS;


}
em_uint32 atp_settings_destroy(atp_settings *settings){
	if(settings){
		if(settings->private_data){
			atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
			atp_thread_destory_lock(data->gravity_lock);
			atp_thread_destory_lock(data->dof_calibration_lock);
			atp_thread_destory_lock(data->sea_level_pressure_lock);
			atp_free(data);

		}
		atp_free(settings);
	}
	return ATP_SUCCESS;

}



em_float32 atp_settings_get_gravity(atp_settings *settings){
	if(settings)
		if(settings->private_data){
			atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
			atp_thread_lock(data->gravity_lock);
			em_float32 val=data->gravity;
			atp_thread_unlock(data->gravity_lock);
			return val;
		}
	return -1;
}


em_int32 atp_settings_get_dof_calibration(atp_settings *settings){
	if(settings)
			if(settings->private_data){
				atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
				atp_thread_lock(data->dof_calibration_lock);
				em_int32 val=data->dof_calibration;
				atp_thread_unlock(data->dof_calibration_lock);
				return val;
			}
		return -1;
	}

em_float32 atp_settings_get_sea_level_pressure(atp_settings *settings){
	if(settings)
				if(settings->private_data){
					atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
					atp_thread_lock(data->sea_level_pressure_lock);
					em_float32 val=data->sea_level_pressure;
					atp_thread_unlock(data->sea_level_pressure_lock);
					return val;
				}
			return -1;

}


