/*
 * atp_services_gps.c
 *
 *  Created on: May 12, 2015
 *      Author: root
 */
#include "atp_services_gps.h"


typedef struct {
	atp_queue *packet_queue;
	void * process_queue_lock;
	atp_thread_id thread_communication_id;
	atp_thread_id thread_queue_id;
	atp_input *input;

}atp_services_gps_data;

#ifdef COMPILE_MODULE_GPS_NEO_6V



//#define read_next_byte()      while(work){ if (em_io_mini_uart_read(&readed) == EM_SUCCESS) break;else em_io_delay_microseconds(3); }



static em_uint8 work = 0;
static em_uint32 log_count=0;

inline void print(const struct ubx *data){
#ifdef COMPILE_DEBUG_CODES
 atp_log(atp_log_create_string(ATP_LOG_DEBUG,"class=%u id=%u length=%u log=%d \n",data->class,data->id,data->lenght,log_count++));
#endif


}
inline void print2(const struct nav_posllh *data){
#ifdef COMPILE_DEBUG_CODES
	atp_log(atp_log_create_string(ATP_LOG_DEBUG,"milisecond=%u lot=%d lat=%d height=%d hsea=%d hacc=%u vacc=%u log=%d \n",data->iTOW,data->lon,data->lat,data->height,data->hMSL,data->hAcc,data->vAcc,log_count++));
#endif

}

inline void print3(const struct nav_timeutc *data){
#ifdef COMPILE_DEBUG_CODES
	atp_log(atp_log_create_string(ATP_LOG_DEBUG,"milisecond=%u accuracy=%d nano=%d year=%u month=%u day=%u hour=%u min=%u sec=%u valid=%u log=%d \n",data->iTOW,data->tAcc,data->nano,data->year,data->month,data->day,data->hour,data->min,data->sec,data->sec,log_count++));
#endif

}

