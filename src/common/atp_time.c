/*
 * atp_time.c
 *
 *  Created on: Jan 17, 2016
 *      Author: root
 */


#include "atp_time.h"

em_uint64 atp_datetime_as_microseconds(){
   struct timeval tv;
    gettimeofday(&tv,0);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}
