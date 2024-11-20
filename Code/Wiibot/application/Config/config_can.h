#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"
#include "wheel_motor.h"

#define MULTI_KT_MOTOR_GROUP      0x280

extern int16_t can1_0x1FF_send_buff[4];
extern int16_t can2_0x1FF_send_buff[4];
extern int16_t can1_0x200_send_buff[4];
extern int16_t can2_0x200_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif


