#include "my_joint.h"


void My_Joint_Phi4_Get(void);
	
Leg_Target_t L_Leg_Target = 
{
	.joint_rad = 0.1f,
};

Leg_Measure_t L_Leg_Measure;

Leg_Target_t R_Leg_Target = 
{
	.joint_rad = 0.1f,
};

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
float test_len = 0.f;
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
	
//	My_Joint[L_LEG].target->joint_rad = test_len;
//	My_Joint[R_LEG].target->joint_rad = test_len;
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
//pid_info_t R_Leg_Length = 
//{
//  .kp = -100.f,
//  .ki = -0.8f,
//  .kd = -50.f,
//  .integral_max = 10.f,
//  .out_max = 30.f,
//};

//pid_info_t L_Leg_Length = 
//{
//  .kp = 100.f,
//  .ki = 0.8f,
//  .kd = 50.f,
//  .integral_max = 10.f,
//  .out_max = 30.f,
//};

///*腿长控制内环速度环*/
//pid_info_t R_Leg_Speed = 
//{
//  .kp = 3.f,
//  .ki = 0.7f,
//  .kd = 0.f,
//  .integral_max = 5.f,
//  .out_max = 16.f,
//};

//pid_info_t L_Leg_Speed = 
//{
//  .kp = 3.f,
//  .ki = 0.7f,
//  .kd = 0.f,
//  .integral_max = 5.f,
//  .out_max = 16.f,
//};

pid_info_t R_Leg_Length = 
{
  .kp = -100.f,
  .ki = -0.5f,
  .kd = -50.f,
  .integral_max = 10.f,
  .out_max = 30.f,
};

pid_info_t L_Leg_Length = 
{
  .kp = 100.f,
  .ki = 0.5f,
  .kd = 50.f,
  .integral_max = 10.f,
  .out_max = 30.f,
};

/*腿长控制内环速度环*/
pid_info_t R_Leg_Speed = 
{
  .kp = 0.4f,
  .ki = 0.05f,
  .kd = 0.f,
  .integral_max = 5.f,
  .out_max = 6.f,
};

pid_info_t L_Leg_Speed = 
{
  .kp = 0.4f,
  .ki = 0.05f,
  .kd = 0.f,
  .integral_max = 5.f,
  .out_max = 6.f,
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
	Leg_Length_Pid[R_LEG].info->measure = (My_Joint[R_LEG].measure->phi4_rad + My_Joint[L_LEG].measure->phi4_rad) / 2.f;
//	Leg_Length_Pid[R_LEG].info->measure = My_Joint[R_LEG].measure->phi4_rad;
	
	Leg_Length_Pid[R_LEG].info->target = My_Joint[R_LEG].target->joint_rad;
	
	Leg_Speed_Pid[R_LEG].info->measure = My_Joint[R_LEG].measure->speed;
	
	Joint_Double_PID_Cal(&Leg_Length_Pid[R_LEG], &Leg_Speed_Pid[R_LEG]);
	
	Leg_Length_Pid[L_LEG].info->measure = (My_Joint[R_LEG].measure->phi4_rad + My_Joint[L_LEG].measure->phi4_rad) / 2.f;
//	Leg_Length_Pid[L_LEG].info->measure = My_Joint[L_LEG].measure->phi4_rad;
	
	Leg_Length_Pid[L_LEG].info->target = My_Joint[L_LEG].target->joint_rad;
	
	Leg_Speed_Pid[L_LEG].info->measure = My_Joint[L_LEG].measure->speed;
	
	Joint_Double_PID_Cal(&Leg_Length_Pid[L_LEG], &Leg_Speed_Pid[L_LEG]);
	
	
	My_Joint_Roll_Torque_Cal();
}



pid_info_t R_Leg_Roll = 
{
  .kp = 20.f,
  .ki = 0.f,
  .kd = 50.f,
  .integral_max = 0.f,
  .out_max = 6.1f,
};

