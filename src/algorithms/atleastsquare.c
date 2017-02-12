/*
 * atleastsquare.c
 *
 *  Created on: May 17, 2016
 *      Author: root
 */
#include "atleastsquare.h"
 inline static em_float32 sqr(em_float32 x) {
        return x*x;
    }

 em_int32 atleastsquare(em_int32 n, const em_float32 y[], const em_float32 x[], em_float32* a, em_float32* b, em_float32* r){

	em_float32   sumx = 0.0;                        /* sum of x                      */
	em_float32   sumx2 = 0.0;                       /* sum of x**2                   */
	em_float32   sumxy = 0.0;                       /* sum of x * y                  */
	em_float32   sumy = 0.0;                        /* sum of y                      */
	em_float32   sumy2 = 0.0;                       /* sum of y**2                   */

    em_int32 i;


       for (i=0;i<n;i++)
          {
          sumx  += x[i];

          sumx2 += sqr(x[i]);

          sumxy += x[i] * y[i];
          sumy  += y[i];
          sumy2 += sqr(y[i]);
          }


       em_float32 denom = (n * sumx2 - sqr(sumx));
       if (denom == 0) {
           // singular matrix. can't solve the problem.
           *a = 0;
           *b = 0;
           if (r) *r = 0;
           return 1;
       }

       *a = (n * sumxy  -  sumx * sumy) / denom;
       *b = (sumy * sumx2  -  sumx * sumxy) / denom;
       if (r!=NULL) {
          *r = (sumxy - sumx * sumy / n) /          /* compute correlation coeff     */
                sqrt((sumx2 - sqr(sumx)/n) *
                (sumy2 - sqr(sumy)/n));
       }
       return 0;
}

