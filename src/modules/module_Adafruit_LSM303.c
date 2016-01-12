/*
 * module_Adafruit_LSM303.c
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */
#include "module_Adafruit_LSM303.h"


static float _lsm303Accel_MG_LSB     = 0.001F;   // 1, 2, 4 or 12 mg per lsb
static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain

#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON              (1.6F)                  /**< The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN               (275.0F)                /**< The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 /**< Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 /**< Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              /**< Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**< Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**< Gauss to micro-Tesla multiplier */


inline em_uint32 i2c_try_read(em_uint16 address,em_uint8 *data,em_uint32  *lenght){
	em_uint32 try=0;
	em_uint32 err;
	 for(try=0;try<100;++try){
		  err=em_io_i2c_read(EM_USE_BSC1,address,data,lenght);
		    if(!err)
		    	break;
	      }
	      if(try==100){
	    	    return ATP_ERROR_HARDWARE_COMMUNICATION;
	      }
	      return ATP_SUCCESS;

}

/**
 * @brief shorcuts to read 8 byte from i2c
 * @param val pointer to data array
 */

inline em_int32 lsm303_accel_read8(em_byte *data){

    em_int32 length=1;
    return em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_ACCEL,data,&length);
}
inline em_int32 lsm303_accel_read16(em_uint16 *data){
    em_byte temp[2];
    em_int32 length=2;
    em_int32 err;
    err= em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_ACCEL,temp,&length);
    *data=temp[0]<<8||temp[1];
    return err;
}

inline em_int32 lsm303_accel_write8(em_byte val){
	em_byte data[1];
	data[0]=val;
	return em_io_i2c_write(EM_USE_BSC1,LSM303_ADDRESS_ACCEL,data,1);
}

inline em_int32 lsm303_accel_write16(em_byte reg,em_byte val){
	em_byte data[2];
	data[0]=reg;
	data[1]=val;
	return em_io_i2c_write(EM_USE_BSC1,LSM303_ADDRESS_ACCEL,data,2);
}



///////////////////////////////////////////////////
//////////////accelerometer codes//////////////////
///////////////////////////////////////////////////

em_int32 adafruit_lsm303_accel_start(void * param){

	em_uint32 err;


	// Enable the accelerometer (100Hz)
     em_byte data[2];

	  err=lsm303_accel_write16(LSM303_REGISTER_ACCEL_CTRL_REG1_A,0x57);
	  if(err){

		  return ATP_ERROR_HARDWARE_COMMUNICATION;
	  }

	  // LSM303DLHC has no WHOAMI register so read CTRL_REG1_A back to check
	  // if we are connected or not

	  err=lsm303_accel_write8(LSM303_REGISTER_ACCEL_CTRL_REG1_A);
	  if(err){

	  		   	return ATP_ERROR_HARDWARE_COMMUNICATION;
	  	  }
	  em_uint32 temp=1;
	  err=lsm303_accel_read8(data);
	  if(err){

    	    return ATP_ERROR_HARDWARE_COMMUNICATION;
	  }


	  em_uint8 reg1_a =data[0];// read8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A);
	  if (reg1_a != 0x57)
	  {

	    return ATP_ERROR_HARDWARE_INITIALIZE;
	  }

	return ATP_SUCCESS;
}
em_int32 adafruit_lsm303_stop(void *param){
	return ATP_SUCCESS;

}




em_int32 adafruit_lsm303_accel_read(float *values){
	em_uint32 err;
	em_byte data[6];
	data[0]=LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80;
	err=lsm303_accel_write8(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
	 if(err){
		     	return ATP_ERROR_HARDWARE_COMMUNICATION;
	 }
	 em_uint32 lenght=6;
	 err=em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_ACCEL,data,&lenght);
		  if(err){
		  	 	return ATP_ERROR_HARDWARE_COMMUNICATION;
		  }

	 values[0]= (em_int16)(data[0] | (data[1] << 8)) >> 4;
	 values[1]= (em_int16)(data[2] | (data[3] << 8)) >> 4;
	 values[2]= (em_int16)(data[4] | (data[5] << 8)) >> 4;

	 values[0]*=_lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
	 values[1]*=_lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
	 values[2]*=_lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;



	return ATP_SUCCESS;
}







