/*
 * atp_task_balance.c
 *
 *  Created on: Apr 24, 2016
 *      Author: root
 */
#include "atp_task_balance.h"


typedef struct{
	atp_task_share *share;
	atp_motor_controller *motor_controller;
	atp_input *input;
	em_int32 work;
	em_int32 is_working;

}atp_task_balance;

struct atp_task * atp_task_balance_create(atp_task_share *task_share,atp_motor_controller *motor_controller,atp_input *input){

	struct atp_task *task=atp_new(struct atp_task);
	  atp_fill_zero(task,sizeof(struct atp_task));
	  task->func_exec=atp_task_balance_exec;
	  task->func_free=atp_task_balance_free;
	  task->func_kill=atp_task_balance_kill;
	  task->is_finished=0;
	  task->is_started=0;
	  task->thread_id=0;
	  task->name="Balance System";

	  atp_task_balance  *data=atp_new(atp_task_balance);
	    atp_fill_zero(data,sizeof(atp_task_balance));
	    data->motor_controller=motor_controller;
	    data->share=task_share;
	    data->input=input;
	    data->work=1;
	    data->is_working=0;

	    task->parameter=data;


	    return task;


}

void balance_rollup(atp_task_balance *data){

	em_int32 index;
		 atp_motor_data motors[ATP_MOTORS_COUNT]={0,0,0,0};
		 atp_ahrs_data ahrs_data;
		 em_int32 work=1;

	    // do{
	    	 atp_input_get_motors(data->input,motors);
	    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_FRONT_RIGHT,motors[ATP_MOTOR_FRONT_RIGHT].motor_value-2);
	    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_BACK_RIGHT,motors[ATP_MOTOR_BACK_RIGHT].motor_value-2);
	    	 em_io_delay_microseconds(100);
	    	 atp_input_get_ahrs(data->input,&ahrs_data);
	         if(ahrs_data.roll<0.5f)
	        	 work=0;



	 //    }while(work);
}


void balance_rolldown(atp_task_balance *data){

	em_int32 index;
		 atp_motor_data motors[ATP_MOTORS_COUNT]={0,0,0,0};
		 atp_ahrs_data ahrs_data;
		 em_int32 work=1;

	   //  do{
	    	 atp_input_get_motors(data->input,motors);
	    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_FRONT_LEFT,motors[ATP_MOTOR_FRONT_LEFT].motor_value-2);
	    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_BACK_LEFT,motors[ATP_MOTOR_BACK_LEFT].motor_value-2);
	    	 em_io_delay_microseconds(100);
	    	 atp_input_get_ahrs(data->input,&ahrs_data);
	         if(ahrs_data.roll<0.5f)
	        	 work=0;



	//     }while(work);
}



void balance_pitchdown(atp_task_balance *data){

	em_int32 index;
	 atp_motor_data motors[ATP_MOTORS_COUNT]={0,0,0,0};
	 atp_ahrs_data ahrs_data;
	 em_int32 work=1;

    // do{
    	 atp_input_get_motors(data->input,motors);
    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_FRONT_LEFT,motors[ATP_MOTOR_FRONT_LEFT].motor_value-2);
    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_FRONT_RIGHT,motors[ATP_MOTOR_FRONT_RIGHT].motor_value-2);
    	 em_io_delay_microseconds(100);
    	 atp_input_get_ahrs(data->input,&ahrs_data);
         if(ahrs_data.pitch<0.5f)
        	 work=0;



    // }while(work);
}

void balance_pitchup(atp_task_balance *data){

	em_int32 index;
	 atp_motor_data motors[ATP_MOTORS_COUNT]={0,0,0,0};
	 atp_ahrs_data ahrs_data;
	 em_int32 work=1;

    // do{
    	 atp_input_get_motors(data->input,motors);
    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_BACK_LEFT,motors[ATP_MOTOR_BACK_LEFT].motor_value-2);
    	 atp_motor_controller_set_value(data->motor_controller,ATP_MOTOR_BACK_RIGHT,motors[ATP_MOTOR_BACK_RIGHT].motor_value-2);
    	 em_io_delay_microseconds(100);
    	 atp_input_get_ahrs(data->input,&ahrs_data);
         if(ahrs_data.pitch>-0.5f)
        	 work=0;



   //  }while(work);
}
#define BALANCESIZE 5
static em_float32 pitch_values[BALANCESIZE];
static em_float32 roll_values[BALANCESIZE];
static em_int64 time_values[BALANCESIZE];


