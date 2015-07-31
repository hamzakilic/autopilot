/*
 * atp_services_gps.c
 *
 *  Created on: May 12, 2015
 *      Author: root
 */
#include "atp_services_gps.h"

#ifdef COMPILE_MODULE_GPS_NEO_6V

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

//#define read_next_byte()      while(work){ if (em_io_mini_uart_read(&readed) == EM_SUCCESS) break;else em_io_delay_microseconds(3); }
#define read_next_byte()      while(work){ if (em_io_uart_read(&readed) == EM_SUCCESS) break; }


static em_uint8 work = 0;
static em_uint32 log_count=0;
void print(const struct ubx *data){

 atp_log(atp_log_create_string(ATP_LOG_INFO,"class=%u id=%u length=%u log=%d \n",data->class,data->id,data->lenght,log_count++));

}
void print2(const struct nav_posllh *data){
	atp_log(atp_log_create_string(ATP_LOG_INFO,"milisecond=%u lot=%d lat=%d height=%d hsea=%d hacc=%u vacc=%u log=%d \n",data->iTOW,data->lon,data->lat,data->height,data->hMSL,data->hAcc,data->vAcc,log_count++));
}

void print3(const struct nav_timeutc *data){
	atp_log(atp_log_create_string(ATP_LOG_INFO,"milisecond=%u accuracy=%d nano=%d year=%u month=%u day=%u hour=%u min=%u sec=%u valid=%u log=%d \n",data->iTOW,data->tAcc,data->nano,data->year,data->month,data->day,data->hour,data->min,data->sec,data->sec,log_count++));
}

void print4(const struct nav_status *data){
	atp_log(atp_log_create_string(ATP_LOG_INFO,"milisecond=%u gpsFix=%u mss=%u log=%d \n",data->iTOW,data->gpsFix,data->msss,log_count++));
}
void print5(const struct nav_sol *data){
	atp_log(atp_log_create_string(ATP_LOG_INFO,"accuracy estimate=%u speed=%u  log=%d \n",data->pAcc,data->sAcc,log_count++));
}



