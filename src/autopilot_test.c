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

	return EXIT_SUCCESS;
}

int test_atp_command_listener(){
	puts("Starting atp_command_manager");
    atp_command_manager *manager;
    if(atp_command_manager_create(&manager))
    	return FATAL;

    //atp_command_manager_add()
	puts("Ending atp_command_manager");
	return EXIT_SUCCESS;
}


int main(void) {
   puts("Starting Test");
   if(test_atp_queue()){
	   puts("Failed atp_queue");
	   return EXIT_FAILURE;
   }


	return EXIT_SUCCESS;
}
#endif

