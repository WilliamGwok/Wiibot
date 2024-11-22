#include "my_posture.h"
#include "imu.h"
#include "math_support.h"

Posture_t My_Posture;

float pitch_offset = -0.45f;

void My_Posture_Update(void)
{

	My_Posture.yaw = (imu.base_info->yaw / 360.f) * 2.f * PI;
	
//	My_Posture.pitch = lowpass(My_Posture.pitch, ((imu.base_info->pitch / 360.f) * 2.f * PI + pitch_offset), 0.1f);
	
	My_Posture.pitch = (imu.base_info->pitch / 360.f) * 2.f * PI + pitch_offset;
	
	My_Posture.roll = (imu.base_info->roll / 360.f) * 2 * PI;
	
	My_Posture.yaw_v = gyro[2];
	
	My_Posture.pitch_v = lowpass(My_Posture.pitch_v, gyro[1], 0.05f);
//	My_Posture.pitch_v = gyro[1];
	
	My_Posture.roll_v = gyro[0];
}