inline void calculate_ck(em_uint8 *data,em_uint32 lenght,em_uint8 *ck_a,em_uint8 *ck_b){
	em_uint32 index=0;
	*ck_a=0;
	*ck_b=0;
	for(index=0;index<lenght;++index){
		*ck_a += data[index];
		*ck_b += *ck_a;
	}

}
inline void calculate_packet_ck(struct ubx *packet,em_uint8 *ck_a,em_uint8 *ck_b){
	em_uint32 index=0;
	em_uint8 ck_a_temp=0;
	em_uint8 ck_b_temp=0;
	ck_a_temp+=packet->class;
    ck_b_temp+=ck_a_temp;
    ck_a_temp+=packet->id;
    ck_b_temp+=ck_a_temp;
    ck_a_temp+=(packet->lenght);
    ck_b_temp+=ck_a_temp;
    ck_a_temp+=(packet->lenght>>8);
    ck_b_temp+=ck_a_temp;

	for(index=0;index<packet->lenght;++index){
		ck_a_temp+=packet->data[index];
		ck_b_temp+=ck_a_temp;
	}
	*ck_a=ck_a_temp;
	*ck_b=ck_b_temp;


}
void set_config(){
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
				//if(em_io_mini_uart_write(bytes[counter])==EM_SUCCESS){
				 if(em_io_uart_write(bytes[counter])==EM_SUCCESS){
					 counter++;
				}else{
					atp_log(atp_log_create_string(ATP_LOG_INFO,"Could Not  Write Gps\n"));
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

}

atp_queue *packet_queue;
void * process_queue_lock;


void add_process_queue(const struct ubx *packet){
	struct ubx *temp=atp_malloc(sizeof(struct ubx));
	memcpy(temp,packet,sizeof(struct ubx));
   atp_thread_lock(&process_queue_lock);
   atp_queue_push(packet_queue,(void *)temp);
   atp_thread_unlock(&process_queue_lock);
}

void *process_packet_queue(void *ptr){
	while(work){

		if(atp_queue_count(packet_queue)==0){
			em_io_delay_microseconds(10000);
			continue;
		}
		atp_thread_lock(&process_queue_lock);
        struct ubx *packet=(struct ubx  *)atp_queue_pop(packet_queue);
        atp_thread_unlock(&process_queue_lock);
		print(packet);
							em_uint8 clk_a,clk_b;
							calculate_packet_ck(packet,&clk_a,&clk_b);
							if(packet->clk_a==clk_a && packet->clk_b==clk_b ){
								if(packet->class==0x01 && packet->id==0x02){

									struct nav_posllh nav;
									if(packet->lenght>=28){
										memcpy(&nav,packet->data,28);
										//print2(&nav);
									}
								}
								if(packet->class==0x01 && packet->id==0x21){
								      struct nav_timeutc timeutc;
										if(packet->lenght>=20){
										   memcpy(&timeutc,packet->data,20);
											//print3(&timeutc);
										}
									}

								if(packet->class==0x01 && packet->id==0x03){
									struct nav_status status;
									if(packet->lenght>=16){
									    memcpy(&status,packet->data,16);
									    //print4(&status);
									    }
								}
								if(packet->class==0x01 && packet->id==0x03){
									struct nav_sol sol;
									if(packet->lenght>=52){
										memcpy(&sol,packet->data,52);
										//print5(&sol);

										}
								}




							}
        atp_free(packet);


	}
}

void * start_communication(void *ptr) {
	atp_input *input = (atp_input *) ptr;


    set_config();


	struct ubx packet;
	memset(&packet, 0, sizeof(struct ubx));
	em_uint8 readed;

	while (work) {

		read_next_byte();

			if(readed == 0xB5)
				packet.sync1 = readed;
			else{
				//atp_log(atp_log_create_string(ATP_LOG_INFO,"problem 1 \n"));
				continue;
			}

		read_next_byte();

			if(readed == 0x62)
				packet.sync2 = readed;
			else {
				//atp_log(atp_log_create_string(ATP_LOG_INFO,"problem 2 \n"));
				continue;

			}


		read_next_byte();
		 packet.class = readed;



	    read_next_byte();
		packet.id = readed;



		read_next_byte();
		packet.lenght = readed;

		read_next_byte();
		packet.lenght += ((em_uint16)readed) << 8;

        if(packet.lenght>100)
        	continue;
		em_uint16 lenght = packet.lenght;
				while (work && lenght) {

					read_next_byte();
						packet.data[packet.lenght - lenght] = readed;
						lenght--;

					}
				read_next_byte();
				packet.clk_a = readed;
				read_next_byte();
				packet.clk_b = readed;
				//print(&packet);
				add_process_queue(&packet);


		}


	return ATP_SUCCESS;
	}

static atp_thread_id thread_communication_id;
static atp_thread_id thread_queue_id;

em_uint32 atp_services_gps_create(atp_services_gps **address, atp_input *input) {


	//em_io_mini_uart_start(EM_MINI_UART_RECEIVE_ENABLE | EM_MINI_UART_TRANSMIT_ENABLE| EM_MINI_UART_DATA_8BIT_ENABLE, 115200, 250);
		em_uint32 err;
		err=em_io_uart_start(EM_UART_FIF0_ENABLE|EM_UART_DATA_8BIT_ENABLE|EM_UART_RECEIVE_ENABLE|EM_UART_TRANSMIT_ENABLE,115200);
		if(err)
		{
			atp_log(atp_log_create_string(ATP_LOG_FATAL,"Starting UART failed Errno:%u \n",err));
			return ATP_ERROR_HARDWARE_UART_START;
		}


    packet_queue= atp_queue_create();


    err=atp_thread_create_lock(&process_queue_lock);
    if (err) {
    		atp_log(
    				atp_log_create_string(ATP_LOG_FATAL,
    						"Create Mutex For Gps Failed Error:%u\n", err));
    		work=0;
    		return ATP_ERROR_CREATE_GPS;
    	}

	work = 1;
	err = atp_thread_create(&thread_communication_id, start_communication, input);
	if (err) {
		atp_log(
				atp_log_create_string(ATP_LOG_FATAL,
						"Create Thread For Gps Failed Error:%u\n", err));
		work=0;
		return ATP_ERROR_CREATE_GPS;
	}
	err=atp_thread_create(&thread_queue_id, process_packet_queue,NULL);
	if (err) {
			atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Thread For Gps PacketQueue Failed Error:%u\n", err));
			work=0;
			return ATP_ERROR_CREATE_GPS;
		}

	return ATP_SUCCESS;
}
em_uint32 atp_services_gps_destroy(atp_services_gps **address) {
	work=0;
    atp_thread_join(&thread_communication_id);
    atp_thread_join(&thread_queue_id);
    atp_queue_destroy(packet_queue);
	return ATP_SUCCESS;
}

#endif
