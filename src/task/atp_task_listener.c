/*
 * atp_task_listener.c
 *
 *  Created on: Feb 21, 2016
 *      Author: root
 */
#include "atp_task_listener.h"
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
    atp_thread_id thread_id;
    em_uint32 work;
    atp_task_manager *manager;
}atp_task_listener_data;


static em_uint32  atp_create_socket(atp_task_listener_data *data){
	data->sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct timeval time;
	time.tv_sec=0;
	time.tv_usec=100000;//timeout
	setsockopt(data->sockfd,SOL_SOCKET,SO_RCVTIMEO,&time,sizeof(time));
	if(data->sockfd==-1)
	{
		atp_log(atp_log_create_string(ATP_LOG_FATAL,strerror(errno)));
		return ATP_ERROR_CREATE_SOCKET;
	}
	bzero(&data->servaddr,sizeof(data->servaddr));
	data->servaddr.sin_family = AF_INET;
	data->servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	data->servaddr.sin_port=htons(9999);
	em_int32 err= bind(data->sockfd,(struct sockaddr *)&data->servaddr,sizeof(data->servaddr));
	if(err==-1){
		atp_log(atp_log_create_string(ATP_LOG_FATAL,strerror(errno)));
		return ATP_ERROR_CREATE_SOCKET;
	}
	return ATP_SUCCESS;


}


void * read_tasks(void *arg){
	atp_task_listener_data *data=arg;
	em_byte bytes[1024];
	em_int32 len=sizeof(data->cliaddr);
	while(data->work){

		em_int32 received_size= recvfrom(data->sockfd,bytes,1024,0,(struct sockaddr *)&data->cliaddr,&len);
		if(received_size>0){
			atp_task_manager_add_task(data->sockfd,received_size,data->manager);


		}
	}
	return 0;
}


em_uint32 atp_task_listener_create(atp_task_listener **task_listener,atp_task_manager *task_manager){

	 atp_task_listener  *listener=atp_malloc(sizeof(atp_task_listener));
		atp_task_listener_data  *data=atp_malloc(sizeof(atp_task_listener_data));

		listener->private_data=data;
		*task_listener=listener;
		data->manager=task_manager;
		em_uint32 err= atp_create_socket(data);
		if(err){
			atp_free(data);
			atp_free(listener);
			return err;
		}
		data->work=1;
		//todo: thread create edilemezse nolacak
		atp_thread_create(&data->thread_id,read_tasks,data);
		return ATP_SUCCESS;


}
em_uint32 atp_task_listener_destroy(atp_task_listener *task_listener){

	if(task_listener)
	   {
		  atp_task_listener_data *data=task_listener->private_data;
		  data->work=0;
		  close(data->sockfd);
		  atp_thread_join(&data->thread_id);
		   if(task_listener->private_data)
		      atp_free(task_listener->private_data);
		   atp_free(task_listener);
	   }
	return ATP_SUCCESS;

}
