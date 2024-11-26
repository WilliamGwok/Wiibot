#ifndef __KTECH_MOTOR_H
#define __KTECH_MOTOR_H

#include "stm32f4xx_hal.h"
#include "config.h"
#include "config_can.h"
#include "drv_can.h"
#include "string.h"
#include "math_support.h"

typedef enum Motor_Ktech_Type_enum_e
{
		motor_9025,
	
		motor_8016,
	
	  motor_6012,
}Motor_Ktech_Type_e;

typedef enum Motor_Ktech_Wrong_State_enum_e
{
	Normal,
	
	Abnormal,
}Motor_Ktech_Wrong_State_e;

typedef enum Motor_Ktech_Command_enum_e
{
	PID_RX_ID				   = 0x30,  //读取电机位置环，速度环，转矩环PI参数
	PID_TX_RAM_ID			   = 0x31,  //写入PID参数到RAM,断电失效
	PID_TX_ROM_ID			   = 0x32,  //写入PID参数到ROM,断电有效
	ACCEL_RX_ID				   = 0x33,  //读取电机加速度
	ACCEL_TX_ID				   = 0x34,  //写入加速度到RAM，断电失效
	ENCODER_RX_ID		       = 0x90,  //获取编码器值
	ZERO_ENCODER_TX_ID		   = 0x91,	//设置零偏，不常用
	ZERO_POSNOW_TX_ID		   = 0x19,	//写入当前位置到ROM作为电机零点，不建议使用
	
	MOTOR_ANGLE_ID			   = 0x92,  //电机总角度
	CIRCLE_ANGLE_ID			   = 0x94,  //单圈角度
	STATE1_ID				   = 0x9A,  //电机状态1
	CLEAR_ERROR_State_ID	   = 0x9B,  //清除电机错误状态
	STATE2_ID				   = 0x9C,  //电机状态2
	STATE3_ID				   = 0x9D,  //电机状态3
	MOTOR_CLOSE_ID			   = 0x80,  //
	MOTOR_STOP_ID			   = 0x81,
	MOTOR_RUN_ID			   = 0x88,
	
	TORQUE_OPEN_LOOP_ID        = 0xA0,  
	TORQUE_CLOSE_LOOP_ID       = 0xA1,  //转矩闭环控制
	SPEED_CLOSE_LOOP_ID        = 0XA2,
	POSI_CLOSE_LOOP_ID1        = 0XA3,   
	POSI_CLOSE_LOOP_ID2        = 0XA4,
	POSI_CLOSE_LOOP_ID3        = 0XA5,
	POSI_CLOSE_LOOP_ID4        = 0XA6,
	POSI_CLOSE_LOOP_ID5        = 0XA7,  //闭环位置增量控制
	POSI_CLOSE_LOOP_ID6        = 0XA8,

}Motor_Ktech_Command_e;

typedef struct Motor_Ktech_Born_Info_struct_t
{
		Motor_Ktech_Type_e ktech_type;
	
    uint32_t stdId;

    CAN_HandleTypeDef *hcan;
	
	  int8_t order_correction;

}Motor_Ktech_Born_Info_t;

typedef struct Motor_Ktech_Rx_Info_struct_t
{
		float torque;
	
	  float torque_current;//数值范围-2048~2048, 对应电流-16.5~16.5A
	
	  int16_t torque_current_raw;
	
		int16_t speed;
	
    uint16_t encoder;//0~65535，类似6020，为绝对角度

    uint16_t encoder_raw;

    uint16_t encoder_offset;

    float motor_angle_sum;

    float motor_angle;//电机弧度制绝对角度，0~2PI
	
	  float motor_angle_last;

    int8_t temperature;
    
    uint16_t voltage;

    uint8_t error_state;
		
		uint8_t angle_PID_Kp;
		
		uint8_t angle_PID_Ki;
		
		uint8_t speed_PID_Kp;
		
		uint8_t speed_PID_Ki;
		
		uint8_t torque_PID_Kp;
		
		uint8_t torque_PID_Ki;
		
		Motor_Ktech_Wrong_State_e voltage_state;
		
		Motor_Ktech_Wrong_State_e temper_state;

}Motor_Ktech_Rx_Info_t;

typedef struct Motor_Ktech_Tx_Info_struct_t
{
		float	torque;//需要发送的转矩
	
	  float torque_current;
	  
	  int16_t torque_current_raw;
	
	  uint8_t ram_torque_kp;
	
	  uint8_t ram_torque_ki;
	
	  uint8_t rom_torque_kp;
	
	  uint8_t rom_torque_ki;
	
		uint8_t single_tx_buff[8];//使用电机发送时的个人数组
}Motor_Ktech_Tx_Info_t;

typedef struct Motor_Ktech_State_struct_t
{
    uint32_t offline_cnt;

    uint32_t offline_cnt_max;

    Dev_Work_State status;

}Motor_Ktech_State_t;

typedef struct Motor_Ktech_struct_t
{
    Motor_Ktech_Born_Info_t* born_info;

    Motor_Ktech_Rx_Info_t* rx_info;
	
		Motor_Ktech_Tx_Info_t* tx_info;

    Motor_Ktech_State_t* state;
		
		void (*pull_request)(struct Motor_Ktech_struct_t *motor, uint8_t command);
	
		void (*single_set_torque)(struct Motor_Ktech_struct_t *motor);
	
	  void (*rx)(struct Motor_Ktech_struct_t *motor, uint8_t *rxBuf);
	
	  void (*single_sleep)(struct Motor_Ktech_struct_t *motor);
	
	  void (*single_init)(struct Motor_Ktech_struct_t *motor);
	
	  void (*single_heart_beat)(struct Motor_Ktech_struct_t *motor);
	
	  void (*single_read_state)(struct Motor_Ktech_struct_t *motor);
		
		void (*single_set_pid)(struct Motor_Ktech_struct_t *motor, uint8_t type);
    
}Motor_Ktech_t;

//因为多电机命令最多只能控制四个电机，故以4个关节电机为一组，2个驱动轮电机为一组，分别挂载在CAN1和CAN2上
typedef struct Motor_Ktech_Group_struct_t
{
		Motor_Ktech_t* motor_1;
	
		Motor_Ktech_t* motor_2;
	
		Motor_Ktech_t* motor_3;
	
		Motor_Ktech_t* motor_4;
	
	  //Motor_Ktech_t* motor[4];为什么不这么写？
	
		uint8_t tx_buff[8];
	
	  uint8_t tx_buff_read[8];
	
	  void (*group_set_torque)(struct Motor_Ktech_Group_struct_t *group);
	
		void (*group_sleep)(struct Motor_Ktech_Group_struct_t *group);
	
	  void (*group_init)(struct Motor_Ktech_Group_struct_t *group);
	
	  void (*group_heartbeat)(struct Motor_Ktech_Group_struct_t *group);
	
}Motor_Ktech_Group_t;

void Motor_Pull_Request(Motor_Ktech_t *motor, uint8_t command);
void Motor_Get_Info(Motor_Ktech_t *motor, uint8_t *rxBuf);
void Motor_Set_Torque(Motor_Ktech_t *motor);
void Group_Motor_Set_Torque(Motor_Ktech_Group_t *group);
void Group_Motor_Sleep(Motor_Ktech_Group_t *group);
void Single_Motor_Init(Motor_Ktech_t *motor);
void Group_Motor_Init(Motor_Ktech_Group_t *group);

#endif



