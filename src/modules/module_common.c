/*
 * module_common.c
 *
 *  Created on: Jan 26, 2016
 *      Author: root
 */

#include "module_common.h"

#define  find_median(type,x,n,size)         if(n>size)return 0xFFFFFFFF; \
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
												return temp[n/2];\






inline em_int16 find_median_i16(em_int16 *x,em_int32 n){

	  /*if(n>DIMSIZE)
		  return 0xFFFF;

	  em_int32 c,d;
	  em_int16 t;
      em_int16 temp[DIMSIZE];
      atp_copy(temp,x,sizeof(em_int16)*n);
	  for (c = 0 ; c < ( n - 1 ); c++)
	  {
	    for (d = 0 ; d < n - c - 1; d++)
	    {
	      if (temp[d] > temp[d+1])
	      {


	        t         = temp[d];
	        temp[d]   = temp[d+1];
	        temp[d+1] = t;
	      }

	  }
	}

	  return temp[n/2];*/
	 find_median(em_int16,x,n,DIMSIZE);

}

inline em_uint16 find_median_u16(em_uint16 *x,em_int32 n){

	  /*if(n>DIMSIZE)
		  return 0xFFFF;

	  em_int32 c,d;
	  em_uint16 t;
      em_uint16 temp[DIMSIZE];
      atp_copy(temp,x,sizeof(em_uint16)*n);
	  for (c = 0 ; c < ( n - 1 ); c++)
	  {
	    for (d = 0 ; d < n - c - 1; d++)
	    {
	      if (temp[d] > temp[d+1])
	      {


	        t         = temp[d];
	        temp[d]   = temp[d+1];
	        temp[d+1] = t;
	      }

	  }
	}

	  return temp[n/2];*/
	find_median(em_uint16,x,n,DIMSIZE);

}




inline em_int32 find_median_i32(em_int32 *x,em_int32 n){

	 /* if(n>DIMSIZE)
		  return 0xFFFFFFFF;

	  em_int32 c,d;
	  em_int32 t;
      em_int32 temp[DIMSIZE];
      atp_copy(temp,x,sizeof(em_int32)*n);
	  for (c = 0 ; c < ( n - 1 ); c++)
	  {
	    for (d = 0 ; d < n - c - 1; d++)
	    {
	      if (temp[d] > temp[d+1])
	      {
	        t         = temp[d];
	        temp[d]   = temp[d+1];
	        temp[d+1] = t;
	      }

	  }
	}

	  return temp[n/2];*/
	find_median(em_int32,x,n,DIMSIZE);

}

