#include "my_robot.h"
#include "config_mec.h"
#include "lqr.h"
#include "my_state_var.h"

void My_Model_Output_Cal(void);
void My_Wheel_Send_Torque(float r_tor, float l_tor);

Straight_Leg_Model_t My_Straight_Leg_Model;

void My_Robot_Update(void)
{
	My_Posture_Update();
		
	My_State_Var_Update();
}

void My_Robot_Output_Cal(void)
{
	My_Model_Output_Cal();
}

void My_Robot_Control(void)
{
	My_Robot_Update();
	
	My_Robot_Output_Cal();
	
	My_Wheel_Send_Torque(My_Straight_Leg_Model.R_Tw, My_Straight_Leg_Model.L_Tw);
}









/*........................................LQR控制器部分begin........................................*/

float My_Yaw_Zero_Point_Process(float err);

void My_Model_Output_Cal(void)
{
  K_Matrix_t *K = &My_K_Matrix;

	/* 左驱动轮扭矩输出计算 */
	My_Straight_Leg_Model.l_s_part = K->wheell_K[0] * constrain(My_State_Var.s, -0.25f, 0.25f) + K->wheell_K[1] * constrain(My_State_Var.sd1, -0.7f, 0.7f);
	
	My_Straight_Leg_Model.l_theta_part = K->wheell_K[2] * constrain(My_State_Var.thetab, -0.5f, 0.5f) + K->wheell_K[3] * My_State_Var.thetabd1;
	
	My_Straight_Leg_Model.l_phi_part = -(K->wheell_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi) + K->wheell_K[5] * My_State_Var.phid1);

  My_Straight_Leg_Model.L_Tw = -( My_Straight_Leg_Model.l_s_part + My_Straight_Leg_Model.l_theta_part + My_Straight_Leg_Model.l_phi_part );
	
	/* 右驱动轮扭矩输出计算 */
	My_Straight_Leg_Model.r_s_part = K->wheelr_K[0] * constrain(My_State_Var.s, -0.25f, 0.25f) + K->wheelr_K[1] * constrain(My_State_Var.sd1, -0.7f, 0.7f);
	
	My_Straight_Leg_Model.r_theta_part = K->wheelr_K[2] * constrain(My_State_Var.thetab, -0.5f, 0.5f) + K->wheelr_K[3] * My_State_Var.thetabd1;
	
	My_Straight_Leg_Model.r_phi_part = -(K->wheelr_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi) + K->wheelr_K[5] * My_State_Var.phid1);

  My_Straight_Leg_Model.R_Tw = -( My_Straight_Leg_Model.r_s_part + My_Straight_Leg_Model.r_theta_part + My_Straight_Leg_Model.r_phi_part );
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
//	can1_0x200_send_buff[0] = Motor_RM6020_Torque_to_Raw_Current(r_tor);
//	
//	can1_0x200_send_buff[1] = Motor_RM6020_Torque_to_Raw_Current(l_tor);
//	
//	CAN1_Send_With_int16_to_uint8(0x1FE, can1_0x200_send_buff);
	can1_0x200_send_buff[0] = Motor_RM6020_Torque_to_Raw_Voltage(r_tor) * -1;
	
	testr = can1_0x200_send_buff[0];
	
	can1_0x200_send_buff[1] = Motor_RM6020_Torque_to_Raw_Voltage(l_tor);
	
	test1 = can1_0x200_send_buff[1];
	
	if(switchc == 1)
	{
		CAN1_Send_With_int16_to_uint8(0x1FF, can1_0x200_send_buff);
	}
	else
	{
		can1_0x200_send_buff[0] = 0;
		
		can1_0x200_send_buff[1] = 0;
		CAN1_Send_With_int16_to_uint8(0x1FF, can1_0x200_send_buff);
	}
}
















