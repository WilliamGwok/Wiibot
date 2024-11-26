#ifndef __LQR_H
#define __LQR_H

#include "stm32f4xx_hal.h"

typedef struct K_Matrix_struct_t
{
  float wheell_K[6];
  
  float wheelr_K[6];
  
  float K_coefficient[2][36];

}K_Matrix_t;

extern K_Matrix_t My_K_Matrix;

#endif

