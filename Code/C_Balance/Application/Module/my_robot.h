#ifndef __MY_ROBOT_H
#define __MY_ROBOT_H

#include "stm32f4xx_hal.h"
#include "my_posture.h"
#include "math_support.h"
#include "drv_can.h"
#include "config_can.h"

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
	
	
}Robot_Remote_t;

typedef struct Robot_struct_t
{
	bool imu_adapt_flag;
	
	uint16_t imu_adapt_cnt;
}Robot_t;

void My_Robot_Control(void);
void My_Robot_Imu_Kp_Adapt(void);

#endif
