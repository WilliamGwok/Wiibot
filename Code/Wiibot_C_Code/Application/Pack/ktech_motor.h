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
	PID_RX_ID				   = 0x30,  //��ȡ���λ�û����ٶȻ���ת�ػ�PI����
	PID_TX_RAM_ID			   = 0x31,  //д��PID������RAM,�ϵ�ʧЧ
	PID_TX_ROM_ID			   = 0x32,  //д��PID������ROM,�ϵ���Ч
	ACCEL_RX_ID				   = 0x33,  //��ȡ������ٶ�
	ACCEL_TX_ID				   = 0x34,  //д����ٶȵ�RAM���ϵ�ʧЧ
	ENCODER_RX_ID		       = 0x90,  //��ȡ������ֵ
	ZERO_ENCODER_TX_ID		   = 0x91,	//������ƫ��������
	ZERO_POSNOW_TX_ID		   = 0x19,	//д�뵱ǰλ�õ�ROM��Ϊ�����㣬������ʹ��
	
	MOTOR_ANGLE_ID			   = 0x92,  //����ܽǶ�
	CIRCLE_ANGLE_ID			   = 0x94,  //��Ȧ�Ƕ�
	STATE1_ID				   = 0x9A,  //���״̬1
	CLEAR_ERROR_State_ID	   = 0x9B,  //����������״̬
	STATE2_ID				   = 0x9C,  //���״̬2
	STATE3_ID				   = 0x9D,  //���״̬3
	MOTOR_CLOSE_ID			   = 0x80,  //
	MOTOR_STOP_ID			   = 0x81,
	MOTOR_RUN_ID			   = 0x88,
	
	TORQUE_OPEN_LOOP_ID        = 0xA0,  
	TORQUE_CLOSE_LOOP_ID       = 0xA1,  //ת�رջ�����
	SPEED_CLOSE_LOOP_ID        = 0XA2,
	POSI_CLOSE_LOOP_ID1        = 0XA3,   
	POSI_CLOSE_LOOP_ID2        = 0XA4,
	POSI_CLOSE_LOOP_ID3        = 0XA5,
	POSI_CLOSE_LOOP_ID4        = 0XA6,
	POSI_CLOSE_LOOP_ID5        = 0XA7,  //�ջ�λ����������
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
	
	  float torque_current;//��ֵ��Χ-2048~2048, ��Ӧ����-16.5~16.5A
	
	  int16_t torque_current_raw;
	
		int16_t speed;
	
    uint16_t encoder;//0~65535������6020��Ϊ���ԽǶ�

    uint16_t encoder_raw;

    uint16_t encoder_offset;

    float motor_angle_sum;

    float motor_angle;//��������ƾ��ԽǶȣ�0~2PI
	
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
		float	torque;//��Ҫ���͵�ת��
	
	  float torque_current;
	  
	  int16_t torque_current_raw;
	
	  uint8_t ram_torque_kp;
	
	  uint8_t ram_torque_ki;
	
	  uint8_t rom_torque_kp;
	
	  uint8_t rom_torque_ki;
	
		uint8_t single_tx_buff[8];//ʹ�õ������ʱ�ĸ�������
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

//��Ϊ�����������ֻ�ܿ����ĸ����������4���ؽڵ��Ϊһ�飬2�������ֵ��Ϊһ�飬�ֱ������CAN1��CAN2��
typedef struct Motor_Ktech_Group_struct_t
{
		Motor_Ktech_t* motor_1;
	
		Motor_Ktech_t* motor_2;
	
		Motor_Ktech_t* motor_3;
	
		Motor_Ktech_t* motor_4;
	
	  //Motor_Ktech_t* motor[4];Ϊʲô����ôд��
	
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



