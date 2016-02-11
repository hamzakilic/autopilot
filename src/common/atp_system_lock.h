/*
 * atp_lock.h
 *
 *  Created on: Feb 8, 2016
 *      Author: root
 */

#ifndef ATP_SYSTEM_LOCK_H_
#define ATP_SYSTEM_LOCK_H_
#include "atp_common.h"

/**
 * @brief hardware i2c lock
 */
extern void * hw_i2c_lock;


#define atp_system_lock_i2c()    atp_thread_lock(hw_i2c_lock)
#define atp_system_unlock_i2c()  atp_thread_unlock(hw_i2c_lock);

em_uint32   atp_system_lock_initialize();
em_uint32   atp_system_lock_uninitialize();

#endif /* ATP_SYSTEM_LOCK_H_ */
