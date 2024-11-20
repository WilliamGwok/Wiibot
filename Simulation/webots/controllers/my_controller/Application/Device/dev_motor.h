#ifndef __DEV_MOTOR_H
#define __DEV_MOTOR_H

#include "config_environment.h"

typedef enum Motor_List
{
  R_Sd_Motor = 0,
  
  R_W_Motor,
  
  L_Sd_Motor,
  
  L_W_Motor,
  
  Motor_Total_Num,
}Motor_List_e;

typedef struct Motor_struct_t
{
  char name[DEVICE_NAME_LENGTH];
  
  double output_torque;
  
  double distance_now;
  
  double distance_last;
  
  double linear_velocity;
  
  double angular_velocity;

  double torque_measure;
    
}Motor_t;

extern Motor_t My_Motor[Motor_Total_Num];
extern WbDeviceTag Motor[Motor_Total_Num];

void My_Motor_Init(void);
void My_Motor_Data_Update(void);
void My_Motor_Work(double tw_l, double tw_r);

#endif


