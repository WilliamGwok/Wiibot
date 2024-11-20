#include "straight_leg_model.h"

Straight_Leg_Model_t My_Straight_Leg_Model;



/**
  * @brief  状态变量更新
  * @param  None
  * @retval None
  */
void My_Model_Output_Cal(void)
{
  K_Matrix_t *K = &My_K_Matrix;

  My_Straight_Leg_Model.L_Tw = K->wheell_K[0] * My_State_Var.s + K->wheell_K[1] * My_State_Var.sd1 + \
                               K->wheell_K[2] * My_State_Var.thetab + K->wheell_K[3] * My_State_Var.thetabd1 + \
                               K->wheell_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi) + K->wheell_K[5] * My_State_Var.phid1;

  My_Straight_Leg_Model.L_Tw *= -1; 

  My_Straight_Leg_Model.R_Tw = K->wheelr_K[0] * My_State_Var.s + K->wheelr_K[1] * My_State_Var.sd1 + \
                               K->wheelr_K[2] * My_State_Var.thetab + K->wheelr_K[3] * My_State_Var.thetabd1 + \
                               K->wheelr_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi) + K->wheelr_K[5] * My_State_Var.phid1;
                               
  My_Straight_Leg_Model.R_Tw *= -1; 
}

/**
  * @brief  yaw轴误差过类过零点处理
  * @param  double err
  * @retval 处理后的误差值
  */
double My_Yaw_Zero_Point_Process(double err)
{
  if(abs(err) > PI)
  {
    err += -(one(err) * 2*PI);
  }
  
  return err;
}
pid_info_t Stand_PID_Info = 
{
  .kp = -1.f,//750
  .ki = -0.2f,
  .kd = 0.f,
  .integral_max = 10.f,
  .out_max = 100.f,
};

pid_info_t Distance_PID_Info = 
{
  .kp = 0.f,//750
  .ki = 0.f,
  .kd = 0.f,
  .integral_max = 0.f,
  .out_max = 0.f,
};

pid_info_t Spin_PID_Info = 
{
  .kp = 0.f,//750
  .ki = 0.f,
  .kd = 0.f,
  .integral_max = 0.f,
  .out_max = 0.f,
};

pid_t Stand_PID = 
{
  .info = &Stand_PID_Info,
};

pid_t Distance_PID = 
{
  .info = &Distance_PID_Info,
};

pid_t Spin_PID = 
{
  .info = &Spin_PID_Info,
};

/**
  * @brief  直立环，检测pitch轴误差，左右两轮输出极性相同
  * @param  double err
  * @retval 处理后的误差值
  */
double My_PID_Cal(double tar, double mea, pid_t * pid)
{
  pid->info->target = tar;

  pid->info->measure = mea;

  single_pid_ctrl(pid->info);

  return (pid->info->out);
}














