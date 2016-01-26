/*
 * module_common.c
 *
 *  Created on: Jan 26, 2016
 *      Author: root
 */

#include "module_common.h"



inline em_int16 find_median(em_int16 *x,em_int32 n){

	  if(n>DIMSIZE)
		  return 0xFFFF;

	  long c, d, t;
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

	  return x[n/2];

}
