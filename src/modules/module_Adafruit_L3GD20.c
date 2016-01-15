/*
 * module_Adafruit_L3GD20.c
 *
 *
 */

#include "module_Adafruit_L3GD20.h"

static gyroRange_t _range;

inline em_uint32 l3gd20_read8(em_byte *data){

    em_int32 length=1;
    return em_io_i2c_read(EM_USE_BSC1,L3GD20_ADDRESS,data,&length);
}
inline em_uint32 l3gd20_read16(em_byte *data){

    em_int32 length=2;
    return em_io_i2c_read(EM_USE_BSC1,L3GD20_ADDRESS,data,&length);
}

inline em_uint32 l3gd20_write8(em_byte val){
	em_byte data[1];
	data[0]=val;
	return em_io_i2c_write(EM_USE_BSC1,L3GD20_ADDRESS,data,1);
}

inline em_uint32 l3gd20_write16(em_byte reg,em_byte val){
	em_byte data[2];
	data[0]=reg;
	data[1]=val;
	return em_io_i2c_write(EM_USE_BSC1,L3GD20_ADDRESS,data,2);
}


 em_uint32 adafruit_l3gd20_gyro_start(void *parameters){
        	em_uint32 err;
        	em_byte data[6];
        	/* Make sure we have the correct chip ID since this checks
        	     for correct address and that the IC is properly connected */
        	data[0]=GYRO_REGISTER_WHO_AM_I;
        	err=l3gd20_write8(GYRO_REGISTER_WHO_AM_I);
        	if(err)	return ATP_ERROR_HARDWARE_COMMUNICATION;

        	em_int32 length=1;
        	err=l3gd20_read8(data);
        	if(err) return ATP_ERROR_HARDWARE_COMMUNICATION;

        	if ((data[0] != L3GD20_ID) && (data[0] != L3GD20H_ID))
        	  {
        	    return ATP_ERROR_HARDWARE_INITIALIZE;
        	  }

        	/* Set CTRL_REG1 (0x20)
        	   ====================================================================
        	   BIT  Symbol    Description                                   Default
        	   ---  ------    --------------------------------------------- -------
        	   7-6  DR1/0     Output data rate                                   00
        	   5-4  BW1/0     Bandwidth selection                                00
        	     3  PD        0 = Power-down mode, 1 = normal/sleep mode          0
        	     2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
        	     1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
        	     0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1 */

        	  /* Reset then switch to normal mode and enable all three channels */
        	  err=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x00);
        	  if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;

        	  err=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x0F);
        	  if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        	  /* ------------------------------------------------------------------ */

        	  /* Set CTRL_REG2 (0x21)
        	   ====================================================================
        	   BIT  Symbol    Description                                   Default
        	   ---  ------    --------------------------------------------- -------
        	   5-4  HPM1/0    High-pass filter mode selection                    00
        	   3-0  HPCF3..0  High-pass filter cutoff frequency selection      0000 */

        	  /* Nothing to do ... keep default values */
        	  /* ------------------------------------------------------------------ */

        	  /* Set CTRL_REG3 (0x22)
        	   ====================================================================
        	   BIT  Symbol    Description                                   Default
        	   ---  ------    --------------------------------------------- -------
        	     7  I1_Int1   Interrupt enable on INT1 (0=disable,1=enable)       0
        	     6  I1_Boot   Boot status on INT1 (0=disable,1=enable)            0
        	     5  H-Lactive Interrupt active config on INT1 (0=high,1=low)      0
        	     4  PP_OD     Push-Pull/Open-Drain (0=PP, 1=OD)                   0
        	     3  I2_DRDY   Data ready on DRDY/INT2 (0=disable,1=enable)        0
        	     2  I2_WTM    FIFO wtrmrk int on DRDY/INT2 (0=dsbl,1=enbl)        0
        	     1  I2_ORun   FIFO overrun int on DRDY/INT2 (0=dsbl,1=enbl)       0
        	     0  I2_Empty  FIFI empty int on DRDY/INT2 (0=dsbl,1=enbl)         0 */

        	  /* Nothing to do ... keep default values */
        	  /* ------------------------------------------------------------------ */

        	  /* Set CTRL_REG4 (0x23)
        	   ====================================================================
        	   BIT  Symbol    Description                                   Default
        	   ---  ------    --------------------------------------------- -------
        	     7  BDU       Block Data Update (0=continuous, 1=LSB/MSB)         0
        	     6  BLE       Big/Little-Endian (0=Data LSB, 1=Data MSB)          0
        	   5-4  FS1/0     Full scale selection                               00
        	                                  00 = 250 dps
        	                                  01 = 500 dps
        	                                  10 = 2000 dps
        	                                  11 = 2000 dps
        	     0  SIM       SPI Mode (0=4-wire, 1=3-wire)                       0 */

        	  /* Adjust resolution if requested */
        	  _range=GYRO_RANGE_500DPS;

        	  switch(_range)
        	  {
        	    case GYRO_RANGE_250DPS:
        	      err=l3gd20_write16(GYRO_REGISTER_CTRL_REG4, 0x00);
        	      if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        	      break;
        	    case GYRO_RANGE_500DPS:
        	    	err=l3gd20_write16(GYRO_REGISTER_CTRL_REG4, 0x10);
        	    	if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        	      break;
        	    case GYRO_RANGE_2000DPS:
        	    	err=l3gd20_write16(GYRO_REGISTER_CTRL_REG4, 0x20);
        	    	if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        	      break;
        	  }
        	  /* ------------------------------------------------------------------ */

        	  /* Set CTRL_REG5 (0x24)
        	   ====================================================================
        	   BIT  Symbol    Description                                   Default
        	   ---  ------    --------------------------------------------- -------
        	     7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
        	     6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
        	     4  HPen      High-pass filter enable (0=disable,1=enable)        0
        	   3-2  INT1_SEL  INT1 Selection config                              00
        	   1-0  OUT_SEL   Out selection config                               00 */

        	  /* Nothing to do ... keep default values */
        	  /* ------------------------------------------------------------------ */





        	return ATP_SUCCESS;
        }

  typedef struct{
	  em_float32 x;
	  em_float32 y;
	  em_float32 z;
  }gyro_data;

  inline em_uint32 adafruit_gyro_read(gyro_data *gyro){
	  em_byte data[6];
	  em_int32 lenght=6;
	  em_uint32 err;
	  err=l3gd20_write8(GYRO_REGISTER_OUT_X_L | 0x80);
	  if(err)return err;
	  err=em_io_i2c_read(EM_USE_BSC1,L3GD20_ADDRESS,data,&lenght);
	  if(err) return err;
	  gyro->x = data[0] | ((em_int32)data[1] << 8);
	  gyro->y = data[2] | ((em_int32)data[3] << 8);
	  gyro->z = data[4] | ((em_int32)data[5] << 8);
	  return ATP_SUCCESS;

  }


        em_uint32 adafruit_l3gd20_gyro_read(float *values){
        	em_int32 reading_valid=0;
        	gyro_data gyro;
        	em_uint32 err;
        	while(!reading_valid){
        		err=adafruit_gyro_read(&gyro);
        		/* Check if the sensor is saturating or not */
        		      if ( (gyro.x >= 32760) | (gyro.x <= -32760) |
        		           (gyro.y >= 32760) | (gyro.y <= -32760) |
        		           (gyro.z >= 32760) | (gyro.z <= -32760) )
        		      {
        		        /* Saturating .... increase the range if we can */
        		        switch(_range)
        		        {
        		          case GYRO_RANGE_500DPS:
        		            /* Push the range up to 2000dps */
        		            _range = GYRO_RANGE_2000DPS;
        		            err=0;//do every operation, | every result, at the end check err
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x00);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x0F);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG4, 0x20);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG5, 0x80);
        		            if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        		            reading_valid = 0;
        		            // Serial.println("Changing range to 2000DPS");
        		            break;
        		          case GYRO_RANGE_250DPS:
        		            /* Push the range up to 500dps */
        		            _range = GYRO_RANGE_500DPS;
        		            err=0;//do every operation, | every result, at the end check err
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x00);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG1, 0x0F);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG4, 0x10);
        		            err|=l3gd20_write16(GYRO_REGISTER_CTRL_REG5, 0x80);
        		            if(err)return ATP_ERROR_HARDWARE_COMMUNICATION;
        		            reading_valid = 0;
        		            // Serial.println("Changing range to 500DPS");
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

        	switch(_range)
        	  {
        	    case GYRO_RANGE_250DPS:
        	      gyro.x *= GYRO_SENSITIVITY_250DPS;
        	      gyro.y *= GYRO_SENSITIVITY_250DPS;
        	      gyro.z *= GYRO_SENSITIVITY_250DPS;
        	      break;
        	    case GYRO_RANGE_500DPS:
        	      gyro.x *= GYRO_SENSITIVITY_500DPS;
        	      gyro.y *= GYRO_SENSITIVITY_500DPS;
        	      gyro.z *= GYRO_SENSITIVITY_500DPS;
        	      break;
        	    case GYRO_RANGE_2000DPS:
        	      gyro.x *= GYRO_SENSITIVITY_2000DPS;
        	      gyro.y *= GYRO_SENSITIVITY_2000DPS;
        	      gyro.z *= GYRO_SENSITIVITY_2000DPS;
        	      break;
        	  }

        	  /* Convert values to rad/s */
        	  gyro.x *= SENSORS_DPS_TO_RADS;
        	  gyro.y *= SENSORS_DPS_TO_RADS;
        	  gyro.z *= SENSORS_DPS_TO_RADS;

        	values[0]=gyro.x;
        	values[1]=gyro.y;
        	values[2]=gyro.z;
        	return ATP_SUCCESS;
        }

        em_uint32 adafruit_l3gd20_gyro_stop(void *parameters){
        	return ATP_SUCCESS;
        }




