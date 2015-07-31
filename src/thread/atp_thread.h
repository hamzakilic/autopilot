/*
 * atp_thread.h
 *
 *  Created on: May 14, 2015
 *      Author: root
 */

#ifndef ATP_THREAD_H_
#define ATP_THREAD_H_
#include <pthread.h>
#include "../common/atp_common.h"
#include "../log/atp_log.h"

typedef void*(*thread_func)(void * ptr);
typedef pthread_t atp_thread_id;

em_uint32 atp_thread_create(atp_thread_id *thread_id,thread_func func,void *arg);
void atp_thread_join(atp_thread_id *thread_id);
em_uint32 atp_thread_create_lock(void **lock_key);
em_uint32 atp_thread_lock(void ** lock_key);
em_uint32 atp_thread_unlock(void **lock_key);
em_uint32 atp_thread_destory_lock(void **lock_key);


#endif /* ATP_THREAD_H_ */
