/*
 * module_common.h
 *
 *  Created on: Jan 26, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_MODULES_MODULE_COMMON_H_
#define AUTOPILOTC_SRC_MODULES_MODULE_COMMON_H_

#include "../common/atp_common.h"



#define DIMSIZE 13
typedef struct{
	em_int16 x[DIMSIZE];
	em_int16 y[DIMSIZE];
	em_int16 z[DIMSIZE];

}frame;

inline em_int16 find_median(em_int16 *x,em_int32 n);


#endif /* AUTOPILOTC_SRC_MODULES_MODULE_COMMON_H_ */
