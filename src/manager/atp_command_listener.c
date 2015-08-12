/*
 * atp_command_listener.c
 *
 *  Created on: Jun 26, 2015
 *      Author: root
 */
#include "atp_command_listener.h"
#include "atp_command_manager.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	em_int32 sockfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	socklen_t len;
    atp_command_manager *command_manager;
    atp_thread_id thread_id;
    em_uint32 work;
}atp_command_listener_data;



static void  atp_create_socket(atp_command_listener_data *data){
	data->sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(data->sockfd==-1)
	{
		atp_log(atp_log_create_string(ATP_LOG_FATAL,strerror(errno)));
		return;
	}
	bzero(&data->servaddr,sizeof(data->servaddr));
	data->servaddr.sin_family = AF_INET;
	data->servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	data->servaddr.sin_port=htons(9999);
	em_int32 err= bind(data->sockfd,(struct sockaddr *)&data->servaddr,sizeof(data->servaddr));
	if(err==-1){
		atp_log(atp_log_create_string(ATP_LOG_FATAL,strerror(errno)));
	}


}

void delete_command_test(void *data)
{
	atp_command_test *temp=(atp_command_test *)data;
	atp_free(temp->data);
	atp_free(temp);
}

void delete_command_motor(void *data){
	atp_command_motor *motor=(atp_command_motor*)data;
	atp_free(motor);
}

atp_command *parse_command(em_byte *data,em_int32 len)
{

	if(len<2)
      return NULL;
	em_int32 counter;
	atp_command *command=atp_malloc(sizeof(atp_command));
	atp_copy(&command->type,data,2);
#ifdef COMPILE_TEST_CODES
	printf("command type is %d\n",command->type);
#endif
	em_uint32 length;
	      if(len<6)//no lenght found, not a valid command
	      {
	    	  atp_log(atp_log_create_string(ATP_LOG_FATAL,"Not a valid command no length:\n"));
	    	  atp_free(command);
	    	  return NULL;
	      }

	      atp_copy(&length,data+2,4);
#ifdef COMPILE_TEST_CODES
          printf("length is %d\n",length);
#endif
	      em_int32 hash;
	      if(len<10)//no hash found,not a valid command
	      {
	    	  atp_log(atp_log_create_string(ATP_LOG_FATAL,"Not a valid command no hash:\n"));
	    	  atp_free(command);
	    	  return NULL;
	      }
	      atp_copy(&hash,data+6,4);
#ifdef COMPILE_TEST_CODES
          printf("hash is %d\n",hash);
#endif
	      em_int32 calculated_hash=0;
	      for(counter=0;counter<6;++counter)
	    	  calculated_hash+=data[counter];
#ifdef COMPILE_TEST_CODES
	      printf("calculated hash is %d\n",calculated_hash);
#endif
	      if(calculated_hash!=hash)//not a valid command
	      {
	    	  atp_log(atp_log_create_string(ATP_LOG_FATAL,"Not a valid command hashes not equal:\n"));
	    	  atp_free(command);
	    	  return NULL;
	      }

	if(command->type==ATP_COMMAND_TEST)
	{
	  atp_command_test *command_test=atp_malloc(sizeof(atp_command_test));

       em_uint8 *data_string=atp_malloc(sizeof(em_uint8)*length);
       atp_copy(data_string,data+10,length);
       command_test->data=data_string;
       command->data=command_test;
       command->destroy=delete_command_test;
#ifdef COMPILE_TEST_CODES
       puts("returning test command");
#endif
       return command;
	}
	if(command->type==ATP_COMMAND_MOTOR && length==4*sizeof(em_uint16)){

      atp_command_motor *command_motor=atp_malloc(sizeof(atp_command_motor));

      em_uint16 *data_motors=atp_malloc(sizeof(em_uint8)*length);
             atp_copy(data_motors,data+10,length);
             command_motor->values[0]=data_motors[0];
             command_motor->values[1]=data_motors[1];
             command_motor->values[2]=data_motors[2];
             command_motor->values[3]=data_motors[3];
             atp_free(data_motors);//must delete, dont forget
             command->data=command_motor;
             command->destroy=delete_command_motor;
             return command;
	}
	atp_log(atp_log_create_string(ATP_LOG_FATAL,"Not a valid command no compatible command found:\n"));
	atp_free(command);
	return NULL;
}

void * read_commands(void *arg){
	atp_command_listener_data *data=arg;
	em_byte bytes[1024];
	em_int32 len=sizeof(data->cliaddr);
	while(data->work){

		em_int32 received_size= recvfrom(data->sockfd,bytes,1024,0,(struct sockaddr *)&data->cliaddr,&len);
		if(received_size>0){
			atp_command *command=parse_command(bytes,received_size);
			if(command)
				atp_command_manager_add(command,data->command_manager);


		}
	}
	return 0;
}

em_uint32 atp_command_listener_create(atp_command_listener **command_listener,atp_command_manager *command_manager){
    atp_command_listener  *listener=atp_malloc(sizeof(atp_command_listener));
	atp_command_listener_data  *data=atp_malloc(sizeof(atp_command_listener_data));
	listener->private_data=data;
	data->command_manager=command_manager;
	*command_listener=listener;
	atp_create_socket(data);
	data->work=1;
	atp_thread_create(&data->thread_id,read_commands,data);
	return ATP_SUCCESS;

}

em_uint32 atp_command_listener_destroy(atp_command_listener *command_listener){

   if(command_listener)
   {
	  atp_command_listener_data *data=command_listener->private_data;
	  data->work=0;
	  close(data->sockfd);
	  atp_thread_join(&data->thread_id);
	   if(command_listener->private_data)
	      atp_free(command_listener->private_data);
	   atp_free(command_listener);
   }

   return ATP_SUCCESS;
}
