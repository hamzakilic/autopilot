/*
 * atp_pilot.c
 *
 *  Created on: May 13, 2015
 *      Author: root
 */
#include "atp_pilot.h"

typedef struct {
	atp_input *input;
	atp_settings *settings;
	atp_command_manager *command_manager;
	atp_command_listener *command_listener;
	em_uint32 service_system_started;
	atp_motor_controller *motor_controller;
	em_int32 pilot_started;
}atp_pilot_data;

static atp_pilot_data *pilot_data=0;

em_uint32 atp_pilot_create(atp_pilot **pilot){

    if(pilot_data!=0)
    {
    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"AllReady Created Pilot\n"));
    	return ATP_ERROR_PILOT_CREATED_ALLREADY;
    }
    //create necessary structures and reset their values
    pilot_data=atp_malloc(sizeof(atp_pilot_data));
    atp_fill_zero(pilot_data,sizeof(atp_pilot_data));
    atp_pilot * pilot_temp=atp_malloc(sizeof(atp_pilot));
    *pilot=pilot_temp;
    pilot_temp->private_data=pilot_data;
    atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Pilot Success \n"));

    return ATP_SUCCESS;
}


void process_command(atp_command *command){

	if(command->type==ATP_COMMAND_TEST)
	{

	    atp_command_test *command_test=(atp_command_test *)command->data;
		#ifdef COMPILE_TEST_CODES
		puts(command_test->data);
		#endif
	}

	if(command->type==ATP_COMMAND_MOTOR)
	{

       atp_command_motor *motor_control=(atp_command_motor *)command->data;
       if(pilot_data!=0){
           atp_motor_controller_set_values(pilot_data->motor_controller,motor_control->values);

       }else{
			#ifdef COMPILE_TEST_CODES
            printf("pilot_data is null, cannot process atp_command_motor\n");
			#endif
       }


	}

	if(command->destroy)
		command->destroy(command->data);
	atp_free(command);

}


em_uint32 atp_pilot_start(atp_pilot *pilot){
     atp_pilot_data *pilot_data=(atp_pilot_data *)pilot->private_data;


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

//create settings
	    err=0;
	    if(pilot_data->settings==NULL)
	    	err=atp_settings_create(&pilot_data->settings);
	    if(err)
	    	    {
	    	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Settings Failed Error:%u\n",err));
	    	    	return ATP_ERROR_CREATE_INPUT;
	    	    }else{
	    	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Create Settings Success \n"));

	    	    }


	    //create command manager
        err=0;
        if(pilot_data->command_manager==NULL)
        err=atp_command_manager_create(&pilot_data->command_manager,process_command);
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
	    err=atp_service_system_start(pilot_data->input,pilot_data->settings);

	    if(err){

	    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Service System Failed Error:%u\n",err));
	    	return ATP_ERROR_START_SERVICE_SYSTEM;
	    }else{
	    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Service System Success \n"));
	    	pilot_data->service_system_started=1;
	    }

	    //create motor controllers
	    err=0;
	    if(pilot_data->motor_controller==NULL)
	    err=atp_motor_controller_create(pilot_data->input,&pilot_data->motor_controller);

	    if(err){

	        	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Start Motor Controllers Failed Error:%u\n",err));
	        	return ATP_ERROR_START_SERVICE_SYSTEM;
	        }else{

	        	atp_log(atp_log_create_string(ATP_LOG_INFO,"Start Motor Controllers Success \n"));
	        }

	    return ATP_SUCCESS;

}
em_uint32 atp_pilot_stop(atp_pilot *pilot){
	atp_pilot_data *pilot_data=(atp_pilot_data*)pilot->private_data;

	//stop command listener
	em_uint32 err=0;
	        if(pilot_data->command_listener)
		    err=atp_command_listener_destroy(pilot_data->command_listener);
		    if(err)
		    {
		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Destroy Command Listener Failed Error:%u\n",err));
		    	return ATP_ERROR_CREATE_INPUT;
		    }else{
		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Command Listener Success \n"));

		    }
		    pilot_data->command_listener=NULL;


		    //stop command manager
            err=0;
            if(pilot_data->command_manager)
		    err=atp_command_manager_destroy(pilot_data->command_manager);
		    		    if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Destroy Command Manager Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Command Manager Success \n"));

		    		    }
		    		    pilot_data->command_manager=NULL;


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
pilot_data->service_system_started=0;


//stop motor controller
err=0;
if(pilot_data->motor_controller)
	err=atp_motor_controller_destroy(pilot_data->motor_controller);

if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Stop Motor Controller Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Stop Motor Controller Success \n"));

		    		    }
pilot_data->motor_controller=NULL;



//destroy input
err=0;
if(pilot_data->input)
	err=atp_input_destroy(pilot_data->input);

if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Stop Input  Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Stop Input  Success \n"));

		    		    }
pilot_data->input=NULL;

//destroy settings

err=0;
if(pilot_data->settings)
	err=atp_settings_destroy(pilot_data->settings);

if(err)
		    		    {
		    		    	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Stop Settings  Failed Error:%u\n",err));
		    		    	return ATP_ERROR_CREATE_INPUT;
		    		    }else{
		    		    	atp_log(atp_log_create_string(ATP_LOG_INFO,"Stop Settings  Success \n"));

		    		    }
pilot_data->settings=NULL;






	return ATP_SUCCESS;

}
em_uint32 atp_pilot_destroy(atp_pilot *pilot){
	if(pilot){
		atp_pilot_stop(pilot);//if it is not stoped check it
		if(pilot->private_data){
	      atp_free(pilot->private_data);
		}

	atp_free(pilot);
	}
	atp_log(atp_log_create_string(ATP_LOG_INFO,"Destroy Pilot Success \n"));


return ATP_SUCCESS;
}
