/*
 * atp_thread.c
 *
 *  Created on: May 14, 2015
 *      Author: root
 */

#include "atp_thread.h"

em_uint32 atp_thread_create(atp_thread_id *thread_id, thread_func func,void *arg){

   em_uint32 err;
   err=pthread_create((pthread_t *)thread_id,NULL,func,arg);
   if(err){
	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Thread Failed Error:%u\n",err));
	   return ATP_ERROR_CREATE_THREAD;
   }

   return ATP_SUCCESS;




}

void atp_thread_join(atp_thread_id *thread_id){
	pthread_join(*thread_id,NULL);
}


em_uint32 atp_thread_create_lock(void **lock_key){
	em_uint32 err;
   pthread_mutex_t *mutex=atp_malloc(sizeof(pthread_mutex_t));
   err=pthread_mutex_init(mutex,NULL);
   if(err){
   	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Thread Mutex Failed Error:%u\n",err));
   	   return ATP_ERROR_CREATE_THREAD_MUTEX;
      }
   *lock_key=mutex;
   return ATP_SUCCESS;

}
em_uint32 atp_thread_lock(void * lock_key){
	em_uint32 err= pthread_mutex_lock(lock_key);
	if(err){
		   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Mutex Lock Failed Error:%u\n",err));
		   return ATP_ERROR_MUTEX_LOCK;
	   }
	return ATP_SUCCESS;

}
em_uint32 atp_thread_unlock(void *lock_key){

	em_uint32 err= pthread_mutex_unlock(lock_key);
	if(err){
		   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Mutex Unlock Failed Error:%u\n",err));
		   return ATP_ERROR_MUTEX_UNLOCK;
	   }
	return ATP_SUCCESS;

}
em_uint32 atp_thread_destory_lock(void *lock_key){
  em_int32 err=pthread_mutex_destroy(lock_key);
  if(err){
  	   atp_log(atp_log_create_string(ATP_LOG_FATAL,"Mutex Destroy Failed Error:%u\n",err));
  	   return ATP_ERROR_MUTEX_DESTROY;
     }
  atp_free(lock_key);
  return ATP_SUCCESS;
}

