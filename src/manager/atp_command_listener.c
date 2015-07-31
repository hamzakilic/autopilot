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
void * read_commands(void *arg){
	atp_command_listener_data *data=arg;
	em_byte bytes[1024];
	em_int32 len=sizeof(data->cliaddr);
	while(data->work){

		em_int32 received_size= recvfrom(data->sockfd,bytes,1024,0,(struct sockaddr *)&data->cliaddr,&len);
		if(received_size>0){
		//create command
		//push to command_manager
		}
	}
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
