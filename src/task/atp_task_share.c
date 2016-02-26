/*
 * atp_task_share.c
 *
 *  Created on: Feb 26, 2016
 *      Author: root
 */

#include "atp_task_share.h"

typedef struct {
  em_int32 killall;
  em_int32 tasks_count;
}atp_task_share_data;

em_uint32 atp_task_share_create(atp_task_share **task_share){

	atp_task_share *share=atp_new(atp_task_share);
	atp_task_share_data *data=atp_new(atp_task_share_data);
	bzero(data,sizeof(atp_task_share_data));
	share->private=data;
	*task_share=share;
	return ATP_SUCCESS;

}
em_uint32 atp_task_share_destroy(atp_task_share *task_share){
	if(task_share){
		if(task_share->private){
			atp_task_share_data *data=atp_convert(task_share->private,atp_task_share_data*);
			atp_free(data);
		}
		atp_free(task_share);
	}
	return ATP_SUCCESS;
}

em_int32 atp_task_share_killall_get(atp_task_share *task_share){
	if(task_share && task_share->private)
	return atp_convert(task_share->private,atp_task_share_data*)->killall;
	return 0 ;
}

void atp_task_share_killall_set(atp_task_share *task_share,em_int32 killall){
	if(task_share && task_share->private)
		 atp_convert(task_share->private,atp_task_share_data*)->killall=killall;

}

em_int32 atp_task_share_count_get(atp_task_share *task_share){
	if(task_share && task_share->private){
		em_int32 count= 	 atp_convert(task_share->private,atp_task_share_data*)->tasks_count;
		printf("task count is%d\n",count);
		return count;
	}
	return 0;
}

void atp_task_share_count_plus(atp_task_share *task_share){
	if(task_share && task_share->private){
		 	 atp_convert(task_share->private,atp_task_share_data*)->tasks_count +=1;
	}

}
void atp_task_share_count_minus(atp_task_share *task_share){
	if(task_share && task_share->private)
		 	 atp_convert(task_share->private,atp_task_share_data*)->tasks_count-=1;

}

