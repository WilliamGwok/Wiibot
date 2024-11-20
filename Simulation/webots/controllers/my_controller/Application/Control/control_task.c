#include "control_task.h"

#define PID_OR_LQR  1  //0 ÎÞ¿ØÖÆÆ÷ 1 lqr 2 pid

void My_Robot_Data_Update(void)
{
  My_Device_Update();

  My_State_Var_Update();
}

void My_Robot_Command_React(void)
{

}

void My_Robot_Work(void)
{
  #if PID_OR_LQR == 1

  My_Model_Output_Cal();

  #elif PID_OR_LQR == 2

  My_Robot_PID_Control();

  #else 

  #endif

  My_Motor_Work(My_Straight_Leg_Model.L_Tw, My_Straight_Leg_Model.R_Tw);
}

void My_Robot_PID_Control(void)
{
  double stand_tor = 0, distance_tor = 0, spin_tor = 0;

  double l_tor = 0, r_tor = 0;

  stand_tor = My_PID_Cal(0, My_Posture.pitch, &Stand_PID);

  l_tor = stand_tor/2;

  r_tor = stand_tor/2;

  My_Straight_Leg_Model.L_Tw = l_tor;

  My_Straight_Leg_Model.R_Tw = r_tor;
}














