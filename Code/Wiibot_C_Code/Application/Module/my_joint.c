#include "my_joint.h"
#include "config_mec.h"
#include "sd_motor.h"
#include "pid.h"

void My_Joint_Phi4_Get(void);
	
Leg_Target_t L_Leg_Target;

Leg_Measure_t L_Leg_Measure;

Leg_Target_t R_Leg_Target;

Leg_Measure_t R_Leg_Measure;

Joint_t My_Joint[LEG_LIST] = 
{
	[R_LEG] = 
	{
		.target = &R_Leg_Target,
		.measure = &R_Leg_Measure,
	},
	[L_LEG] = 
	{
		.target = &L_Leg_Target,
		.measure = &L_Leg_Measure,
	},
};


void My_Joint_Data_Update(void)
{
	My_Joint_Phi4_Get();
	
	My_Joint_Target_Process();
}

void My_Joint_Sleep(void)
{
	My_Sd_Sleep();
}

void My_Joint_Control(void)
{
	My_Joint_Data_Update();
	
	My_Joint_Sleep();
}

//ch3
void My_Joint_Target_Process(void)
{
	int16_t channel = 0;
	
	if(abs(rc.base_info->ch3) <= 10)
	{
		channel = 0;
	}
	else
	{
		channel = rc.base_info->ch3;
	}
	
	My_Joint[R_LEG].target->joint_rad += (channel / 250.f) * JOINT_RAD_TAR_MAX;
	
	My_Joint[R_LEG].target->joint_rad = constrain(My_Joint[R_LEG].target->joint_rad, 0.f, JOINT_RAD_TAR_MAX);
}

//该计算方式没有通用性
void My_Joint_Phi4_Get(void)
{
	if(R_Sd.rx_info->motor_angle > PI)
	{
		My_Joint[R_LEG].measure->phi4_rad = R_JOINT_ORG_ANGLE + (2 * PI - R_Sd.rx_info->motor_angle);
	}	
	else
	{
		My_Joint[R_LEG].measure->phi4_rad = R_JOINT_ORG_ANGLE - R_Sd.rx_info->motor_angle;
	}
	
	My_Joint[L_LEG].measure->phi4_rad = L_Sd.rx_info->motor_angle - L_JOINT_ORG_ANGLE;
}

pid_info_t R_Leg_Length = 
{
  .kp = 850.f,//750
  .ki = 0.1f,
  .kd = 30000.f,
  .integral_max = 7.f,
  .out_max = 200.f,
};

pid_info_t L_Leg_Length = 
{
  .kp = 850.f,//750
  .ki = 0.1f,
  .kd = 30000.f,
  .integral_max = 7.f,
  .out_max = 200.f,
};

pid_info_t R_Leg_Roll = 
{
  .kp = 500.f,
  .ki = 0.1f,
  .kd = 0.f,
  .integral_max = 0.f,
  .out_max = 210.f,
};

pid_info_t L_Leg_Roll = 
{
  .kp = 500.f,
  .ki = 0.1f,
  .kd = 0.f,//4000
  .integral_max = 0.f,
  .out_max = 210.f,
};

pid_t Leg_Length_Pid[LEG_LIST] = 
{
  [L_LEG] = {
    .info = &L_Leg_Length,
  },
  [R_LEG] = {
    .info = &R_Leg_Length,
  },
};

pid_t Leg_Roll_Pid[LEG_LIST] = 
{
  [L_LEG] = {
    .info = &L_Leg_Roll,
  },
  [R_LEG] = {
    .info = &R_Leg_Roll,
  },
};

void My_Joint_Torque_Cal(void)
{
	
}

