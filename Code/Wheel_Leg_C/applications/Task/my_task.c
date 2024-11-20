#include "my_task.h"



void my_control(void const * argument)
{
  /* USER CODE BEGIN my_control */
  /* Infinite loop */
  for(;;)
  {
		My_Wheel_Heartbeat();
		
		My_Sd_HeartBeat();
		
		My_Sd_Sleep();
		
    osDelay(1);
  }
  /* USER CODE END my_control */
}

