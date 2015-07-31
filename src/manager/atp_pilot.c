/*
 * atp_pilot.c
 *
 *  Created on: May 13, 2015
 *      Author: root
 */
#include "atp_pilot.h"

static atp_input *input;
static atp_command_manager *command_manager;
em_uint32 atp_pilot_create(){
	em_uint32 err;
    err=atp_input_create(&input);
    if(err)
    {
    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Input Failed Error:%u\n",err));
    	return ATP_ERROR_CREATE_INPUT;
    }else{
    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Input Success \n"));

    }

    err=atp_command_manager_create(&command_manager);
    if(err){
    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Command Manager System Failed Error:%u\n",err));
    	    	return ATP_ERROR_START_SERVICE_SYSTEM;
    	    }else{
    	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Command Manager Success \n"));
    	    }


    err=atp_service_system_start(input);

    if(err){

    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Service System Failed Error:%u\n",err));
    	return ATP_ERROR_START_SERVICE_SYSTEM;
    }else{
    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Service System Success \n"));
    }

    err=atp_motor_controller_start(input);

    if(err){

        	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Motor Controllers Failed Error:%u\n",err));
        	return ATP_ERROR_START_SERVICE_SYSTEM;
        }else{
        	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Motor Controllers Success \n"));
        }

    return ATP_SUCCESS;
}
