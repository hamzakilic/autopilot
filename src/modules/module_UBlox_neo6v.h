/*
 * module_UBlox-neo6v.h
 *
 *  Created on: Feb 9, 2016
 *      Author: root
 */

#ifndef MODULE_UBLOX_NEO6V_H_
#define MODULE_UBLOX_NEO6V_H_

#include "../common/atp_common.h"
#include "../log/atp_log.h"


struct ubx {
	em_uint8 sync1;
	em_uint8 sync2;
	em_uint8 class;
	em_uint8 id;
	em_uint16 lenght;
	em_uint8 data[65535];
	em_uint8 clk_a;
	em_uint8 clk_b;
};
//no need alignment on arm
struct nav_posllh{//0x01 0x02
	em_uint32 iTOW;
	em_int32 lon;
	em_int32 lat;
	em_int32 height;
	em_int32 hMSL;
	em_uint32 hAcc;
	em_uint32 vAcc;
};
struct nav_timeutc{
	em_uint32 iTOW;
	em_uint32 tAcc;
	em_int32  nano;
	em_uint16 year;
	em_uint8 month;
	em_uint8 day;
	em_uint8 hour;
	em_uint8 min;
	em_uint8 sec;
	em_uint8 valid;

};

struct nav_status{
	em_uint32 iTOW;
	em_uint8 gpsFix;
	em_uint8 flags;
	em_uint8 fixStat;
	em_uint8 flags2;
	em_uint32 ttff;
	em_uint32 msss;
};
struct nav_sol{
	em_uint32 iTOW;
	em_int32 fTOW;
	em_int16 week;
	em_uint8 gpsFix;
	em_uint8 flags;
	em_int32 ecefX;
	em_int32 ecefY;
	em_int32 ecefZ;
	em_uint32 pAcc;
	em_int32 ecefVX;
	em_int32 ecefVY;
	em_int32 ecefVZ;
	em_uint32 sAcc;
	em_uint16 pDOP;
	em_uint8 reserved1;
	em_uint8 numSV;
	em_uint32 reserved2;

};


/**
 * @brief starts gps devices
 */
em_uint32 ublox_neo6v_start(void *parameters);
/**
 * @brief sets default configuration
 */
em_uint32 ublox_neo6v_setconfig();



em_uint32 ublox_neo6v_read_packet(struct ubx *packet,const em_uint8 *work);


#endif /* MODULE_UBLOX_NEO6V_H_ */
