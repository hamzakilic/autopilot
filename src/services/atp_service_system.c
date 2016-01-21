/*
 * atp_service_system.c
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#include "atp_service_system.h"

static atp_services_gps *service_gps=0;
static atp_services_ahrs *service_ahrs=0;
em_uint32  atp_service_system_start(atp_input *input,atp_settings *settings){
	em_uint32 err;
	if((err=atp_services_gps_create(&service_gps,input)))
	{
		atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Gps Failed Error:%u\n",err));
       return ATP_ERROR_CREATE_GPS;
	}else{
		atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Gps Success \n"));
	}

	if((err=atp_services_ahrs_create(&service_ahrs,input,settings)))
		{
			atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create AHRS Failed Error:%u\n",err));
	       return ATP_ERROR_CREATE_GPS;
		}else{
			atp_log(atp_log_create_string(ATP_LOG_INFO,"Create AHRS Success \n"));
		}

	return ATP_SUCCESS;

}
em_uint32 atp_service_system_stop(){

	if(service_gps)
	atp_services_gps_destroy(service_gps);
	if(service_ahrs)
		atp_services_ahrs_destroy(service_ahrs);

	return ATP_SUCCESS;
}
