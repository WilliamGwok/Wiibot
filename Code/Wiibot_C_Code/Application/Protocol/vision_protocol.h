#ifndef __VISION_PROTOCOL_H
#define __VISION_PROTOCOL_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

typedef __packed struct
{
	uint8_t  			SOF;
	uint8_t       mode;
	uint8_t  			CRC8;
	float    			yaw_angle;	 			// dataf_1
	uint16_t 			CRC16;
}Vision_Tx_Info_t;

typedef __packed struct
{
	uint8_t			  SOF;
	uint8_t       mode;           // datau8_1
	uint8_t 			CRC8;
	float   			yaw_angle;    // dataf_1
	uint16_t 			CRC16;
}Vision_Rx_Info_t;

extern Vision_Tx_Info_t Vision_Tx_Info;
extern Vision_Rx_Info_t Vision_Rx_Info;

bool Vision_Send_Data(void);
bool Vision_Recieve_Data(uint8_t *rxBuf);

#endif

