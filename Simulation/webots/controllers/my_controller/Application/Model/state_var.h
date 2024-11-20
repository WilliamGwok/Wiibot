#ifndef __STATE_VAR_H
#define __STATE_VAR_H

#include "config_environment.h"
#include "config_mec.h"
#include "dev_all.h"

typedef struct State_Var_struct_t
{
  double s;
  
  double s_bias;
  
  double sd1;
  
  double phi;
  
  double phid1;
  
  double thetab;
  
  double thetabd1;
  
  /*中间变量用 begin*/
  
  double s_now;
  
  double s_last;
  
  /*中间变量用 end*/
}State_Var_t;

extern State_Var_t My_State_Var;

void My_State_Var_Update(void);

#endif


