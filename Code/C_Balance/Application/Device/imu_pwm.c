#include "imu_pwm.h"
#include "main.h"
#include "dji_pid.h"

#define TEMPERATURE_PID_KP 1600.0f //kp of temperature control PID 
#define TEMPERATURE_PID_KI 0.2f    //ki of temperature control PID 
#define TEMPERATURE_PID_KD 0.0f    //kd of temperature control PID 

#define TEMPERATURE_PID_MAX_OUT 4500.0f  //max out of temperature control PID 
#define TEMPERATURE_PID_MAX_IOUT 4400.0f //max iout of temperature control PID 

//kp, ki,kd three params
const fp32 imu_temp_PID[3] = {TEMPERATURE_PID_KP, TEMPERATURE_PID_KI, TEMPERATURE_PID_KD};
//pid struct 
pid_type_def imu_temp_pid;

void Imu_PWM_Init(void)
{
	//pid init  PID≥ı ºªØ
  PID_init(&imu_temp_pid, PID_POSITION, imu_temp_PID, TEMPERATURE_PID_MAX_OUT, TEMPERATURE_PID_MAX_IOUT);
}

void Imu_PWM_Set(fp32 temp)
{
	uint16_t tempPWM;
  //pid calculate. PIDº∆À„
  PID_calc(&imu_temp_pid, temp, 40.0f);
  if (imu_temp_pid.out < 0.0f)
  {
     imu_temp_pid.out = 0.0f;
  }
  tempPWM = (uint16_t)imu_temp_pid.out;
	
	
  TIM10->CCR1 = (tempPWM);

}
