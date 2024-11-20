#include "dev_imu.h"
#include "config_math.h"

WbDeviceTag Imu;
WbDeviceTag Gyro;
WbDeviceTag Acce;

Posture_t My_Posture;

void My_Imu_Init(void)
{
  Imu = wb_robot_get_device("My_Imu");
  wb_inertial_unit_enable(Imu, TIME_STEP);
  
  Gyro = wb_robot_get_device("My_Gyro");
  wb_gyro_enable(Gyro, TIME_STEP);
  
  Acce = wb_robot_get_device("My_Accelerometer");
  wb_accelerometer_enable(Acce, TIME_STEP);
}

void My_Imu_Data_Update(void)
{
  const double *imu = wb_inertial_unit_get_roll_pitch_yaw(Imu);
  My_Posture.roll = imu[2];
  My_Posture.pitch = imu[1];
  My_Posture.yaw = imu[1];

//   if((My_Posture.yaw + PI) > PI)
//   {
//     My_Posture.yaw = PI - My_Posture.yaw;

//     My_Posture.yaw *= -1;
//   }
//   else
//   {
//     My_Posture.yaw += PI;
//   }
  
  const double *gyro = wb_gyro_get_values(Gyro);
  My_Posture.roll_v = gyro[0];
  My_Posture.yaw_v = gyro[2];
  My_Posture.pitch_v = gyro[1];
  
  const double *acce = wb_accelerometer_get_values(Acce);
  My_Posture.x_acc = acce[0];
  My_Posture.y_acc = acce[1];
  My_Posture.z_acc = acce[2];

  
}
