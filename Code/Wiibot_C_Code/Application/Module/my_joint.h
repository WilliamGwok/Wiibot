#ifndef __MY_JOINT_H
#define __MY_JOINT_H

#include "stm32f4xx_hal.h"
#include "my_posture.h"
#include "math_support.h"

typedef enum
{
	R_LEG = 0,
	L_LEG ,
	
	LEG_LIST
}leg_list_e;

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
	
	float joint_rad;
}Leg_Measure_t;

typedef struct Joint_struct_t
{
	Leg_Target_t* target;
	
	Leg_Measure_t* measure;
	
}Joint_t;

#endif
