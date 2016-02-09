/*
 * module_Adafruit_BMP085.c
 *
 *  Created on: Jan 11, 2016
 *      Author: root
 */

#include "module_Adafruit_BMP085.h"


inline em_uint32 bmp085_read8(em_byte *data){
    em_uint32 err;
    em_int32 length=1;
    atp_system_lock_i2c();
    err= em_io_i2c_read(EM_USE_BSC1,BMP085_ADDRESS,data,length,EM_TIMEOUT_HALF_SECOND);
    atp_system_unlock_i2c();
    return err;
}
inline em_uint32 bmp085_read16(em_uint16 *data){
   em_byte temp[2];
    em_int32 length=2;
    em_uint32 err;
    atp_system_lock_i2c();
    err= em_io_i2c_read(EM_USE_BSC1,BMP085_ADDRESS,temp,length,EM_TIMEOUT_HALF_SECOND);
    atp_system_unlock_i2c();
    *data= (temp[0]<<8) | (temp[1]);

    return err;
}
inline em_uint32 bmp085_write8(em_byte val){
	em_uint32 err;
	em_byte data[1];
	data[0]=val;
	atp_system_lock_i2c();
	err= em_io_i2c_write(EM_USE_BSC1,BMP085_ADDRESS,data,1,EM_TIMEOUT_HALF_SECOND);
	atp_system_unlock_i2c();
	return err;
}
inline em_uint32 bmp085_read16from(em_byte address,em_uint16 *value){
	    em_uint32 err;
	    err=bmp085_write8(address);
		if(err)return err;
		em_io_busy_wait(5000);
		return bmp085_read16(value);
}

inline em_uint32 bmp085_read24from(em_byte address,em_byte *data){
	 em_uint32 err;
		    err=bmp085_write8(address);
			if(err)return err;
			em_io_busy_wait(2000);
			atp_system_lock_i2c();
			    err= em_io_i2c_read(EM_USE_BSC1,BMP085_ADDRESS,data,3,EM_TIMEOUT_HALF_SECOND);
			atp_system_unlock_i2c();
			return err;


}





inline em_uint32 bmp085_write16(em_byte reg,em_byte val){
	em_uint32 err;
	em_byte data[2];
	data[0]=reg;
	data[1]=val;
	atp_system_lock_i2c();
	err= em_io_i2c_write(EM_USE_BSC1,BMP085_ADDRESS,data,2,EM_TIMEOUT_ONE_SECOND);
	atp_system_unlock_i2c();
	return err;
}

static em_uint8           _bmp085Mode;
static bmp085_calib_data _bmp085_coeffs;


/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
#define BMP085_USE_DATASHEET_VALS (0)
static em_uint32  read_coefficients(void)
{
	em_uint32 err=ATP_SUCCESS;
  #if BMP085_USE_DATASHEET_VALS
    _bmp085_coeffs.ac1 = 408;
    _bmp085_coeffs.ac2 = -72;
    _bmp085_coeffs.ac3 = -14383;
    _bmp085_coeffs.ac4 = 32741;
    _bmp085_coeffs.ac5 = 32757;
    _bmp085_coeffs.ac6 = 23153;
    _bmp085_coeffs.b1  = 6190;
    _bmp085_coeffs.b2  = 4;
    _bmp085_coeffs.mb  = -32768;
    _bmp085_coeffs.mc  = -8711;
    _bmp085_coeffs.md  = 2868;
    _bmp085Mode        = 0;
  #else
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC1, &_bmp085_coeffs.ac1);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC2, &_bmp085_coeffs.ac2);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC3, &_bmp085_coeffs.ac3);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC4,&_bmp085_coeffs.ac4);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC5, &_bmp085_coeffs.ac5);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_AC6, &_bmp085_coeffs.ac6);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_B1, &_bmp085_coeffs.b1);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_B2, &_bmp085_coeffs.b2);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_MB, &_bmp085_coeffs.mb);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_MC, &_bmp085_coeffs.mc);
    err |= bmp085_read16from(BMP085_REGISTER_CAL_MD, &_bmp085_coeffs.md);


  #endif
    return err;
}


static frame tempreature_frame;

static em_uint32 read_raw_temperature(em_int32 *temperature)
{
  #if BMP085_USE_DATASHEET_VALS
    *temperature = 27898;
  #else
    em_uint16 t;
    em_uint32 err;
    err=bmp085_write16(BMP085_REGISTER_CONTROL, BMP085_REGISTER_READTEMPCMD);
    if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
    em_io_busy_wait(1000);
    err=bmp085_read16from(BMP085_REGISTER_TEMPDATA, &t);

    if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;

    em_int32 i;
    	  for(i=1;i<DIMSIZE;++i){
    		  tempreature_frame.x_u16[i-1]=tempreature_frame.x_u16[i];
    	  }
    	  tempreature_frame.x_u16[DIMSIZE-1]=t;
    *temperature = find_median_u16(tempreature_frame.x_u16,DIMSIZE);


  #endif
    return ATP_SUCCESS;
}

