#include "wheel_motor.h"
#include "motor.h"

motor_t Wheel_Motor[WHEEL_LIST] = 
{
	[RIGHT_WHEEL] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = GM6020,
		.id.rx_id = right_wheel_id,
		
		.init = motor_class_init,
	},
	[LEFT_WHEEL] = 
	{
		.id.drive_type = M_CAN2,
		.id.motor_type = GM6020,
		.id.rx_id = left_wheel_id,
		
		.init = motor_class_init,
	},
};

void My_Wheel_Init(void)
{
	Wheel_Motor[RIGHT_WHEEL].init(&Wheel_Motor[RIGHT_WHEEL]);
	Wheel_Motor[LEFT_WHEEL].init(&Wheel_Motor[LEFT_WHEEL]);
}

/**
  * @brief  ÔÆÌ¨ÐÄÌø°ü
  * @param  None
  * @retval None
  */
void My_Wheel_Heartbeat(void)
{
	Wheel_Motor[RIGHT_WHEEL].heartbeat(&Wheel_Motor[RIGHT_WHEEL]);
	Wheel_Motor[LEFT_WHEEL].heartbeat(&Wheel_Motor[LEFT_WHEEL]);
}

