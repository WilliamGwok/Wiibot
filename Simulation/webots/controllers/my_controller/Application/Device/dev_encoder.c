#include "dev_encoder.h"

WbDeviceTag Encoder[Encoder_Total_Num];
Encoder_t My_Encoder[Encoder_Total_Num] = 
{
  [R_Sd_Encoder] = {
        .name = "R_Sd_Pos",
    },
  [R_W_Encoder] = {
        .name = "R_W_Pos",
    },
  [L_Sd_Encoder] = {
        .name = "L_Sd_Pos",
    },
  [L_W_Encoder] = {
        .name = "L_W_Pos",
    },
};

void My_Encoder_Init(void)
{
  for(int i = 0; i < Encoder_Total_Num; i++)
  {
    Encoder[i] = wb_robot_get_device(My_Encoder[i].name);
    
    wb_position_sensor_enable(Encoder[i], TIME_STEP);
  }
}

void My_Encoder_Data_Update(void)
{
  for(int i = 0; i < Encoder_Total_Num; i++)
  {
    My_Encoder[i].rad_now = wb_position_sensor_get_value(Encoder[i]);
    
    My_Encoder[i].rad_diff = My_Encoder[i].rad_now - My_Encoder[i].rad_last;
    
    My_Encoder[i].rad_last = My_Encoder[i].rad_now;
  }
}













