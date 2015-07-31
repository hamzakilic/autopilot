/*
 * atp_queue.h
 *
 *  Created on: May 22, 2015
 *      Author: root
 */

#ifndef ATP_QUEUE_H_
#define ATP_QUEUE_H_
#include "atp_common.h"


typedef struct{
   void *private_data;
}atp_queue;

atp_queue *atp_queue_create();
void atp_queue_destroy(atp_queue *queue);
void atp_queue_push(atp_queue *queue, void *data);
em_uint32 atp_queue_count(atp_queue *queue);
void * atp_queue_pop(atp_queue *queue);




#endif /* ATP_QUEUE_H_ */
