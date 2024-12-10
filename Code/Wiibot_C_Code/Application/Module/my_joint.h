#ifndef __MY_JOINT_H
#define __MY_JOINT_H

#include "stm32f4xx_hal.h"
#include "my_posture.h"
#include "math_support.h"
#include "remote.h"
#include "config_mec.h"
#include "sd_motor.h"
#include "pid.h"

typedef enum
{
	R_LEG = 0,
	L_LEG ,
	
	LEG_LIST
}leg_list_e;

#define R_JOINT_ORG_ANGLE     0.63f
#define L_JOINT_ORG_ANGLE     3.54f;

#define JOINT_RAD_TAR_MAX     0.7f
#define JOINT_RAD_SPEED_MAX   0.35f

typedef struct Leg_Target_struct_t
{
	float speed;
	
	float leg_length;
	
	float joint_rad;
}Leg_Target_t;

typedef struct Leg_Measure_struct_t
{
	float speed;
	
	float leg_length;
	
	float phi4_rad;
}Leg_Measure_t;

typedef struct Joint_struct_t
{
	Leg_Target_t* target;
	
	Leg_Measure_t* measure;
	
}Joint_t;

extern Joint_t My_Joint[LEG_LIST];

void My_Joint_Control(void);
void My_Joint_Target_Process(void);
void My_Joint_Torque_Cal(void);
float Joint_Double_PID_Cal(pid_t* pid_out, pid_t* pid_in);
void My_Joint_Roll_Torque_Cal(void);
void My_Jump_Flag_Detect(void);

#endif
