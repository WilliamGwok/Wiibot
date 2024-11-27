#include "my_task.h"
#include "imu.h"
#include "cmsis_os.h"
#include "drv_haltick.h"
#include "imu_pwm.h"
#include "my_robot.h"
#include "sd_motor.h"
#include "wheel_motor.h"
#include "remote.h"
#include "my_joint.h"

//extern IWDG_HandleTypeDef hiwdg;

uint32_t task_this_h = 0;
uint32_t task_last_h = 0;
uint32_t task_err_h = 0;

void Task_H1(void const * argument)
{
	/*裁判系统数据，软件复位*/
  for(;;)
  {
		task_this_h = micros();

		task_err_h = micros() - task_this_h;
		
    osDelay(1);
  }
}

uint32_t task_this_m = 0;
uint32_t task_last_m = 0;
uint32_t task_err_m = 0;

void Task_H2(void const * argument)
{
	/*失联检测，视觉交互，车间通讯*/
  for(;;)
  {
		
		
		imu_update(&imu);
		
		Imu_PWM_Set(imu.base_info->temp);
		
		
		
		My_Sd_HeartBeat();
		
		My_Wheel_Heartbeat();
		
		rc_heart_beat(&rc);
		
    osDelay(1);
  }
}



void Task_M(void const * argument)
{

	/*车间通讯,车辆控制*/
  for(;;)
  {
		task_this_m = micros();
		
		
		
		My_Robot_Control();
		
		My_Joint_Control();//在6020后发，buglist
		
	
		task_err_m =  micros() - task_this_m ;
		
		//HAL_IWDG_Refresh(&hiwdg);
		
    osDelay(1);
  }

}












