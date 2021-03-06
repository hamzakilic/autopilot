/*
 * module_common.c
 *
 *  Created on: Jan 26, 2016
 *      Author: root
 */

#include "module_common.h"


#define  find_median(type,x,n,size,retval)         if(n>size)return retval; \
											em_int32 c,d; \
											type t; \
											type temp[size]; \
											atp_copy(temp,x,sizeof(type)*n);\
											for (c = 0 ; c < ( n - 1 ); c++){\
												for (d = 0 ; d < n - c - 1; d++){\
													if (temp[d] > temp[d+1]){\
														t         = temp[d];\
														temp[d]   = temp[d+1];\
														temp[d+1] = t;\
														}\
													}\
												}\
												return temp[(n/2)];

#define  find_mean(type,x,n,size,retval)         if(n>size)return retval; \
											em_int32 c,d; \
											type t=0; \
											for (c = 0 ; c <  n; c++){\
												t+=x[c];\
												}\
												return t/(type)n;








inline em_int16 find_median_i16(em_int16 *x,em_int32 n){



	 find_median(em_int16,x,n,DIMSIZE,0xFFFF);

}

inline em_uint16 find_median_u16(em_uint16 *x,em_int32 n){


	find_median(em_uint16,x,n,DIMSIZE,0xFFFF);

}




inline em_int32 find_median_i32(em_int32 *x,em_int32 n){


	find_median(em_int32,x,n,DIMSIZE,0xFFFFFFFF);

}

inline em_int16 find_mean_i16(em_int16 *x,em_int32 n){



	 find_mean(em_int16,x,n,DIMSIZE,0xFFFF);

}

inline em_uint16 find_mean_u16(em_uint16 *x,em_int32 n){


	find_mean(em_uint16,x,n,DIMSIZE,0xFFFF);

}




inline em_int32 find_mean_i32(em_int32 *x,em_int32 n){


	find_mean(em_int32,x,n,DIMSIZE,0xFFFFFFFF);

}



inline void start_kalman(kalman *k){
    k->xk=0;
	k->pk=1;
	k->r=0.01f;


}

