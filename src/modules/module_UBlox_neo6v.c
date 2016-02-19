/*
 * module_UBlox_neo6v.c
 *
 *  Created on: Feb 9, 2016
 *      Author: root
 */
#include "module_UBlox_neo6v.h"



em_uint32 ublox_neo6v_start(void *parameters){
	em_uint32 err;
	//these parameters are so important, otherwise cannot communicate with ublox gps
			err=em_io_uart_start(EM_UART_FIF0_ENABLE|EM_UART_DATA_8BIT_ENABLE|EM_UART_RECEIVE_ENABLE|EM_UART_TRANSMIT_ENABLE,115200);
			if(err)
			{
				atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting UART failed Errno:%u \n",err));
				return ATP_ERROR_HARDWARE_UART_START;
			}
			return ATP_SUCCESS;
}

em_uint32 ublox_neo6v_setconfig(){



	 const char *config="MON-VER - 0A 04 28 00 37 2E 30 33 20 28 34 35 39 36 39 29 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 30 30 30 34 30 30 30 37 00 00#\
	CFG-ANT - 06 13 04 00 1B 00 8B A9#\
	CFG-DAT - 06 06 02 00 00 00#\
	CFG-FXN - 06 0E 24 00 0C 00 00 00 00 00 00 00 00 00 00 00 10 27 00 00 10 27 00 00 D0 07 00 00 18 FC FF FF 00 00 00 00 00 00 00 00#\
	CFG-INF - 06 02 0A 00 00 00 00 00 00 00 00 00 00 00#\
	CFG-INF - 06 02 0A 00 01 00 00 00 87 87 87 87 87 87#\
	CFG-INF - 06 02 0A 00 03 00 00 00 00 00 00 00 00 00#\
	CFG-ITFM - 06 39 08 00 F3 AC 62 2D 1E 03 00 00#\
	CFG-MSG - 06 01 08 00 0B 30 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0B 32 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0B 33 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0B 31 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0B 01 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 0B 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 09 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 02 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 06 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 07 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 21 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0A 08 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 60 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 22 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 31 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 04 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 01 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 02 00 01 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 32 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 06 00 01 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 03 00 01 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 30 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 20 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 21 00 01 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 11 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 01 12 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 02 20 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0D 03 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 0D 01 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F0 00 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 01 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 02 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 03 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 04 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 05 01 00 01 01 01 01#\
	CFG-MSG - 06 01 08 00 F0 06 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F0 07 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F0 08 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F0 09 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F0 0A 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F1 00 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F1 03 00 00 00 00 00 00#\
	CFG-MSG - 06 01 08 00 F1 04 00 00 00 00 00 00#\
	CFG-NAV5 - 06 24 24 00 FF FF 08 03 00 00 00 00 10 27 00 00 05 00 FA 00 FA 00 64 00 2C 01 00 3C 00 00 00 00 00 00 00 00 00 00 00 00#\
	CFG-NAVX5 - 06 23 28 00 00 00 FF FF 03 00 00 00 03 02 03 10 07 00 00 01 00 00 43 06 00 00 00 00 01 01 00 00 00 64 78 00 00 00 00 00 00 00 00 00#\
	CFG-NMEA - 06 17 04 00 00 23 00 02#\
	CFG-PM - 06 32 18 00 00 06 00 00 04 90 00 00 E8 03 00 00 10 27 00 00 00 00 00 00 02 00 00 00#\
	CFG-PM2 - 06 3B 2C 00 01 06 00 00 00 90 02 00 E8 03 00 00 10 27 00 00 00 00 00 00 02 00 00 00 2C 01 00 00 4F C1 03 00 86 02 00 00 FE 00 00 00 64 40 01 00#\
	CFG-PRT - 06 00 14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00#\
	CFG-PRT - 06 00 14 00 01 00 00 00 C0 08 00 00 00 C2 01 00 01 00 01 00 00 00 00 00#\
	CFG-PRT - 06 00 14 00 02 00 00 00 C0 08 00 00 80 25 00 00 00 00 00 00 00 00 00 00#\
	CFG-PRT - 06 00 14 00 03 00 00 00 00 00 00 00 00 00 00 00 07 00 07 00 00 00 00 00#\
	CFG-PRT - 06 00 14 00 04 00 00 00 00 32 00 00 00 00 00 00 07 00 07 00 00 00 00 00#\
	CFG-RATE - 06 08 06 00 C8 00 01 00 01 00#\
	CFG-RINV - 06 34 18 00 00 4E 6F 74 69 63 65 3A 20 6E 6F 20 64 61 74 61 20 73 61 76 65 64 21 00#\
	CFG-RXM - 06 11 02 00 08 00#\
	CFG-SBAS - 06 16 08 00 01 03 03 00 51 62 06 00#\
	CFG-TP - 06 07 14 00 40 42 0F 00 A0 86 01 00 01 01 00 00 32 00 00 00 00 00 00 00#\
	CFG-TP5 - 06 31 20 00 00 00 00 00 32 00 00 00 40 42 0F 00 40 42 0F 00 00 00 00 00 A0 86 01 00 00 00 00 00 F7 00 00 00#\
	CFG-TP5 - 06 31 20 00 01 00 00 00 32 00 00 00 04 00 00 00 01 00 00 00 48 E8 01 00 A0 86 01 00 00 00 00 00 FE 00 00 00#\
	CFG-USB - 06 1B 6C 00 46 15 A6 01 00 00 00 00 64 00 00 01 75 2D 62 6C 6F 78 20 41 47 20 2D 20 77 77 77 2E 75 2D 62 6C 6F 78 2E 63 6F 6D 00 00 00 00 00 00 75 2D 62 6C 6F 78 20 36 20 20 2D 20 20 47 50 53 20 52 65 63 65 69 76 65 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00#";


		 char splitter1='#';
		 char splitter2='-';
		 char splitter3=' ';
		 em_int32 len= strlen(config);
		 atp_log(atp_log_create_string(ATP_LOG_INFO,"Setting Gps Settings\n"));
		 em_int32 temp=0;
		 em_uint32 index=2;
		 em_uint8 bytes[32768];
		 bytes[0]=0xB5;
		 bytes[1]=0x62;
		 em_uint8 ck_a;
		 em_uint8 ck_b;
		 em_uint8 started=0;
		 char temp_buf[4];
		 for(temp=0;temp<len;++temp){

			 if(config[temp]==splitter1){

				 started=0;
				 calculate_ck(bytes+2,index-2,&ck_a,&ck_b);
				 bytes[index++]=ck_a;
				 bytes[index]=ck_b;

				 em_uint32 counter=0;
				 while(counter<=index){
					 if(em_io_uart_write(bytes[counter])==EM_SUCCESS){

						 counter++;
					}else{
						//bu pek anlamlı değil zaten success olana kadar deniyoruz
						//atp_log(atp_log_create_string(ATP_LOG_INFO,"Could Not  Write Gps\n"));
						//em_io_delay_microseconds(100);
					}
				 }

				 index=2;



			 }else{
				 if(started==2)
				 {

					 if(config[temp]==splitter3)
						 continue;
					 temp_buf[0]='0';
					 temp_buf[1]='x';
					 temp_buf[2]=config[temp++];
					 temp_buf[3]=config[temp];
					 bytes[index++]=strtol(temp_buf,NULL,16);

				 }else
				 if(config[temp]==splitter2)
					 started++;


			 }
		 }

		 return ATP_SUCCESS;

}

#define read_next_byte()      while(*work){ if (em_io_uart_read(&readed) == EM_SUCCESS) break;}
em_uint32 ublox_neo6v_read_packet(struct ubx *packet,const em_uint8 *work){

	em_uint8 readed;
	read_next_byte();

				if(readed == 0xB5)
					packet->sync1 = readed;
				else{

					return ATP_ERROR_HARDWARE_COMMUNICATION;
				}

			read_next_byte();


				if(readed == 0x62)
					packet->sync2 = readed;
				else {

					return ATP_ERROR_HARDWARE_COMMUNICATION;

				}


			read_next_byte();
			packet->class = readed;



		    read_next_byte();
			packet->id = readed;



			read_next_byte();
			packet->lenght = readed;

			read_next_byte();
			packet->lenght += ((em_uint16)readed) << 8;

	        if(packet->lenght>100)
	        	return ATP_ERROR_HARDWARE_COMMUNICATION;
			em_uint16 lenght = packet->lenght;
					while (*work & lenght) {

						read_next_byte();
							packet->data[packet->lenght - lenght] = readed;
							lenght--;

						}
					read_next_byte();
					packet->clk_a = readed;
					read_next_byte();
					packet->clk_b = readed;

					return ATP_SUCCESS;



}




