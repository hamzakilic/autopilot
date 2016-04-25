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
  em_int32 can_balance_system;
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
	em_int32 val=0;
	if(task_share && task_share->private){
	val= atp_convert(task_share->private,atp_task_share_data*)->killall;
	}
	//printf("task share killall is:%d\n",val);
	return val;
}

void atp_task_share_killall_set(atp_task_share *task_share,em_int32 killall){
	if(task_share && task_share->private)
		 atp_convert(task_share->private,atp_task_share_data*)->killall=killall;

}

em_int32 atp_task_share_task_count_get(atp_task_share *task_share){
	if(task_share && task_share->private){
		em_int32 count= 	 atp_convert(task_share->private,atp_task_share_data*)->tasks_count;
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

em_int32 atp_task_share_can_balance_system_get(atp_task_share *task_share){
	if(task_share && task_share->private)
			atp_convert(task_share->private,atp_task_share_data*)->can_balance_system;
	return 0;
}

void atp_task_share_can_balance_system_set(atp_task_share *task_share,em_int32 can){
	if(task_share && task_share->private)
				atp_convert(task_share->private,atp_task_share_data*)->can_balance_system=can;

}

