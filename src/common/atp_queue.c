/*
 * atp_queue.c
 *
 *  Created on: May 22, 2015
 *      Author: root
 */
#include "atp_queue.h"

typedef struct node_t{
	void *item;
	struct node_t *next;
}node;

typedef struct{
	em_uint32 item_count;
	node* first;
    node* last;

}queue_data;



atp_queue *atp_queue_create(){
	atp_queue *queue= atp_malloc(sizeof(atp_queue));
	queue_data *data=atp_malloc(sizeof(queue_data));
	memset(data,0,sizeof(queue_data));
	queue->private_data=data;
	return queue;

}
void atp_queue_destroy(atp_queue *queue){
	if(queue->private_data){
		queue_data *data= queue->private_data;
		while(data->item_count){
			atp_queue_pop(queue);
		}
     atp_free(queue->private_data);
	}
	if(queue)
   atp_free(queue);
}

void atp_queue_push(atp_queue *queue, void *item){
     queue_data *data= queue->private_data;
     if(data->first==0)
    	{
    	    node *leaf=atp_malloc(sizeof(node));
    	    leaf->item=item;
    	    leaf->next=0;
    	    data->first=leaf;
    	}
     if(data->last==0){
    	 data->last=data->first;

     }else{
    	 node *leaf=atp_malloc(sizeof(node));
    	    	    leaf->item=item;
    	    	    leaf->next=0;
    	    	    data->last->next=leaf;
    	    	    data->last=leaf;
     }
     data->item_count++;


}
em_uint32 atp_queue_count(atp_queue *queue){
	queue_data *data= queue->private_data;
	return data->item_count;

}
void * atp_queue_pop(atp_queue *queue){
	queue_data *data= queue->private_data;
	node * temp= data->first;
	data->first=temp->next;
	data->item_count--;
	if(data->item_count==0)
		data->last=0;
	void *item_temp= temp->item;
	atp_free(temp);
	return item_temp;

}
