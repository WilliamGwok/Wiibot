#ifndef __WHEEL_MOTOR_H
#define __WHEEL_MOTOR_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "stdbool.h"


/*CAN1*/
#define right_wheel_id  		0x205
#define left_wheel_id  	    0x206
/*CAN2*/



typedef enum
{
	RIGHT_WHEEL = 0,
	LEFT_WHEEL ,
	
	WHEEL_LIST
}wheel_motor_list_e;

extern motor_t Wheel_Motor[WHEEL_LIST];

void My_Wheel_Init(void);
void My_Wheel_Heartbeat(void);

#endif


