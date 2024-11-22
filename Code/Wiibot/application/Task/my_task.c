#include "my_task.h"
#include "remote.h"
#include "my_robot.h"

void my_control(void const * argument)
{
  /* USER CODE BEGIN my_control */
  /* Infinite loop */
  for(;;)
  {
		My_Wheel_Heartbeat();
		
		My_Sd_HeartBeat();
		
		My_Robot_Control();
		
//		My_Sd_Sleep();
		
		rc_heart_beat(&rc);
		
    osDelay(1);
  }
  /* USER CODE END my_control */
}

