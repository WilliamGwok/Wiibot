#include "state_var.h"

State_Var_t My_State_Var;

/**
  * @brief  状态变量更新
  * @param  None
  * @retval None
  */
void My_State_Var_Update(void)
{
  /*路程与速度*/
  My_State_Var.s = 0.5 * WHEEL_RADIUS * (My_Encoder[R_W_Encoder].rad_now - My_Encoder[L_W_Encoder].rad_now);
  
  My_State_Var.s_now = My_State_Var.s;
  
  My_State_Var.sd1 = (My_State_Var.s_now - My_State_Var.s_last) / (TIME_STEP * 0.001);
  
  My_State_Var.s_last = My_State_Var.s_now;
  
  /*偏航角与偏航角速度*/
  My_State_Var.phi = My_Posture.yaw;
  
  My_State_Var.phid1 = My_Posture.yaw_v;
  
  /*俯仰角与俯仰角速度*/
  My_State_Var.thetab = My_Posture.pitch;
  
  My_State_Var.thetabd1 = My_Posture.pitch_v;
}
















