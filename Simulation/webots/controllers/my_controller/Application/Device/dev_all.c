#include "dev_all.h"



void My_Device_Init(void)
{
  My_Imu_Init();

  My_Motor_Init();

  My_Encoder_Init();
}

/**
  * @brief  ���������ݸ��£�����Imu,Gyro�����������
  * @param  None
  * @retval None
  */
void My_Device_Update(void)
{
  My_Imu_Data_Update();

  My_Encoder_Data_Update();

  My_Motor_Data_Update();
}