pid_info_t L_Leg_Roll = 
{
  .kp = -20.f,
  .ki = 0.f,
  .kd = -50.f,
  .integral_max = 0.f,
  .out_max = 6.1f,
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

void My_Joint_Roll_Torque_Cal(void)
{
	Leg_Roll_Pid[L_LEG].info->measure = My_Posture.roll;
	
	Leg_Roll_Pid[L_LEG].info->target = 0.f;
	
	single_pid_ctrl(Leg_Roll_Pid[L_LEG].info);
	
	Leg_Roll_Pid[R_LEG].info->measure = My_Posture.roll;
	
	Leg_Roll_Pid[R_LEG].info->target = 0.f;
	
	single_pid_ctrl(Leg_Roll_Pid[R_LEG].info);
	
		if(rc.info->status == DEV_ONLINE)
	{
		R_Sd.tx_info->torque = Leg_Roll_Pid[R_LEG].info->out + Leg_Speed_Pid[R_LEG].info->out;
	
	  R_Sd.single_set_torque(&R_Sd);
		
		L_Sd.tx_info->torque =  Leg_Roll_Pid[L_LEG].info->out - Leg_Speed_Pid[L_LEG].info->out;
	
	  L_Sd.single_set_torque(&L_Sd);
	}
}


//static void My_Jump_Target_Process(void)
//{
//  static int step = 0;
//  
//  static double org_tar = 0.f;
//  
//  double mea = (My_Joint[R_LEG].measure->phi4_rad + My_Joint[L_LEG].measure->phi4_rad) / 2.f;
//  
//  if(My_Balance.command->chassis->Jump_Flag == true)
//  {
//		if(step == 0)
//    {
//			org_tar = My_Chassis.target->leg_length;
//      My_Chassis.target->leg_length = 0.36f;
//      step++;
//      My_Link_Var[R_Link].link_pid->length_cal->info->kp = 3000;
//      My_Link_Var[L_Link].link_pid->length_cal->info->kp = 3000;
//			My_Link_Var[R_Link].link_pid->length_cal->info->ki = 5;
//			My_Link_Var[L_Link].link_pid->length_cal->info->ki = 5;
//			
//			My_Link_Var[R_Link].link_pid->length_cal->info->kd = 10000;
//			My_Link_Var[L_Link].link_pid->length_cal->info->kd = 10000;
//			
//      My_Link_Var[R_Link].link_pid->length_cal->info->out_max = 3000;
//      My_Link_Var[L_Link].link_pid->length_cal->info->out_max = 3000;
//    }
//    else if(step == 1 && mea > 0.355f)
//    {
//      My_Chassis.target->leg_length = 0.13f;
//      step++;
//      My_Link_Var[R_Link].link_pid->length_cal->info->kp = 3000;
//      My_Link_Var[L_Link].link_pid->length_cal->info->kp = 3000;
//			My_Link_Var[R_Link].link_pid->length_cal->info->ki = 10;
//			My_Link_Var[L_Link].link_pid->length_cal->info->ki = 10;
//      My_Link_Var[R_Link].link_pid->length_cal->info->out_max = 4000;
//      My_Link_Var[L_Link].link_pid->length_cal->info->out_max = 4000;
//    }
//    else if(step == 2 && mea < 0.15f)
//    {
//      My_Chassis.target->leg_length = 0.13f;
//			
//			My_Chassis.target->leg_length = org_tar;
//      step = 0;
//      My_Balance.command->chassis->Jump_Flag = false;
//      My_Link_Var[R_Link].link_pid->length_cal->info->kp = 700;
//      My_Link_Var[L_Link].link_pid->length_cal->info->kp = 700;
//      My_Link_Var[R_Link].link_pid->length_cal->info->out_max = 200;
//      My_Link_Var[L_Link].link_pid->length_cal->info->out_max = 200;
//			My_Link_Var[R_Link].link_pid->length_cal->info->ki = 0.8;
//			My_Link_Var[L_Link].link_pid->length_cal->info->ki = 0.8;
//			My_Link_Var[R_Link].link_pid->length_cal->info->kd = 40000;
//			My_Link_Var[L_Link].link_pid->length_cal->info->kd = 40000;
//    }
//  }
//  
//}

