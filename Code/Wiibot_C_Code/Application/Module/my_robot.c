#include "my_robot.h"
#include "config_mec.h"
#include "lqr.h"
#include "my_state_var.h"
#include "bmi.h"
#include "remote.h"
#include "my_joint.h"
#include "vision_protocol.h"

void My_Model_Output_Cal(void);
void My_Wheel_Send_Torque(float r_tor, float l_tor);
void My_Vision_Data_Send(void);

Straight_Leg_Model_t My_Straight_Leg_Model;

Robot_Remote_t My_Robot_Remote;

Robot_Target_t My_Robot_Target;

Robot_t My_Robot = 
{
	.imu_adapt_cnt = 0,
	
	.imu_adapt_flag = false,
		
	.remote = &My_Robot_Remote,
	
	.target = &My_Robot_Target,
};

void My_Robot_Update(void)
{
	My_Robot_Imu_Kp_Adapt();
	
	My_Posture_Update();
		
	My_State_Var_Update();
	
	My_Robot_RC_Value_Filt();
	
	My_Vision_Mode_Detect();
	
	My_Robot_Distance_Target_Process();
	
	My_Robot_Spin_Target_Process();
}
//float AAA = 0.f;
void My_Robot_Output_Cal(void)
{
//	K_Matrix_Fitting_Update(&My_K_Matrix, (1.5708f - (My_Joint[L_LEG].measure->phi4_rad + A_INITIAL_ANGLE)),\
//	                        (1.5708f - (My_Joint[R_LEG].measure->phi4_rad + A_INITIAL_ANGLE)));
	
//	AAA = 1.5708f - (My_Joint[R_LEG].measure->phi4_rad + A_INITIAL_ANGLE);
	
	My_Model_Output_Cal();
}

void My_Robot_Control(void)
{
	My_Robot_Update();
	
	My_Robot_Output_Cal();
	
	My_Vision_Data_Send();
	
	My_Wheel_Send_Torque(My_Straight_Leg_Model.R_Tw, My_Straight_Leg_Model.L_Tw);
}



void My_Robot_Imu_Kp_Adapt(void)
{
	if(My_Robot.imu_adapt_flag == false)
	{
		My_Robot.imu_adapt_cnt++;
		
		bmi_Kp = 10.f;
		
		if(My_Robot.imu_adapt_cnt >= 1000)
		{
			My_Robot.imu_adapt_flag = true;
		}
	}
	else
	{
		bmi_Kp = 0.1f;
	}
}


void My_Robot_RC_Value_Filt(void)
{
	if(abs(rc.base_info->ch0) <= STICK_DRIFT_THRESHOLD)
	{
		My_Robot.remote->ch0 = 0;
	}
	else
	{
		My_Robot.remote->ch0 = rc.base_info->ch0;
	}
	
	if(abs(rc.base_info->ch1) <= STICK_DRIFT_THRESHOLD)
	{
		My_Robot.remote->ch1 = 0;
	}
	else
	{
		My_Robot.remote->ch1 = rc.base_info->ch1;
	}
	
	if(abs(rc.base_info->ch2) <= STICK_DRIFT_THRESHOLD)
	{
		My_Robot.remote->ch2 = 0;
	}
	else
	{
		My_Robot.remote->ch2 = rc.base_info->ch2;
	}
	
	if(abs(rc.base_info->ch3) <= STICK_DRIFT_THRESHOLD)
	{
		My_Robot.remote->ch3 = 0;
	}
	else
	{
		My_Robot.remote->ch3 = rc.base_info->ch3;
	}
}
float test_dis_add = 0.f;

//ch1 10~500 0.25m/s
void My_Robot_Distance_Target_Process(void)
{
	if(My_Robot.vision_flag != true)
	{
		My_Robot.target->velocity = ((float)My_Robot.remote->ch1 / MAX_CHANNEL) * MAX_VELOCITY;
	}
	else
	{
		if(Vision_Rx_Info.mode == 0)
		{
			My_Robot.target->velocity = 0;
		}
		else if(Vision_Rx_Info.mode == 1)
		{
			My_Robot.target->velocity = 0.2f;
		}
		else if(Vision_Rx_Info.mode == 2)
		{
			My_Robot.target->velocity = -0.2f;
		}
	}
	
	My_Robot.target->distance += My_Robot.target->velocity * TIME_BASE;
}

