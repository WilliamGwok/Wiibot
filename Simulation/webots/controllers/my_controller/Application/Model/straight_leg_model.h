#ifndef __STRAIGHT_LEG_MODEL_H
#define __STRAIGHT_LEG_MODEL_H

#include "config_environment.h"
#include "state_var.h"
#include "lqr.h"
#include "pid.h"
#include "config_math.h"

extern pid_t Stand_PID;
extern pid_t Distance_PID;
extern pid_t Spin_PID;

typedef struct Straight_Leg_Model_struct_t
{

  double L_Tw;
  
  double R_Tw;

}Straight_Leg_Model_t;

extern Straight_Leg_Model_t My_Straight_Leg_Model;

void My_Model_Output_Cal(void);
double My_Yaw_Zero_Point_Process(double err);
double My_PID_Cal(double tar, double mea, pid_t * pid);

#endif