//////////////////////////////////////////////////
///////////magnetometer codes/////////////////////
//////////////////////////////////////////////////


static em_int32 _magGain;

inline em_int32 lsm303_mag_read8(em_byte *data){

    em_int32 length=1;
    return em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_MAG,data,&length);
}
inline em_int32 lsm303_mag_read16(em_byte *data){

    em_int32 length=2;
    return em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_MAG,data,&length);
}

inline em_int32 lsm303_mag_write8(em_byte val){
	em_byte data[1];
	data[0]=val;
	return em_io_i2c_write(EM_USE_BSC1,LSM303_ADDRESS_MAG,data,1);
}

inline em_int32 lsm303_mag_write16(em_byte reg,em_byte val){
	em_byte data[2];
	data[0]=reg;
	data[1]=val;
	return em_io_i2c_write(EM_USE_BSC1,LSM303_ADDRESS_MAG,data,2);
}


em_int32 setMagGain(lsm303MagGain gain)
{

    em_int32 err;
	em_byte data[2];

			  err=lsm303_mag_write16(LSM303_REGISTER_MAG_CRB_REG_M,gain);
			  if(err)
			 	 return ATP_ERROR_HARDWARE_COMMUNICATION;
			  _magGain=gain;

  switch(gain)
  {
    case LSM303_MAGGAIN_1_3:
      _lsm303Mag_Gauss_LSB_XY = 1100;
      _lsm303Mag_Gauss_LSB_Z  = 980;
      break;
    case LSM303_MAGGAIN_1_9:
      _lsm303Mag_Gauss_LSB_XY = 855;
      _lsm303Mag_Gauss_LSB_Z  = 760;
      break;
    case LSM303_MAGGAIN_2_5:
      _lsm303Mag_Gauss_LSB_XY = 670;
      _lsm303Mag_Gauss_LSB_Z  = 600;
      break;
    case LSM303_MAGGAIN_4_0:
      _lsm303Mag_Gauss_LSB_XY = 450;
      _lsm303Mag_Gauss_LSB_Z  = 400;
      break;
    case LSM303_MAGGAIN_4_7:
      _lsm303Mag_Gauss_LSB_XY = 400;
      _lsm303Mag_Gauss_LSB_Z  = 355;
      break;
    case LSM303_MAGGAIN_5_6:
      _lsm303Mag_Gauss_LSB_XY = 330;
      _lsm303Mag_Gauss_LSB_Z  = 295;
      break;
    case LSM303_MAGGAIN_8_1:
      _lsm303Mag_Gauss_LSB_XY = 230;
      _lsm303Mag_Gauss_LSB_Z  = 205;
      break;
  }
  return ATP_SUCCESS;
}



em_int32 adafruit_lsm303_mag_start(void * param){

	em_uint32 err;


		// Enable the magnetometer
			        em_byte data[2];

		  err=lsm303_mag_write16(LSM303_REGISTER_MAG_MR_REG_M,0x00);
		  if(err){

			  return ATP_ERROR_HARDWARE_COMMUNICATION;
		  }

		  // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
		   // the default value (0b00010000/0x10)

		  err=lsm303_mag_write8(LSM303_REGISTER_MAG_CRA_REG_M);
		  if(err){

			  return ATP_ERROR_HARDWARE_COMMUNICATION;
		  }

		  em_uint32 temp=1;
		  err=lsm303_mag_read8(data);
		  if(err){

	    	    return ATP_ERROR_HARDWARE_COMMUNICATION;
		  }


		  em_uint8 reg1_a =data[0];
		  if (reg1_a != 0x10 && reg1_a != 24)
		  {

		    return ATP_ERROR_HARDWARE_INITIALIZE;
		  }

		  setMagGain(LSM303_MAGGAIN_1_3);
		  //setmagrate
		  em_byte reg_m = (LSM303_MAGRATE_75 & 0x07) << 2;
		  err=lsm303_mag_write16(LSM303_REGISTER_MAG_CRA_REG_M,reg_m);
		  if(err){

			  return ATP_ERROR_HARDWARE_COMMUNICATION;
		  }




		return ATP_SUCCESS;


}