/**
  * @brief  yaw轴目标值处理
  * @param  double target
  * @retval 处理后的目标值
  */
float My_Robot_Yaw_Target_Process(float target)
{
  if(abs(target) > PI)
  {
    target -= one(target)*2*PI;
  }
  
  return target;
}

//ch0
void My_Robot_Spin_Target_Process(void)
{
	if(My_Robot.vision_flag != true)
	{
		My_Robot.target->spin_velocity = -((float)My_Robot.remote->ch0 / MAX_CHANNEL) * MAX_SPIN_VELOCITY;
	
	  My_Robot.target->spin_rad += My_Robot.target->spin_velocity * TIME_BASE;
	}
	else
	{
		if(Vision_Rx_Info.mode == 3)
		{
			
			
			My_Robot.target->spin_rad = Vision_Rx_Info.yaw_angle;
			
			if((My_Robot.target->spin_rad - My_State_Var.phi) > 0)
			{
				My_Robot.target->spin_velocity = 1.5f;
			}
			else if((My_Robot.target->spin_rad - My_State_Var.phi) < 0)
			{
				My_Robot.target->spin_velocity = -1.5f;
			}
			else
			{
				My_Robot.target->spin_velocity = 0.f;
			}
		}
	}

	My_Robot.target->spin_rad = My_Robot_Yaw_Target_Process(My_Robot.target->spin_rad);
	
}

void My_Vision_Data_Send(void)
{
	Vision_Tx_Info.yaw_angle = My_Posture.yaw;
	
	
  Vision_Send_Data();
}

void My_Vision_Mode_Detect(void)
{
	static int16_t s2_last = 0;
	
	if(s2_last != 0 && s2_last != rc.base_info->s2.value && rc.base_info->s2.value == 1)
	{
		My_Robot.vision_flag = true;
	}
	
	if(rc.base_info->s2.value != 1)
	{
		My_Robot.vision_flag = false;
	}
	
	s2_last = rc.base_info->s2.value;
}
/*........................................LQR控制器部分begin........................................*/

float My_Yaw_Zero_Point_Process(float err);


void My_Model_Output_Cal(void)
{
  K_Matrix_t *K = &My_K_Matrix;

	/* 左驱动轮扭矩输出计算 */
	My_Straight_Leg_Model.l_s_part = \
	  K->wheell_K[0] * constrain((My_State_Var.s - My_Robot.target->distance), -0.4f, 0.4f) \
	  + K->wheell_K[1] * constrain(My_State_Var.sd1 - 1.f * My_Robot.target->velocity, -2.f, 2.f);
	
	My_Straight_Leg_Model.l_theta_part = \
	  K->wheell_K[2] * My_State_Var.thetab \
	  + K->wheell_K[3] * My_State_Var.thetabd1;
	
	My_Straight_Leg_Model.l_phi_part = \
	  -(K->wheell_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi - My_Robot.target->spin_rad) \
	  + K->wheell_K[5] * (My_State_Var.phid1 - My_Robot.target->spin_velocity));

  My_Straight_Leg_Model.L_Tw = \
	  -( My_Straight_Leg_Model.l_s_part \
	  + My_Straight_Leg_Model.l_theta_part + My_Straight_Leg_Model.l_phi_part );
	
	/* 右驱动轮扭矩输出计算 */
	My_Straight_Leg_Model.r_s_part = \
	  K->wheelr_K[0] * constrain((My_State_Var.s - My_Robot.target->distance), -0.4f, 0.4f) \
		+ K->wheelr_K[1] * constrain(My_State_Var.sd1 - 1.f * My_Robot.target->velocity, -2.f, 2.f);
	
	My_Straight_Leg_Model.r_theta_part = \
	  K->wheelr_K[2] * My_State_Var.thetab \
		+ K->wheelr_K[3] * My_State_Var.thetabd1;
	
	My_Straight_Leg_Model.r_phi_part = \
	  -(K->wheelr_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi - My_Robot.target->spin_rad) \
		+ K->wheelr_K[5] * (My_State_Var.phid1 - My_Robot.target->spin_velocity));

  My_Straight_Leg_Model.R_Tw = \
	  -( My_Straight_Leg_Model.r_s_part \
		+ My_Straight_Leg_Model.r_theta_part + My_Straight_Leg_Model.r_phi_part );
}

