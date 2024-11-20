#include "sd_motor.h"

/*×óÇý¶¯ÂÖ*/

Motor_Ktech_Born_Info_t L_Sd_Born_Info = 
{
	.ktech_type = motor_6012,
	
	.stdId = CAN_MOTOR_6012_LEFT,
	
	.hcan = &hcan1,
	
	.order_correction = L_SD_ORDER_CORRECT,
};

Motor_Ktech_Rx_Info_t L_Sd_Rx_Info;

Motor_Ktech_Tx_Info_t L_Sd_Tx_Info;

Motor_Ktech_State_t L_Sd_State;

Motor_Ktech_t L_Sd = 
{
	.born_info = &L_Sd_Born_Info,
	
	.rx_info = &L_Sd_Rx_Info,
	
	.tx_info = &L_Sd_Tx_Info,
	
	.state = &L_Sd_State,
	
	.single_init = &Single_Motor_Init,
};


/*ÓÒÇý¶¯ÂÖ*/

Motor_Ktech_Born_Info_t R_Sd_Born_Info = 
{
	.ktech_type = motor_6012,
	
	.stdId = CAN_MOTOR_6012_RIGHT,
	
	.hcan = &hcan1,
	
	.order_correction = R_SD_ORDER_CORRECT,
};

Motor_Ktech_Rx_Info_t R_Sd_Rx_Info;

Motor_Ktech_Tx_Info_t R_Sd_Tx_Info;

Motor_Ktech_State_t R_Sd_State;

Motor_Ktech_t R_Sd = 
{
	.born_info = &R_Sd_Born_Info,
	
	.rx_info = &R_Sd_Rx_Info,
	
	.tx_info = &R_Sd_Tx_Info,
	
	.state = &R_Sd_State,
	
	.single_init = &Single_Motor_Init,
};


void My_Sd_Init(void)
{
	L_Sd.single_init(&L_Sd);
	
	R_Sd.single_init(&R_Sd);
}

/**
  * @brief  µ×ÅÌÐÄÌø°ü
  * @param  None
  * @retval None
  */
void My_Sd_HeartBeat(void)
{
	L_Sd.single_heart_beat(&L_Sd);
	
	R_Sd.single_heart_beat(&R_Sd);
}

void My_Sd_Sleep(void)
{
	L_Sd.single_sleep(&L_Sd);
	
	R_Sd.single_sleep(&R_Sd);
}
