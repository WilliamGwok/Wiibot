/**
  ******************************************************************************
  * @file           : imu.c\h
  * @brief          : 
  * @note           : finish 2022-2-13 19:59:24
  ******************************************************************************
  */
	
#include "imu.h"
#include "BMI088driver.h"
#include "bmi.h"
#include "stdbool.h"
#include "math_support.h"
#include "ave_filter.h"

imu_config_t imu_config;
imu_base_info_t imu_base_info;
imu_info_t imu_info;
imu_t imu = 
{
	.base_info = &imu_base_info,
	.config = &imu_config,
	.info = &imu_info,
};

float gyro[3], accel[3], temp;

ave_filter_t imu_pitch_dif_speed_ave_filter;
ave_filter_t imu_roll_dif_speed_ave_filter;
ave_filter_t imu_yaw_dif_speed_ave_filter;

/**
  * @brief  
  * @param  
  * @retval 
  */
void imu_update(imu_t *imu)
{
	imu_base_info_t *base_info = imu->base_info;
	
	/* 获取陀螺仪数据 */
	
	BMI088_read(gyro, accel, &temp);
	
	base_info->temp = temp;
	
//	Gyro2World(0.f , -0.7072f, 0.7071f , 0.f, gyro[0], gyro[1], gyro[2],\
//						&base_info->ggx, &base_info->ggy, &base_info->ggz);
//	Gyro2World(0.f , -0.7072f, 0.7071f , 0.f, accel[0], accel[1], accel[2],\
//						&base_info->aax, &base_info->aay, &base_info->aaz);
	
	
	base_info->ggx = gyro[0]; base_info->ggy = gyro[1]; base_info->ggz = gyro[2];
	base_info->aax = accel[0]; base_info->aay = accel[1]; base_info->aaz = accel[2]; 
	//使用C板原始数据

	/* 原始数据低通滤波 */
	base_info->ggx_K = lowpass(base_info->ggx_K, base_info->ggx, 0.1);
	base_info->ggy_K = lowpass(base_info->ggy_K, base_info->ggy, 0.1);
	base_info->ggz_K = lowpass(base_info->ggz_K, base_info->ggz, 0.1);
	base_info->aax_K = lowpass(base_info->aax_K, base_info->aax, 1);//imu->config->acce_lowpass);
	base_info->aay_K = lowpass(base_info->aay_K, base_info->aay, 1);//imu->config->acce_lowpass);
	base_info->aaz_K = lowpass(base_info->aaz_K, base_info->aaz, 1);//imu->config->acce_lowpass);

	/* 解算陀螺仪数据 */
	BMI_Get_EulerAngle(&base_info->pitch,  &base_info->roll,  &base_info->yaw,\
	                   &base_info->pitch_, &base_info->roll_, &base_info->yaw_,\
	                   &base_info->ggx_K,  &base_info->ggy_K, &base_info->ggz_K,\
	                   &base_info->aax_K,  &base_info->aay_K, &base_info->aaz_K);

}


