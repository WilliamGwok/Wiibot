#ifndef __CONTROL_TASK_H
#define __CONTROL_TASK_H

#include "config_environment.h"
#include "dev_all.h"
#include "state_var.h"
#include "straight_leg_model.h"

void My_Robot_Data_Update(void);
void My_Robot_Command_React(void);
void My_Robot_Work(void);
void My_Robot_PID_Control(void);

#endif


