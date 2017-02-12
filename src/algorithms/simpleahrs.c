/*
 * simpleahrs.c
 *
 *  Created on: Jun 2, 2016
 *      Author: root
 */
#include "simpleahrs.h"

void simpleahrs_update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float *roll,float *pitch,float *yaw){

	float const PI_F = 3.14159265F;
	*roll = (float)atan2(ay, az);
	if (ay * sin(*roll) + az * cos(*roll) == 0)
	    *pitch = ax > 0 ? (PI_F / 2) : (-PI_F / 2);
	  else
	    *pitch = (float)atan(-ax / (ay * sin(*roll) + az * cos(*roll)));

	*yaw = (float)atan2(mz * sin(*roll) - my * cos(*roll),\
	                                      mx * cos(*pitch) + \
	                                      my * sin(*pitch) * sin(*roll) + \
	                                      mz * sin(*pitch) * cos(*roll));




}