inline void print4(const struct nav_status *data){
#ifdef COMPILE_DEBUG_CODES
	atp_log(atp_log_create_string(ATP_LOG_DEBUG,"milisecond=%u gpsFix=%u mss=%u log=%d \n",data->iTOW,data->gpsFix,data->msss,log_count++));
#endif

}
inline void print5(const struct nav_sol *data){
#ifdef COMPILE_DEBUG_CODES
	atp_log(atp_log_create_string(ATP_LOG_DEBUG,"accuracy estimate=%u speed=%u  log=%d \n",data->pAcc,data->sAcc,log_count++));
#endif

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


void * starttest(void *data){

}

void add_process_queue(atp_services_gps_data *data, const struct ubx *packet){
	struct ubx *temp=atp_malloc(sizeof(struct ubx));
	memcpy(temp,packet,sizeof(struct ubx));
   atp_thread_lock(data->process_queue_lock);
   atp_queue_push(data->packet_queue,(void *)temp);
   atp_thread_unlock(data->process_queue_lock);
}

void *process_packet_queue(void *ptr){
	atp_services_gps_data *data=atp_convert(ptr,atp_services_gps_data*);

	while(work){

		if(atp_queue_count(data->packet_queue)==0){
			em_io_delay_microseconds(10000);
			continue;
		}
		atp_services_gps_data *data=atp_convert(ptr,atp_services_gps_data *);
		atp_thread_lock(data->process_queue_lock);
        struct ubx *packet=(struct ubx  *)atp_queue_pop(data->packet_queue);
        atp_thread_unlock(data->process_queue_lock);
		print(packet);
							em_uint8 clk_a,clk_b;
							calculate_packet_ck(packet,&clk_a,&clk_b);
							if(packet->clk_a==clk_a && packet->clk_b==clk_b ){
								if(packet->class==0x01 && packet->id==0x02){

									struct nav_posllh nav;
									if(packet->lenght>=28){
										memcpy(&nav,packet->data,28);
										print2(&nav);
										atp_gps_location_data location_data;
										location_data.latitude=nav.lat;
										location_data.longtitude=nav.lon;
										location_data.sealevel=nav.hMSL;

										atp_input_update_gps_location(data->input,location_data);
									}
								}
								if(packet->class==0x01 && packet->id==0x21){
								      struct nav_timeutc timeutc;
										if(packet->lenght>=20){
										   memcpy(&timeutc,packet->data,20);
											print3(&timeutc);
										   atp_gps_time_data time_data;
										   time_data.day=timeutc.day;
										   time_data.hour=timeutc.hour;
										   time_data.min=timeutc.min;
										   time_data.month=timeutc.month;
										   time_data.sec=timeutc.sec;
										   time_data.year=timeutc.year;
										   atp_input_update_gps_time(data->input,time_data);

										}
									}

								if(packet->class==0x01 && packet->id==0x03){
									struct nav_status status;
									if(packet->lenght>=16){
									    memcpy(&status,packet->data,16);
									    print4(&status);
									    atp_gps_location_ex_data location_ex_data;
									    location_ex_data.gps_fix=status.gpsFix;
									    location_ex_data.msss=status.msss;
									    location_ex_data.flags=status.flags;
									    atp_input_update_gps_location_ex(data->input,location_ex_data);
									    }
								}
								if(packet->class==0x01 && packet->id==0x06){
									struct nav_sol sol;
									if(packet->lenght>=52){
										memcpy(&sol,packet->data,52);
										print5(&sol);

										}
								}




							}
        atp_free(packet);


	}
}

void * start_communication_gps(void *ptr) {
	atp_services_gps_data *gps_data =atp_convert(ptr,atp_services_gps_data*);

	ublox_neo6v_setconfig();

	struct ubx packet;
	memset(&packet, 0, sizeof(struct ubx));

    em_uint32 err;
	while (work) {

		 err=ublox_neo6v_read_packet(&packet,&work);
		 if(!err){
//todo:adding to process queue is to late
	       add_process_queue(gps_data,&packet);

		 }
		 else{
#ifdef COMPILE_DEBUG_CODES
			 atp_log(atp_log_create_string(ATP_LOG_DEBUG,"Gps Next Packet Read Error:%d\n",err));

#endif

		 }


		}


	return ATP_SUCCESS;
	}

#endif



em_uint32 atp_services_gps_create(atp_services_gps **address, atp_input *input) {



		em_uint32 err;
#ifdef COMPILE_MODULE_GPS_NEO_6V
		ublox_neo6v_start(NULL);
#endif
    atp_services_gps * gps=atp_new(atp_services_gps);
    atp_fill_zero(gps,sizeof(atp_services_gps));
    atp_services_gps_data *gps_data=atp_new(atp_services_gps_data);
    atp_fill_zero(gps_data,sizeof(atp_services_gps_data));
    gps->private_data=gps_data;
    *address=gps;


    gps_data->packet_queue= atp_queue_create();
    gps_data->input=input;


    err=atp_thread_create_lock(&gps_data->process_queue_lock);
    if (err) {
    		atp_log(
    				atp_log_create_string(ATP_LOG_FATAL,
    						"Create Mutex For Gps Failed Error:%u\n", err));
    		work=0;
    		return ATP_ERROR_CREATE_GPS;
    	}

	work = 1;

	err = atp_thread_create(&gps_data->thread_communication_id, start_communication_gps, gps_data);
	if (err) {
		atp_log(
				atp_log_create_string(ATP_LOG_FATAL,
						"Create Thread For Gps Failed Error:%u\n", err));
		work=0;
		return ATP_ERROR_CREATE_GPS;
	}
	err=atp_thread_create(&gps_data->thread_queue_id, process_packet_queue,gps_data);
	if (err) {
			atp_log(atp_log_create_string(ATP_LOG_FATAL,"Create Thread For Gps PacketQueue Failed Error:%u\n", err));
			work=0;
			return ATP_ERROR_CREATE_GPS;
	}

	return ATP_SUCCESS;
}
em_uint32 atp_services_gps_destroy(atp_services_gps *address) {
	work=0;

	if(address->private_data){
		atp_services_gps_data *gps_data=atp_convert(address->private_data,atp_services_gps_data*);

		if(gps_data->thread_communication_id)
		atp_thread_join(&gps_data->thread_communication_id);


		if(gps_data->thread_queue_id)
        atp_thread_join(&gps_data->thread_queue_id);


		atp_queue_destroy(gps_data->packet_queue);

		if(gps_data->process_queue_lock)
    	atp_thread_destory_lock(gps_data->process_queue_lock);
	}
	return ATP_SUCCESS;
}


