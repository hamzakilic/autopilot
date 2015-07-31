/*
 * atp_input_table.h
 *
 *  Created on: May 12, 2015
 *      Author: root
 */

#ifndef ATP_INPUT_H_
#define ATP_INPUT_H_
#include "../common/atp_common.h"


typedef em_uint32 (*atp_input_update_gps_func)(const gps * const data);

typedef struct {
    atp_input_update_gps_func update_gps;
}atp_input;


em_uint32 atp_input_create(atp_input **address);
em_uint32 atp_input_destroy(atp_input **address);



#endif /* ATP_INPUT_H_ */
