#include "my_task.h"
#include "imu.h"
#include "cmsis_os.h"
#include "drv_haltick.h"
#include "imu_pwm.h"
#include "my_robot.h"

//extern IWDG_HandleTypeDef hiwdg;

uint32_t task_this_h = 0;
uint32_t task_last_h = 0;
uint32_t task_err_h = 0;

void Task_H1(void const * argument)
{
	/*����ϵͳ���ݣ������λ*/
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
	/*ʧ����⣬�Ӿ�����������ͨѶ*/
  for(;;)
  {
		task_this_m = micros();
		
		imu_update(&imu);
		
		Imu_PWM_Set(imu.base_info->temp);
		
		task_err_m =  micros() - task_this_m ;
		
    osDelay(1);
  }
}



void Task_M(void const * argument)
{

	/*����ͨѶ,��������*/
  for(;;)
  {
		My_Robot_Control();
		
	
		
		
		//HAL_IWDG_Refresh(&hiwdg);
		
    osDelay(1);
  }

}












