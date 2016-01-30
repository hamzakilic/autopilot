/*
 * module_Adafruit_PCA9685.h
 *
 *  Created on: Jan 16, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_PCA9685_H_
#define AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_PCA9685_H_

#include "../common/atp_common.h"
#include "../log/atp_log.h"



#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD




/**
 * @brief starts adafruit pwm driver
 */
em_uint32 adafruit_pca9685_start(void *parameter);

/**
 * @brief set pwm value to pin_number
 * @param value is set value
 * @pin_number is motor connected pin
 * return ATP_SUCCESS or errors
 */
em_uint32 adafruit_pca9685_set(em_uint16 value,em_uint8 pin_number);

/**
 * @brief empty function, no need to use
 *
 */
em_uint32 adafruit_pca9685_stop(void *parameter);



#endif /* AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_PCA9685_H_ */
