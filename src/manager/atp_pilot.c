/*
 * atp_pilot.c
 *
 *  Created on: May 13, 2015
 *      Author: root
 */
#include "atp_pilot.h"

typedef struct {
	atp_input *input;
	atp_command_manager *command_manager;
	atp_command_listener *command_listener;
	em_uint32 service_system_started;
	em_uint32 motor_controller_started;
}atp_pilot_data;

static atp_pilot_data *data=0;

em_uint32 atp_pilot_create(atp_pilot **pilot){

    if(data!=0)
    {
    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"AllReady Created Pilot\n"));
    	return ATP_ERROR_PILOT_CREATED_ALLREADY;
    }
    //create necessary structures and reset their values
    data=atp_malloc(sizeof(atp_pilot_data));
    data->command_listener=NULL;
    data->command_manager=NULL;
    data->input=NULL;
    data->service_system_started=0;
    data->motor_controller_started=0;
    atp_pilot * pilot_temp=atp_malloc(sizeof(atp_pilot));
    *pilot=pilot_temp;
    pilot_temp->private_data=data;
    atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Pilot Success \n"));

    return ATP_SUCCESS;
}


em_uint32 atp_pilot_start(atp_pilot *pilot){
     atp_pilot_data *pilot_data=(atp_pilot_data)pilot->private_data;

     //create input
	em_uint32 err=0;
        if(pilot_data->input==NULL)
	    err=atp_input_create(&pilot_data->input);
	    if(err)
	    {
	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Input Failed Error:%u\n",err));
	    	return ATP_ERROR_CREATE_INPUT;
	    }else{
	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Input Success \n"));

	    }

	    //create command manager
        err=0;
        if(pilot_data->command_manager==NULL)
	    err=atp_command_manager_create(&pilot_data->command_manager);
	    if(err){
	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Command Manager System Failed Error:%u\n",err));
	    	    	return ATP_ERROR_START_COMMANDMANAGER;
	    	    }else{
	    	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Command Manager Success \n"));
	    	    }

	    //create command listener
        err=0;
        if(pilot_data->command_listener==NULL)
	    err=atp_command_listener_create(&pilot_data->command_listener,pilot_data->command_manager);
	    if(err){
	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Command Listener Failed Error:%u\n",err));
	    	    	    	return ATP_ERROR_START_COMMANDLISTENER;
	    	    	    }else{
	    	    	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Command Listener Success \n"));
	    	    	    }

	    //create service system
        err=0;
        if(pilot_data->service_system_started==0)
	    err=atp_service_system_start(pilot_data->input);

	    if(err){

	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Service System Failed Error:%u\n",err));
	    	return ATP_ERROR_START_SERVICE_SYSTEM;
	    }else{
	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Service System Success \n"));
	    	pilot_data->service_system_started=1;
	    }

	    //create motor controllers
	    err=0;
	    if(pilot_data->motor_controller_started==0)
	    err=atp_motor_controller_start(pilot_data->input);

	    if(err){

	        	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Motor Controllers Failed Error:%u\n",err));
	        	return ATP_ERROR_START_SERVICE_SYSTEM;
	        }else{
	        	pilot_data->motor_controller_started=1;
	        	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Motor Controllers Success \n"));
	        }
	    return ATP_SUCCESS;

}
em_uint32 atp_pilot_stop(atp_pilot *pilot){
	atp_pilot_data *pilot_data=(atp_pilot_data*)pilot->private_data;

	//stop command listener
	em_uint32 err=0;
	        if(pilot_data->command_listener)
		    err=atp_command_listener_destroy(&pilot_data->command_listener);
		    if(err)
		    {
		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Destroy Command Listener Failed Error:%u\n",err));
		    	return ATP_ERROR_CREATE_INPUT;
		    }else{
		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Command Listener Success \n"));

		    }

		    //stop command manager
            err=0;
            if(pilot_data->command_manager)
		    err=atp_command_manager_destroy(&pilot_data->command_manager);
		    		    if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Destroy Command Manager Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Command Manager Success \n"));

		    		    }

		    		    //stop service system
err=0;
if(pilot_data->service_system_started)
	err=atp_service_system_stop();

if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Stop Service System Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Stop Service System Success \n"));

		    		    }

//stop motor controller
err=0;
if(pilot_data->motor_controller_started)
	err=atp_motor_controller_stop();

if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Stop Motor Controller Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Stop Motor Controller Success \n"));

		    		    }

	return ATP_SUCCESS;

}
em_uint32 atp_pilot_destroy(atp_pilot *pilot){
	if(pilot->private_data)
	atp_free(pilot->private_data);
	if(pilot)
	atp_free(pilot);
	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Pilot Success \n"));


return ATP_SUCCESS;
}
