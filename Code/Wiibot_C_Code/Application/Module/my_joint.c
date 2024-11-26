#include "my_joint.h"
#include "config_mec.h"

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
	
}