typedef struct {
	float x;
	float y;
	float z;
}mag_values;

em_int32 adafruit_mag_read(mag_values *values){
	em_uint32 err;
			em_byte data[6];
			data[0]=LSM303_REGISTER_MAG_OUT_X_H_M;
			err=lsm303_mag_write8(LSM303_REGISTER_MAG_OUT_X_H_M);
			 if(err){
				     	return ATP_ERROR_HARDWARE_COMMUNICATION;
			 }
			 em_uint32 lenght=6;
			 err=em_io_i2c_read(EM_USE_BSC1,LSM303_ADDRESS_MAG,data,&lenght);
				  if(err){
				  	 	return ATP_ERROR_HARDWARE_COMMUNICATION;
				  }

				  values->x = (int16_t)(data[1] | ((int16_t)data[0] << 8));
				  values->y = (int16_t)(data[3] | ((int16_t)data[2] << 8));
				  values->z = (int16_t)(data[5] | ((int16_t)data[4] << 8));
				  return ATP_SUCCESS;
}


em_int32 adafruit_lsm303_mag_read(float *values){
	  em_int32 reading_valid = 0;
	  em_byte data[6];
	  em_uint32 err;
	  mag_values _magData;
	  em_int32 length;
	    while(!reading_valid){
	    	//read data from register and check
	    	data[0]=LSM303_REGISTER_MAG_SR_REG_Mg;
	    	err=lsm303_mag_write8(LSM303_REGISTER_MAG_SR_REG_Mg);
	    	if(err)
	    		return ATP_ERROR_HARDWARE_COMMUNICATION;
	    	length=1;
	    	err=lsm303_mag_read8(data);
	    	if(err  || !(data[0] & 0x1) ) {
	    				return ATP_ERROR_HARDWARE_COMMUNICATION;
	    	    }
	    	err=adafruit_mag_read(&_magData);
	    	if(err)
	    		return ATP_ERROR_HARDWARE_COMMUNICATION;


	    	if ( (_magData.x >= 2040) | (_magData.x <= -2040) |
	    	           (_magData.y >= 2040) | (_magData.y <= -2040) |
	    	           (_magData.z >= 2040) | (_magData.z <= -2040) )
	    	      {
	    	        /* Saturating .... increase the range if we can */
	    	        switch(_magGain)
	    	        {
	    	          case LSM303_MAGGAIN_5_6:
	    	            setMagGain(LSM303_MAGGAIN_8_1);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 8.1");
	    	#endif
	    	            break;
	    	          case LSM303_MAGGAIN_4_7:
	    	            setMagGain(LSM303_MAGGAIN_5_6);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 5.6");
	    	#endif
	    	            break;
	    	          case LSM303_MAGGAIN_4_0:
	    	            setMagGain(LSM303_MAGGAIN_4_7);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 4.7");
	    	#endif
	    	            break;
	    	          case LSM303_MAGGAIN_2_5:
	    	            setMagGain(LSM303_MAGGAIN_4_0);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 4.0");
	    	#endif
	    	            break;
	    	          case LSM303_MAGGAIN_1_9:
	    	            setMagGain(LSM303_MAGGAIN_2_5);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 2.5");
	    	#endif
	    	            break;
	    	          case LSM303_MAGGAIN_1_3:
	    	            setMagGain(LSM303_MAGGAIN_1_9);
	    	            reading_valid = 0;
	    	#ifdef LSM303_DEBUG
	    	            Serial.println("Changing range to +/- 1.9");
	    	#endif
	    	            break;
	    	          default:
	    	        	  reading_valid = 1;
	    	            break;
	    	        }
	    	      }
	    	      else
	    	      {
	    	        /* All values are withing range */
	    	    	  reading_valid = 1;
	    	      }

	    }

	    values[0]= _magData.x / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
	    values[1] = _magData.y / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
	    values[2] = _magData.z / _lsm303Mag_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;






	   return ATP_SUCCESS;
   }

em_int32 adafruit_lsm303_mag_stop(void * param){
	   return ATP_SUCCESS;
   }