/**
  * @brief  yaw轴误差过类过零点处理
  * @param  double err
  * @retval 处理后的误差值
  */
float My_Yaw_Zero_Point_Process(float err)
{
  if(abs(err) > PI)
  {
    err += -(one(err) * 2*PI);
  }
  
  return err;
}

/*........................................LQR控制器部分end........................................*/


/*........................................驱动轮扭矩转电流输出部分begin........................................*/

/* 查阅RM6020电机手册，知电流给定值范围：-16384~0~16384, 对应最大转矩电流范围 -3A~0~3A。转矩常数 0.741N·m/A 
   电压给定值范围：-25000~0~25000, 对应最大转矩电流范围 -24V~0~24V
*/

#define RM6020_TORQUE_CONSTANT           0.741f

#define RM6020_TORQUE_TO_RPM_CONSTANT    156.f
#define RM6020_RPM_TO_VOLTAGE            13.33f
#define RM6020_MAX_VOLTAGE               24.f

int16_t Motor_RM6020_Torque_to_Raw_Current(float tor)
{
	float current = 0.f;
	
	int16_t raw_current = 0;
	
	//转矩转电流
	current = constrain((tor / RM6020_TORQUE_CONSTANT), -3.f, 3.f);
	
	//电流转电流协议值
	raw_current = (int16_t)(current / 3.f) * 16384;
	
	return raw_current;
}

int16_t Motor_RM6020_Torque_to_Raw_Voltage(float tor)
{
	float rpm = 0.f, voltage = 0.f;
	
	int16_t raw_voltage = 0;
	
	rpm = tor * RM6020_TORQUE_TO_RPM_CONSTANT;
	
	voltage = rpm / RM6020_RPM_TO_VOLTAGE;
	
	raw_voltage = (int16_t)(constrain(voltage, -RM6020_MAX_VOLTAGE, RM6020_MAX_VOLTAGE) / RM6020_MAX_VOLTAGE * 25000);
	
	return raw_voltage;
}

int16_t test1 = 0, testr = 0, switchc = 0;;

void My_Wheel_Send_Torque(float r_tor, float l_tor)
{
	can1_0x200_send_buff[0] = Motor_RM6020_Torque_to_Raw_Voltage(r_tor) * -1;
	
	testr = can1_0x200_send_buff[0];
	
	can1_0x200_send_buff[1] = Motor_RM6020_Torque_to_Raw_Voltage(l_tor);
	
	test1 = can1_0x200_send_buff[1];
	
	if(rc.info->status == DEV_ONLINE && My_Robot.imu_adapt_flag == true)
	{
		if(R_Sd.rx_info->torque >= -0.5f || L_Sd.rx_info->torque >= -0.5f)
		{
			My_Robot.target->velocity = 0.f;
		
		  My_Robot.target->distance = My_State_Var.s;
		
		  My_Robot.target->spin_rad = My_State_Var.phi;
		
		  My_Robot.target->spin_velocity = 0.f;
			
			can1_0x200_send_buff[0] = 0;
		
		  can1_0x200_send_buff[1] = 0;
		}
		
		//		can1_0x200_send_buff[0] = 0;
    //		
    //		can1_0x200_send_buff[1] = 0;
		
		CAN1_Send_With_int16_to_uint8(0x1FF, can1_0x200_send_buff);
	}
	else
	{
			can1_0x200_send_buff[0] = 0;
		
		  can1_0x200_send_buff[1] = 0;
		
		CAN1_Send_With_int16_to_uint8(0x1FF, can1_0x200_send_buff);
		
		My_Robot.target->velocity = 0.f;
		
		My_Robot.target->distance = My_State_Var.s;
		
		My_Robot.target->spin_rad = My_State_Var.phi;
		
		My_Robot.target->spin_velocity = 0.f;
		
		My_Robot.vision_flag = false;
	}
}
















