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

	//no need to create a thread lock
	//it will change sometimes
	em_float32  acceleration_biasx;
	em_float32  acceleration_biasy;
	em_float32  acceleration_biasz;
	//no need to create a thread lock
		//it will change sometimes
	em_float32  acceleration_scalex;
	em_float32  acceleration_scaley;
	em_float32  acceleration_scalez;


	//no need to create a thread lock
		//it will change sometimes
		em_float32  gyroscope_biasx;
		em_float32  gyroscope_biasy;
		em_float32  gyroscope_biasz;
		//no need to create a thread lock
			//it will change sometimes
		em_float32  gyroscope_scalex;
		em_float32  gyroscope_scaley;
		em_float32  gyroscope_scalez;

		//no need to create a thread lock
	    //it will change sometimes
		em_float32 roll_bias;
		em_float32 pitch_bias;
		em_float32 yaw_bias;




}atp_settings_data;


em_uint32 atp_settings_create(atp_settings **address){

	atp_settings *settings=atp_new(atp_settings);
	atp_fill_zero(settings,sizeof(atp_settings));

	atp_settings_data *data=atp_new(atp_settings_data);
	atp_fill_zero(data,sizeof(atp_settings_data));
	settings->private_data=data;
	data->gravity=9.8029f;
	data->dof_calibration=0;
	data->sea_level_pressure=1022.1f;
	data->acceleration_biasx=0;//1.4932f;
	data->acceleration_biasy=0;//1.3905f;
	data->acceleration_biasz=0;//2.1590f;
	data->acceleration_scalex=1.0f;//2048.0f/(1028+1100);
	data->acceleration_scaley=1.0f;//2048.0f/(1032+1048);
	data->acceleration_scalez=1.0f;//2048.0f/(1016+1092);
	data->gyroscope_biasx=0;//1.5162f;
	data->gyroscope_biasy=0;//1.5423f;
	data->gyroscope_biasz=0;//1.8041f;
	data->gyroscope_scalex=1.0f;//28571.4f*2/(19136+18446);
	data->gyroscope_scaley=1.0f;//32768.0f*2/(19136+18446);
	data->gyroscope_scalez=1.0f;//32768.0f*2/(19136+18171);
	data->roll_bias=0.20f;
	data->pitch_bias=2.55f;
	data->yaw_bias=0.0f;


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


em_float32 atp_settings_get_acceleration_biasx(atp_settings *settings){
	if(settings)
				if(settings->private_data){
					atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
					return data->acceleration_biasx;
				}
			return -1;

}
em_float32 atp_settings_get_acceleration_biasy(atp_settings *settings){
	if(settings)
					if(settings->private_data){
						atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
						return data->acceleration_biasy;
					}
				return -1;

}
em_float32 atp_settings_get_acceleration_biasz(atp_settings *settings){
	if(settings)
					if(settings->private_data){
						atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
						return data->acceleration_biasz;
					}
				return -1;
}

em_float32 atp_settings_get_acceleration_scalex(atp_settings *settings){
	if(settings)
						if(settings->private_data){
							atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
							return data->acceleration_scalex;
						}
					return -1;
}
em_float32 atp_settings_get_acceleration_scaley(atp_settings *settings){
	if(settings)
							if(settings->private_data){
								atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
								return data->acceleration_scaley;
							}
						return -1;

}
em_float32 atp_settings_get_acceleration_scalez(atp_settings *settings){
	if(settings)
							if(settings->private_data){
								atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
								return data->acceleration_scalez;
							}
						return -1;

}



em_float32 atp_settings_get_gyroscope_biasx(atp_settings *settings){
	if(settings)
								if(settings->private_data){
									atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
									return data->gyroscope_biasx;
								}
							return -1;

}
em_float32 atp_settings_get_gyroscope_biasy(atp_settings *settings){
	if(settings)
									if(settings->private_data){
										atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
										return data->gyroscope_biasy;
									}
								return -1;

}
em_float32 atp_settings_get_gyroscope_biasz(atp_settings *settings){
	if(settings)
									if(settings->private_data){
										atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
										return data->gyroscope_biasz;
									}
								return -1;

}


em_float32 atp_settings_get_gyroscope_scalex(atp_settings *settings){
	if(settings)
									if(settings->private_data){
										atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
										return data->gyroscope_scalex;
									}
								return -1;


}
em_float32 atp_settings_get_gyroscope_scaley(atp_settings *settings){
	if(settings)
									if(settings->private_data){
										atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
										return data->gyroscope_scaley;
									}
								return -1;

}
em_float32 atp_settings_get_gyroscope_scalez(atp_settings *settings){
	if(settings)
									if(settings->private_data){
										atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
										return data->gyroscope_scalez;
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


em_float32 atp_settings_get_roll_bias(atp_settings *settings){
	if(settings)
		if(settings->private_data){
	 	atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
			return data->roll_bias;
		}
	return 0.0f;

}
em_float32 atp_settings_get_pitch_bias(atp_settings *settings){
	if(settings)
		if(settings->private_data){
	 	atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
			return data->pitch_bias;
		}
	return 0.0f;

}
em_float32 atp_settings_get_yaw_bias(atp_settings *settings){
	if(settings)
		if(settings->private_data){
	 	atp_settings_data *data=atp_convert(settings->private_data,atp_settings_data*);
			return data->yaw_bias;
		}
	return 0.0f;

}






