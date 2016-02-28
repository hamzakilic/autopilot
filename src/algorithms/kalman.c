/*
 * kalman.c
 *
 *  Created on: Feb 28, 2016
 *      Author: root
 */

#include "kalman.h"

inline void kalman_calculate(kalman *filter){
   em_float32 xk=filter->xk;
   em_float32 pk=filter->pk;

   em_float32 kk=pk/(1+filter->r);
   xk=xk+kk*(filter->zk-xk);
   pk=(1-kk)*pk;
   filter->xk=xk;
   filter->pk=pk;



}
