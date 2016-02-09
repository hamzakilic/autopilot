/*
 * autopilot_test.c
 *
 *  Created on: Aug 3, 2015
 *      Author: root
 */

/*
 ============================================================================
 Name        : autopilot.c
 Author      : Hamza Kılıç
 Version     :
 Copyright   : Feel free to use
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <em_io.h>
#include "manager/atp_pilot.h"
#include "log/atp_log.h"

#ifdef COMPILE_TEST_CODES
#define FATAL 1
int test_atp_queue(){
	puts("Starting atp_queue");
    atp_queue *queue=atp_queue_create();
    puts("Adding queue 5");
    atp_queue_push(queue,(void *)5);
    puts("Adding queue 10");
    atp_queue_push(queue,(void *)10);
    puts("Getting first element it must be 5");
    int first_element=(int)atp_queue_pop(queue);
    if(first_element!=5)
    	return FATAL;
    puts("Getting second element it must be 10");
    int second_element=(int)atp_queue_pop(queue);
    if(second_element!=10)
    	return FATAL;


    atp_queue_push(queue,(void *)20);
    atp_queue_push(queue,(void *)200);
    atp_queue_push(queue,(void *)100);
    int count=atp_queue_count(queue);
    if(count!=3)
    	return FATAL;
    atp_queue_destroy(queue);
	puts("Ending atp_queue");
    puts("******************************************************");
	return EXIT_SUCCESS;
}




extern void process_command(atp_command *command);

void destroy_atp_command_test(void * command)
{
	atp_command_test *command_test=(atp_command_test *)command;
	if(command_test->data)
		atp_free(command_test->data);
	atp_free(command_test);
}

int test_atp_command_manager(){
	puts("Starting atp_command_manager");
    atp_command_manager *manager;
    if(atp_command_manager_create(&manager,process_command))
    	return FATAL;

    atp_command *command=atp_malloc(sizeof(atp_command));
    command->type=ATP_COMMAND_TEST;
    command->destroy=destroy_atp_command_test;
    atp_command_test *command_test=atp_malloc(sizeof(atp_command_test));


    const char * command_text="atp_command for atp_command_manager";
    command_test->data=(void*)command_test;

    em_int32 size_of_text=strlen(command_text)+1;

    command_test->data=atp_malloc(sizeof(em_byte)*size_of_text);
    atp_fill_zero(command_test->data,size_of_text);
    atp_copy(command_test->data,command_text,size_of_text-1);
    command->data=command_test;

    if(atp_command_manager_add(command,manager))
    	return FATAL;

    puts("you must see command text as");
    puts(command_text);
    em_io_delay_microseconds(1000000);
    if(atp_command_manager_destroy(manager))
    	return FATAL;
	puts("Ending atp_command_manager");
	puts("******************************************************");
	return EXIT_SUCCESS;
}


int test_atp_command_listener(){
	puts("Starting atp_command_listener");
	puts("it can only test command_test, you cannot see other commands");
	 atp_command_manager *manager;
	    if(atp_command_manager_create(&manager,process_command))
	    	return FATAL;
	 atp_command_listener *listener;
	   	if(atp_command_listener_create(&listener,manager))
	   		return FATAL;
	   	puts("waiting for commands:");
	   	puts("press any key to finish");
	   	getchar();
	   	getchar();
	   	if(atp_command_listener_destroy(listener))
	   		return FATAL;
	   	if(atp_command_manager_destroy(manager))
	   		return FATAL;
	   	puts("Ending atp_command_listener");
	   	puts("******************************************************");
   return EXIT_SUCCESS;


}

int test_pilot(){
	puts("Starting test pilot");
   atp_pilot *pilot;
   if(atp_pilot_create(&pilot)){
	   return FATAL;
   }
   if(atp_pilot_start(pilot))
   {
	   return FATAL;
   }
   puts("waiting for commands:");
   puts("press any key to finish");
   getchar();
   getchar();
   if(atp_pilot_stop(pilot))
	   return FATAL;
   if(atp_pilot_destroy(pilot))
	   return FATAL;
   puts("Ending test pilot");
   puts("******************************************************");
   return EXIT_SUCCESS;
}

int test_litte_big_endian(){
	union {
		    int i;
		    char c[sizeof(int)];
		} x;
		x.i = 1;
		if(x.c[0] == 1)
		    printf("little-endian\n");
		else    printf("big-endian\n");
}




int main(void) {
   puts("Starting Test");
   em_uint32 err;
  	if((err=em_io_initialize(0))){
  		puts("Hardware Initialize Failed Error");
  		return ATP_ERROR_HARDWARE_INITIALIZE;
  	}
   if(test_atp_queue()){
	   puts("Failed atp_queue");
	   return EXIT_FAILURE;
   }

   if(test_atp_command_manager())
   {
	   puts("Failed atp_command_manager");
	   return EXIT_FAILURE;
   }
   if(test_atp_command_listener())
   {
	   puts("Failed atp_command_listener");
	   return EXIT_FAILURE;
   }
   if(test_pilot()){
	   puts("Failed atp_pilot");
	   return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
#endif

