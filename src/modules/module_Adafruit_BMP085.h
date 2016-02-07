/*
 * module_Adafruit_BMP085.h
 *
 *  Created on: Jan 11, 2016
 *      Author: root
 */

#ifndef AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_BMP085_H_
#define AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_BMP085_H_



#include "../common/atp_common.h"
#include "../log/atp_log.h"
#include "module_common.h"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define BMP085_ADDRESS                (0x77)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      BMP085_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
      BMP085_REGISTER_CHIPID             = 0xD0,
      BMP085_REGISTER_VERSION            = 0xD1,
      BMP085_REGISTER_SOFTRESET          = 0xE0,
      BMP085_REGISTER_CONTROL            = 0xF4,
      BMP085_REGISTER_TEMPDATA           = 0xF6,
      BMP085_REGISTER_PRESSUREDATA       = 0xF6,
      BMP085_REGISTER_READTEMPCMD        = 0x2E,
      BMP085_REGISTER_READPRESSURECMD    = 0x34
    };
/*=========================================================================*/

/*=========================================================================
    MODE SETTINGS
    -----------------------------------------------------------------------*/
    typedef enum
    {
      BMP085_MODE_ULTRALOWPOWER          = 0,
      BMP085_MODE_STANDARD               = 1,
      BMP085_MODE_HIGHRES                = 2,
      BMP085_MODE_ULTRAHIGHRES           = 3
    } bmp085_mode_t;
/*=========================================================================*/

/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
    typedef struct
    {
      em_int16  ac1;
      em_int16  ac2;
      em_int16  ac3;
      em_uint16 ac4;
      em_uint16 ac5;
      em_uint16 ac6;
      em_int16  b1;
      em_int16  b2;
      em_int16  mb;
      em_int16  mc;
      em_int16  md;
    } bmp085_calib_data;
/*=========================================================================*/

    /**********************************************************************
     **************************PRESSURE AND TEMPRATURE FUNCTIONS***********
     **********************************************************************/

    /**
     * @brief starts bmp085 temprerature and pressure sensor
     * @param parameters send NULL
     * @return ATP_SUCCESS or ATP_???? failure numbers
     */
    em_uint32 adafruit_bmp085_temp_pres_start(void *parameter);



    /**
     * @brief read temprature and pressure
     * @param temp is tempreture pointer ,degrees Celsius
     * @param press is pressure pointer, pressure level in hPa
     * return ATP_SUCCESS or ATP_???? failure numbers
     */
    em_uint32 adafruit_bmp085_temp_press_read(em_float32 *temp,em_float32 *press);

    em_uint32 adafruit_bmp085_temp_press_read_raw(em_float32 *temp,em_float32 *press);


    /**
     * @brief does not do anything, empty function
     */
    em_uint32 adafruit_bmp085_temp_press_stop(void *);







#endif /* AUTOPILOTC_SRC_MODULES_MODULE_ADAFRUIT_BMP085_H_ */
