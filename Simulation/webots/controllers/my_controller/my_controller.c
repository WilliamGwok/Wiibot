#include <webots/robot.h>
#include <webots/motor.h>
#include "dev_all.h"
#include "state_var.h"
#include "robot_debug.h"
#include "control_task.h"
/*
 * You may want to add macros here.
 */


int main(int argc, char **argv) {


  wb_robot_init();
  
  My_Device_Init();

  while (wb_robot_step(TIME_STEP) != -1) {
  
  // My_Device_Update();
  
  // My_Motor_Work();
  
  // My_State_Var_Update();
  My_Robot_Data_Update();
  
  My_Robot_Work();
  
  Debug_State_Var();
  Debug_LQR();
  
  // printf("%f\n",My_Posture.pitch);

  };

  wb_robot_cleanup();

  return 0;
}


//速度赋正值，右轮正转，左轮反转
