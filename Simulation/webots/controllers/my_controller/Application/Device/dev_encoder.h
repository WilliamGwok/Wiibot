#ifndef __DEV_ENCODER_H
#define __DEV_ENCODER_H

#include "config_environment.h"
#include "dev_encoder.h"

typedef enum Encoder_List
{
  R_Sd_Encoder = 0,
  
  R_W_Encoder,

  L_Sd_Encoder,
  
  L_W_Encoder,
  
  Encoder_Total_Num,
}Encoder_List_e;

typedef struct Encoder_struct_t
{
  char name[DEVICE_NAME_LENGTH];
  
  double rad_now;
  
  double rad_last;
  
  double rad_diff;  
}Encoder_t;

extern Encoder_t My_Encoder[Encoder_Total_Num];

void My_Encoder_Init(void);
void My_Encoder_Data_Update(void);

#endif