inline static void recalculate_values(atp_ahrs_data *ahrs_data, em_float32 *times){
	em_int32 index;
	 for(index=1;index<BALANCESIZE;++index){
	        	   pitch_values[index-1]=pitch_values[index];
	        	   roll_values[index-1]=roll_values[index];
	        	   time_values[index-1]=time_values[index];
	           }
	           pitch_values[BALANCESIZE-1]=ahrs_data->pitch;
	           roll_values[BALANCESIZE-1]=ahrs_data->roll;
	           time_values[BALANCESIZE-1]=ahrs_data->time;
	           for(index=0;index<BALANCESIZE;++index){
	           	      times[index]=time_values[index]-(time_values[0]-1);
	           }
}

void balance_system(atp_task_balance *data){


	   em_int32 index;

	    atp_motor_data motors[ATP_MOTORS_COUNT]={0,0,0,0};

	    atp_input_get_motors(data->input,motors);
	   for(index=0;index<ATP_MOTORS_COUNT;++index)
	         if(motors[index].motor_value <=500){
	         	return;
	         }


	    atp_ahrs_data ahrs_data;
	    atp_input_get_ahrs(data->input,&ahrs_data);
	    em_float32 times[BALANCESIZE];
        recalculate_values(&ahrs_data,times);

        em_float32 apitch,bpitch,aroll,broll;
        atleastsquare(BALANCESIZE,pitch_values,times,&apitch,&bpitch,NULL);
        em_float32 y1pitch=apitch*2+bpitch;
        em_float32 y2pitch=apitch*5+bpitch;
        em_double64 tanjantpitch= atan2((y2pitch-y1pitch),(5-2))*180;

        atleastsquare(BALANCESIZE,roll_values,times,&aroll,&broll,NULL);
        em_float32  y1roll=aroll*2+broll;
        em_float32  y2roll=aroll*5+broll;
        em_double64 tanjantroll= atan2((y2roll-y1roll),(5-2))*180;
       printf("pitch %4.2f %4.2f %4.2f roll %4.2f %4.2f %4.2f \n",apitch,bpitch,tanjantpitch,aroll,broll,tanjantroll);



	       if(ahrs_data.pitch>3.0f){
             balance_pitchdown(data);
	       }
	       atp_input_get_ahrs(data->input,&ahrs_data);
	       if(ahrs_data.pitch<-3.0f){
	           balance_pitchup(data);
	       }

	       atp_input_get_ahrs(data->input,&ahrs_data);
	       	       if(ahrs_data.roll>3.0f){
	       	           balance_rolldown(data);
	       	       }
	       	    atp_input_get_ahrs(data->input,&ahrs_data);
	       	    	       	       if(ahrs_data.roll<-3.0f){
	       	    	       	           balance_rollup(data);
	       	    	       	       }






}






void * atp_task_balance_exec(void *parameter){

	 struct atp_task *task=atp_convert(parameter,struct atp_task*);
	   atp_task_balance *data=atp_convert(task->parameter,atp_task_balance*);
       atp_fill_zero(pitch_values,sizeof(em_float32)*BALANCESIZE);
       atp_fill_zero(roll_values,sizeof(em_float32)*BALANCESIZE);
	   while(data->work){
		   data->is_working=1;

		   if(atp_task_share_killall_get(data->share)){//if killall tasks setted than break

			   data->work=0;
			   break;
		   }
		   balance_system(data);
		   em_io_delay_microseconds(1000000/25);//every 100 milisecond sleep
	   }
	   data->is_working=0;
	   task->is_finished=1;

	   return ATP_SUCCESS;

}
void atp_task_balance_free(void *parameter){

	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_balance *data=atp_convert(task->parameter,atp_task_balance*);

	atp_free(data);
	atp_free(task);

}
void atp_task_balance_kill(void *parameter){
	struct atp_task *task=atp_convert(parameter,struct atp_task*);
	atp_task_balance *data=atp_convert(task->parameter,atp_task_balance*);
	data->work=0;
	while(data->is_working)em_io_delay_microseconds(100);//çalışıyorken bekle


}

