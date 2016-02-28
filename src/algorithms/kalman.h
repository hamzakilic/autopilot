/*
 * kalman.h
 *
 *  Created on: Feb 28, 2016
 *      Author: root
 */

#ifndef KALMAN_H_
#define KALMAN_H_
#include "../common/atp_common.h"

#define KALMAN_R 0.01f
#define KALMAN_PK 1

typedef struct{
  em_float32 xk;
  em_float32 pk;
  em_float32 zk;
  em_float32 r;

}kalman;

inline void kalman_calculate(kalman *filter);

#endif /* KALMAN_H_ */
