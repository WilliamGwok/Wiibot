#include "config_can.h"
#include "config.h"
#include "string.h"
#include "sd_motor.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x1FF_send_buff[4];


/*
CAN2: 四个关节电机 云台Yaw 超级电容
*/
void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case right_wheel_id:
			Wheel_Motor[RIGHT_WHEEL].rx(&Wheel_Motor[RIGHT_WHEEL], rxBuf, 8);
		  break;
		case left_wheel_id:
			Wheel_Motor[LEFT_WHEEL].rx(&Wheel_Motor[LEFT_WHEEL], rxBuf, 8);
		  break;
		case CAN_MOTOR_6012_RIGHT:
			R_Sd.rx(&R_Sd, rxBuf);
		
		  R_Sd.state->offline_cnt = 0;
		  break;
		case CAN_MOTOR_6012_LEFT:
			L_Sd.rx(&L_Sd, rxBuf);
		
		  L_Sd.state->offline_cnt = 0;
		  break;

		default:
			break;
	}
}

