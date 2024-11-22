#ifndef __MY_STATE_VAR_H
#define __MY_STATE_VAR_H

#include "stm32f4xx_hal.h"
#include "my_posture.h"
#include "math_support.h"

typedef struct State_Var_struct_t
{
  float s;
  
  float s_bias;
  
  float sd1;
  
  float phi;
  
  float phid1;
  
  float thetab;
  
  float thetabd1;
  
  /*中间变量用 begin*/
  
  float s_now;
  
  float s_last;
	
	float rw_rad;
	
	float lw_rad;
  
  /*中间变量用 end*/
}State_Var_t;

extern State_Var_t My_State_Var;

void My_State_Var_Update(void);

#endif
