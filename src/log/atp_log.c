/*
 * atp_log.c
 *
 *  Created on: May 13, 2015
 *      Author: root
 */

#include "atp_log.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>



struct client{
	em_int32 socket_descriptior;
    struct sockaddr_in client_address;
    em_uint16 client_port;

};

struct client clients[10];



//static em_int8 client_max_index=1;
static em_int8 initialized_clients=0;
void initialize_clients(const char * host,em_uint16 port){
	clients[0].socket_descriptior=socket(AF_INET,SOCK_DGRAM,0);
	if(clients[0].socket_descriptior==-1)//todo buraya dikkat socket oluşturulamazsa ne olacak
		{
		  perror(strerror(errno));
		  return;
		}
	bzero(&clients[0].client_address,sizeof(struct sockaddr_in));
	clients[0].client_address.sin_family=AF_INET;
	clients[0].client_address.sin_port=htons(port);
	clients[0].client_port=port;
	clients[0].client_address.sin_addr.s_addr=inet_addr(host);
    initialized_clients=1;


}



void atp_log(atp_log_data *log){
#ifdef COMPILE_DEBUG_CODES
	if(log->type==ATP_LOG_INFO)
		printf("Info->");
	if(log->type==ATP_LOG_FATAL)
			printf("Fatal->");
	if(log->type==ATP_LOG_ERROR)
			printf("Error->");
	if(log->type==ATP_LOG_DEBUG)
		printf("Debug->");
	if(log->data_type==ATP_LOG_DATA_TYPE_STRING){
	   printf("Data Type Is String->");
	   printf("Data Len %u->",log->data_len);
	   printf(log->data);
	}
#endif


	if(!initialized_clients){
		initialize_clients("192.168.2.138",9999);

	}
	if(log->type!=ATP_LOG_DEBUG)
	{
		em_uint8 buf_temp[8+log->data_len];

	buf_temp[0]=log->type;
	buf_temp[1]=log->data_type;
	buf_temp[2]=log->data_type>>8;
	buf_temp[3]=log->data_len;
	em_uint32 hash=0;
	hash+=buf_temp[0];
	hash+=buf_temp[1];
	hash+=buf_temp[2];
	hash+=buf_temp[3];
	buf_temp[4]=hash;
	buf_temp[5]=hash>>8;
	buf_temp[6]=hash>>16;
	buf_temp[7]=hash>>24;
	memcpy(buf_temp+8,log->data,log->data_len);

	em_int32 err=0;
	err=sendto(clients[0].socket_descriptior,buf_temp,8+log->data_len,0,(struct sockaddr *)&(clients[0].client_address),sizeof(struct sockaddr_in));
	if(err==-1)
	  perror(strerror(errno));




	}






	atp_free(log->data);
	atp_free(log);
}

void atp_log_raw(atp_log_data *log){
	printf(log->data);
	atp_free(log->data);
	atp_free(log);
}



atp_log_data* atp_log_create_string(em_uint8 log_type, const char *fmt,...){
	atp_log_data *log=atp_malloc(sizeof(atp_log_data));
	log->type=log_type;

	    va_list arg;
		va_start(arg, fmt);
		char *buffer=atp_malloc(sizeof(char)*255);
		atp_fill_zero(buffer,255);
		log->data_len=vsnprintf(buffer,255,fmt,arg);

		va_end(arg);
		log->data=buffer;

        log->data_type=ATP_LOG_DATA_TYPE_STRING;

  return log;

}
