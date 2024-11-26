#include "my_state_var.h"
#include "config_mec.h"
#include "math_support.h"

void My_Wheel_Angle_Sum_to_Rad(void);

State_Var_t My_State_Var;

/**
  * @brief  状态变量更新
  * @param  None
  * @retval None
  */
void My_State_Var_Update(void)
{
  /*路程与速度*/   
	My_Wheel_Angle_Sum_to_Rad();
	
//  My_State_Var.s = lowpass(My_State_Var.s, (0.5f * WHEEL_RADIUS * (My_State_Var.lw_rad - My_State_Var.rw_rad)), 0.1f);
	
	My_State_Var.s = 0.5f * WHEEL_RADIUS * (My_State_Var.lw_rad - My_State_Var.rw_rad);
	
  My_State_Var.s_now = My_State_Var.s;
  
  My_State_Var.sd1 = lowpass(My_State_Var.sd1, ((My_State_Var.s_now - My_State_Var.s_last) / TIME_BASE), 0.01f);
  
  My_State_Var.s_last = My_State_Var.s_now;
  
  /*偏航角与偏航角速度*/
  My_State_Var.phi = My_Posture.yaw;
  
  My_State_Var.phid1 = My_Posture.yaw_v;
  
  /*俯仰角与俯仰角速度*/
  My_State_Var.thetab = My_Posture.pitch;
  
  My_State_Var.thetabd1 = My_Posture.pitch_v;
}

void My_Wheel_Angle_Sum_to_Rad(void)
{
	My_State_Var.lw_rad = (float)Wheel_Motor[LEFT_WHEEL].rx_info.angle_sum / 8192.f * 2.f * PI;
	
	My_State_Var.rw_rad = (float)Wheel_Motor[RIGHT_WHEEL].rx_info.angle_sum / 8192.f * 2.f * PI;
}


