/*
 * atp_input_table.c
 *
 *  Created on: May 12, 2015
 *      Author: root
 */
#include "atp_input.h"







em_uint32 atp_input_create(atp_input **address){
    *address=(atp_input *)malloc(sizeof(atp_input));


    return ATP_SUCCESS;

}
em_uint32 atp_input_destroy(atp_input **address){


	return ATP_SUCCESS;
}

