#ifndef __SD_MOTOR_H
#define __SD_MOTOR_H

#include "stm32f4xx_hal.h"
#include "ktech_motor.h"


extern CAN_HandleTypeDef hcan1;

#define CAN_MOTOR_6012_RIGHT    			0x142
#define CAN_MOTOR_6012_LEFT     			0x141

#define L_SD_ORDER_CORRECT            1.f
#define R_SD_ORDER_CORRECT            -1.f

extern Motor_Ktech_t R_Sd;
extern Motor_Ktech_t L_Sd;

void My_Sd_Init(void);
void My_Sd_HeartBeat(void);
void My_Sd_Sleep(void);

#endif


