/*
 * simpleahrs.h
 *
 *  Created on: Jun 2, 2016
 *      Author: root
 */

#ifndef SIMPLEAHRS_H_
#define SIMPLEAHRS_H_
#include <math.h>

void simpleahrs_update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float *roll,float *pitch,float *yaw);

#endif /* SIMPLEAHRS_H_ */
