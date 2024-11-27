#include "my_joint.h"


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
	
	My_Joint_Torque_Cal();
	
	
	if(rc.info->status == DEV_OFFLINE)
	{
		My_Joint_Sleep();
	}
	
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
	
	My_Joint[R_LEG].target->speed = (channel / 250.f) * JOINT_RAD_SPEED_MAX;
	
	My_Joint[R_LEG].target->joint_rad += (My_Joint[R_LEG].target->speed * TIME_BASE);
	
	My_Joint[R_LEG].target->joint_rad = constrain(My_Joint[R_LEG].target->joint_rad, 0.f, JOINT_RAD_TAR_MAX);
	
	My_Joint[L_LEG].target->speed = (channel / 250.f) * JOINT_RAD_SPEED_MAX;
	
	My_Joint[L_LEG].target->joint_rad += (My_Joint[L_LEG].target->speed * TIME_BASE);
	
	My_Joint[L_LEG].target->joint_rad = constrain(My_Joint[L_LEG].target->joint_rad, 0.f, JOINT_RAD_TAR_MAX);
}

//该计算方式没有通用性
void My_Joint_Phi4_Get(void)
{
	/*更新关节角度*/
	if(R_Sd.rx_info->motor_angle > PI)
	{
		My_Joint[R_LEG].measure->phi4_rad = R_JOINT_ORG_ANGLE + (2 * PI - R_Sd.rx_info->motor_angle);
	}	
	else
	{
		My_Joint[R_LEG].measure->phi4_rad = R_JOINT_ORG_ANGLE - R_Sd.rx_info->motor_angle;
	}
	
	My_Joint[L_LEG].measure->phi4_rad = L_Sd.rx_info->motor_angle - L_JOINT_ORG_ANGLE;
	
	/*更新关节角速度*/
	My_Joint[R_LEG].measure->speed = R_Sd.rx_info->speed / 8.f * PI / 30.f;
	
	My_Joint[L_LEG].measure->speed = L_Sd.rx_info->speed / 8.f * PI / 30.f;
	
}

//pid_info_t R_Leg_Length = 
//{
//  .kp = 200.f,
//  .ki = 0.3f,
//  .kd = 0.f,
//  .integral_max = 50.f,
//  .out_max = 150.f,
//};

/*腿长控制外环*/
pid_info_t R_Leg_Length = 
{
  .kp = -100.f,
  .ki = 0.f,
  .kd = -50.f,
  .integral_max = 0.f,
  .out_max = 30.f,
};

pid_info_t L_Leg_Length = 
{
  .kp = 100.f,
  .ki = 0.f,
  .kd = 50.f,
  .integral_max = 0.f,
  .out_max = 30.f,
};

/*腿长控制内环速度环*/
pid_info_t R_Leg_Speed = 
{
  .kp = 3.f,
  .ki = 0.7f,
  .kd = 0.f,
  .integral_max = 5.f,
  .out_max = 50.f,
};

pid_info_t L_Leg_Speed = 
{
  .kp = 3.f,
  .ki = 0.7f,
  .kd = 0.f,
  .integral_max = 5.f,
  .out_max = 50.f,
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

pid_t Leg_Speed_Pid[LEG_LIST] = 
{
  [L_LEG] = {
    .info = &L_Leg_Speed,
  },
  [R_LEG] = {
    .info = &R_Leg_Speed,
  },
};

float Joint_Double_PID_Cal(pid_t* pid_out, pid_t* pid_in)
{
	single_pid_ctrl(pid_out->info);
	
	pid_in->info->target = pid_out->info->out;
	
	single_pid_ctrl(pid_in->info);
	
	return pid_in->info->out;
}

void My_Joint_Torque_Cal(void)
{
	Leg_Length_Pid[R_LEG].info->measure = My_Joint[R_LEG].measure->phi4_rad;
	
	Leg_Length_Pid[R_LEG].info->target = My_Joint[R_LEG].target->joint_rad;
	
	Leg_Speed_Pid[R_LEG].info->measure = My_Joint[R_LEG].measure->speed;
	
	Joint_Double_PID_Cal(&Leg_Length_Pid[R_LEG], &Leg_Speed_Pid[R_LEG]);
	
	Leg_Length_Pid[L_LEG].info->measure = My_Joint[L_LEG].measure->phi4_rad;
	
	Leg_Length_Pid[L_LEG].info->target = My_Joint[L_LEG].target->joint_rad;
	
	Leg_Speed_Pid[L_LEG].info->measure = My_Joint[L_LEG].measure->speed;
	
	Joint_Double_PID_Cal(&Leg_Length_Pid[L_LEG], &Leg_Speed_Pid[L_LEG]);
	
	
	My_Joint_Roll_Torque_Cal();
	
	
	if(rc.info->status == DEV_ONLINE)
	{
		L_Sd.tx_info->torque = Leg_Speed_Pid[L_LEG].info->out;
	
	  L_Sd.single_set_torque(&L_Sd);
		
		R_Sd.tx_info->torque = Leg_Speed_Pid[R_LEG].info->out;
	
	  R_Sd.single_set_torque(&R_Sd);
	}
}


pid_info_t R_Leg_Roll = 
{
  .kp = 10.f,
  .ki = 0.1f,
  .kd = 0.f,
  .integral_max = 0.f,
  .out_max = 50.f,
};

pid_info_t L_Leg_Roll = 
{
  .kp = 10.f,
  .ki = 0.1f,
  .kd = 0.f,//4000
  .integral_max = 0.f,
  .out_max = 50.f,
};

pid_info_t R_Leg_Roll_Speed = 
{
  .kp = 10.f,
  .ki = 0.1f,
  .kd = 0.f,
  .integral_max = 0.f,
  .out_max = 50.f,
};

pid_info_t L_Leg_Roll_Speed = 
{
  .kp = 10.f,
  .ki = 0.1f,
  .kd = 0.f,//4000
  .integral_max = 0.f,
  .out_max = 50.f,
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

pid_t Leg_Roll_Speed_Pid[LEG_LIST] = 
{
  [L_LEG] = {
    .info = &L_Leg_Roll_Speed,
  },
  [R_LEG] = {
    .info = &R_Leg_Roll_Speed,
  },
};

void My_Joint_Roll_Torque_Cal(void)
{
	Leg_Roll_Pid[L_LEG].info->measure = My_Posture.roll;
	
	Leg_Roll_Pid[L_LEG].info->target = 0.f;
	
	Leg_Roll_Speed_Pid[L_LEG].info->measure = My_Posture.roll_v;
	
	Joint_Double_PID_Cal(&Leg_Roll_Pid[L_LEG], &Leg_Roll_Speed_Pid[L_LEG]);
	
	Leg_Roll_Pid[R_LEG].info->measure = My_Posture.roll;
	
	Leg_Roll_Pid[R_LEG].info->target = 0.f;
	
	Leg_Roll_Speed_Pid[R_LEG].info->measure = My_Posture.roll_v;
	
	Joint_Double_PID_Cal(&Leg_Roll_Pid[R_LEG], &Leg_Roll_Speed_Pid[R_LEG]);
}

