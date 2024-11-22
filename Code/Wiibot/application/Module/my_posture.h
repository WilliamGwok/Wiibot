#ifndef __MY_POSTURE_H
#define __MY_POSTURE_H

#include "stm32f4xx_hal.h"
#include "dev_all.h"
#include "INS_task.h"

typedef struct Posture_struct_t
{
	float yaw;
	
	float pitch;
	
	float roll;
	
	float yaw_v;
	
	float pitch_v;
	
	float roll_v;
}Posture_t;

extern Posture_t My_Posture;

void My_Posture_Update(void);

#endif
