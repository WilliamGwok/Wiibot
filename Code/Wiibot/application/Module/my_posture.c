#include "my_posture.h"

void My_Yaw_Fix(void);

Posture_t My_Posture;

uint8_t fix_flag = 0;
uint16_t init_cnt = 0;
float yaw_offset = 0;
float pitch_offset = -0.4f;

void My_Posture_Update(void)
{
	My_Yaw_Fix();
	
	if(fix_flag == 0)
	{
		My_Posture.yaw = 0;
	}
	else
	{
		My_Posture.yaw = INS_angle[0] - yaw_offset;
	}
	
	My_Posture.pitch = INS_angle[1] + pitch_offset;
	
//	My_Posture.pitch = lowpass(My_Posture.pitch, (INS_angle[1] + pitch_offset), 0.2);
	
	My_Posture.roll = INS_angle[2];
	
	My_Posture.yaw_v = bmi088_real_data.gyro[2];
	
	My_Posture.pitch_v = bmi088_real_data.gyro[1];
	
	My_Posture.roll_v = bmi088_real_data.gyro[1];
}

void My_Yaw_Fix(void)
{
	if(fix_flag == 0)
	{
		init_cnt++;
		
		if(init_cnt >= 10000)
		{
			fix_flag = 1;
			
			yaw_offset = INS_angle[0];
		}
	}
}
