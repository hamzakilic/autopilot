/*
 * atp_system_lock.c
 *
 *  Created on: Feb 8, 2016
 *      Author: root
 */


#include "atp_system_lock.h"

 void * hw_i2c_lock;


em_uint32   atp_system_lock_initialize(){
	hw_i2c_lock=NULL;
	return atp_thread_create_lock(&hw_i2c_lock);

}
em_uint32   atp_system_lock_uninitialize(){
	if(hw_i2c_lock)
		return atp_thread_destory_lock(hw_i2c_lock);
	return ATP_SUCCESS;
}