static frame pressure_frame;
static em_uint32 read_raw_pressure(em_int32 *pressure)
{
  #if BMP085_USE_DATASHEET_VALS
    *pressure = 23843;
  #else
    em_uint8  temp[3];
    em_int32  p32;
    em_uint32 err;

    err=bmp085_write16(BMP085_REGISTER_CONTROL, BMP085_REGISTER_READPRESSURECMD + (_bmp085Mode << 6));
    if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
    switch(_bmp085Mode)
    {
      case BMP085_MODE_ULTRALOWPOWER:
    	  em_io_busy_wait(5000);
        break;
      case BMP085_MODE_STANDARD:
    	  em_io_busy_wait(8000);
        break;
      case BMP085_MODE_HIGHRES:
    	  em_io_busy_wait(14000);
        break;
      case BMP085_MODE_ULTRAHIGHRES:
      default:
    	  em_io_busy_wait(26000);
        break;
    }

    err=bmp085_read24from(BMP085_REGISTER_PRESSUREDATA,temp);
    if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
    p32 = (((em_int32)temp[0]<<16) | ((em_int32)temp[1]<<8) | ((em_int32)temp[2])) >>(8-_bmp085Mode);



    em_int32 i;
       	  for(i=1;i<DIMSIZE;++i){
       		  pressure_frame.x_i32[i-1]=pressure_frame.x_i32[i];
       	  }
       	pressure_frame.x_i32[DIMSIZE-1]=p32;
       *pressure =find_median_i32(pressure_frame.x_i32,DIMSIZE);

    //*pressure = p32;
    return  ATP_SUCCESS;
  #endif
}

static em_int32 computeB5(em_int32 ut) {
  em_int32 X1 = (ut - (em_int32)_bmp085_coeffs.ac6) * ((em_int32)_bmp085_coeffs.ac5) >> 15;
  em_int32 X2 = ((em_int32)_bmp085_coeffs.mc << 11) / (X1+(em_int32)_bmp085_coeffs.md);
  return X1 + X2;
}



static em_uint32 get_pressure(em_float32 *pressure)
{
  em_int32  ut = 0, up = 0, compp = 0;
  em_int32  x1, x2, b5, b6, x3, b3, p;
  em_uint32 b4, b7;
  em_uint32 err=ATP_SUCCESS;
  /* Get the raw pressure and temperature values */
  err=read_raw_temperature(&ut);
  err=read_raw_pressure(&up);

  /* Temperature compensation */
  b5 = computeB5(ut);

  /* Pressure compensation */
  b6 = b5 - 4000;
  x1 = (_bmp085_coeffs.b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (_bmp085_coeffs.ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((em_int32) _bmp085_coeffs.ac1) * 4 + x3) << _bmp085Mode) + 2) >> 2;
  x1 = (_bmp085_coeffs.ac3 * b6) >> 13;
  x2 = (_bmp085_coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (_bmp085_coeffs.ac4 * (em_uint32) (x3 + 32768)) >> 15;
  b7 = ((em_uint32) (up - b3) * (50000 >> _bmp085Mode));

  if (b7 < 0x80000000)
  {
    p = (b7 << 1) / b4;
  }
  else
  {
    p = (b7 / b4) << 1;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  compp = p + ((x1 + x2 + 3791) >> 4);

  /* Assign compensated pressure value */
  *pressure = compp/100.0;

  return err;
}



static em_uint32 get_temperature(em_float32 *temp)
{
  em_int32 UT, X1, X2, B5;     // following ds convention
  em_float32 t;
  em_uint32 err=ATP_SUCCESS;
  err=read_raw_temperature(&UT);

  #if BMP085_USE_DATASHEET_VALS
    // use datasheet numbers!
    UT = 27898;
    _bmp085_coeffs.ac6 = 23153;
    _bmp085_coeffs.ac5 = 32757;
    _bmp085_coeffs.mc = -8711;
    _bmp085_coeffs.md = 2868;
  #endif

  B5 = computeB5(UT);
  t = (B5+8) >> 4;
  t /= 10;
  //printf("%d %8.2f\n",UT,t);
  *temp = t;
  return err;
}








   em_uint32 adafruit_bmp085_temp_pres_start(void *parameter){
    	em_uint32 err;
    	atp_fill_zero(&tempreature_frame,sizeof(frame));
    	atp_fill_zero(&pressure_frame,sizeof(frame));

    	_bmp085Mode = BMP085_MODE_STANDARD;

    	err=bmp085_write8(BMP085_REGISTER_CHIPID);
    	if(err)		return ATP_ERROR_HARDWARE_COMMUNICATION;
    	em_uint8 id;
    	  err=bmp085_read8(&id);
    	  if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
    	  if(id != 0x55)   return ATP_ERROR_HARDWARE_INITIALIZE;
    	  err=read_coefficients();
    	  if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;

    return ATP_SUCCESS;
    }

    em_uint32 adafruit_bmp085_temp_press_read(em_float32 *temp,em_float32 *press){
    	em_uint32 err=ATP_SUCCESS;
    	err |=get_temperature(temp);
    	err |=get_pressure(press);

    	return err;

    }

    em_uint32 adafruit_bmp085_temp_press_read_raw(em_float32 *temp,em_float32 *press){
        	em_uint32 err=ATP_SUCCESS;
        	em_int32 t1,p1;

        	err |=read_raw_temperature(&t1);
        	err |=read_raw_pressure(&p1);
        	*temp=t1;
        	*press=p1;

        	return err;

        }

    em_uint32 adafruit_bmp085_temp_press_stop(void *param){
    	return ATP_SUCCESS;

    }


