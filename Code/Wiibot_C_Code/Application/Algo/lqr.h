#ifndef __LQR_H
#define __LQR_H

#include "stm32f4xx_hal.h"

typedef struct K_Matrix_struct_t
{
  float wheell_K[6];
  
  float wheelr_K[6];
  
  float K_coefficient[2][30];

}K_Matrix_t;

extern K_Matrix_t My_K_Matrix;

void K_Matrix_Fitting_Update(K_Matrix_t* model, float l_angle_a, float r_angle_a);

#endif

