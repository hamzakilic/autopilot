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

	em_int16 x_i16[DIMSIZE];
	em_int16 y_i16[DIMSIZE];
	em_int16 z_i16[DIMSIZE];

	em_uint16 x_u16[DIMSIZE];
	em_uint16 y_u16[DIMSIZE];
	em_uint16 z_u16[DIMSIZE];


	em_int32 x_i32[DIMSIZE];



}frame;

inline em_int16 find_median_i16(em_int16 *x,em_int32 n);
inline em_uint16 find_median_u16(em_uint16 *x,em_int32 n);
inline em_int32 find_median_i32(em_int32 *x,em_int32 n);


#endif /* AUTOPILOTC_SRC_MODULES_MODULE_COMMON_H_ */
