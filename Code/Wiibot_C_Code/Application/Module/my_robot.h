#ifndef __MY_ROBOT_H
#define __MY_ROBOT_H

#include "stm32f4xx_hal.h"
#include "my_posture.h"
#include "math_support.h"
#include "drv_can.h"
#include "config_can.h"
#include "config_mec.h"

#define STICK_DRIFT_THRESHOLD   10
#define MAX_CHANNEL             500

#define MAX_VELOCITY            0.55f
#define MAX_SPIN_VELOCITY       3.f

typedef struct Straight_Leg_Model_struct_t
{
	float l_s_part;
	
	float l_theta_part;
	
	float l_phi_part;
	
	float r_s_part;
	
	float r_theta_part;
	
	float r_phi_part;
	
  float L_Tw;
  
  float R_Tw;

}Straight_Leg_Model_t;

typedef struct Robot_Remote_struct_t
{
	int16_t ch0;
	
	int16_t ch1;
	
	int16_t ch2;
	
	int16_t ch3;
	
}Robot_Remote_t;

typedef struct Robot_Target_struct_t
{
	float velocity;
	
	float distance;
	
	float spin_velocity;
	
	float spin_rad;
}Robot_Target_t;

typedef struct Robot_struct_t
{
	bool imu_adapt_flag;
	
	uint16_t imu_adapt_cnt;
	
	Robot_Remote_t *remote;
	
	Robot_Target_t *target;
}Robot_t;

void My_Robot_Control(void);
void My_Robot_Imu_Kp_Adapt(void);
void My_Robot_RC_Value_Filt(void);
void My_Robot_Distance_Target_Process(void);
void My_Robot_Spin_Target_Process(void);

#endif
