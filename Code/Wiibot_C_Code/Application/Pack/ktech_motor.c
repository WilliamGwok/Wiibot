#include "ktech_motor.h"

/*有待完善：

电机离线数据处理，如角度和

*/


#define _9025_TORQUE_CONSTANT     0.32f	  //9025转矩常数，单位：N*m/A 32f
#define _8016_TORQUE_CONSTANT			0.24f		//8016转矩常数，单位：N*m/A
#define _6012_TORQUE_CONSTANT     1.09f   //6012转矩常数，单位：N*m/A
#define _9025_MAX_CURRENT         16.f    //9025输出最大电流，手册-16.5~16.5
#define _8016_MAX_CURRENT         32.f    //8016输出最大电流，手册-33~33
#define _6012_MAX_CURRENT         14.f    //8016输出最大电流，手册-33~33

/**
  * @brief          将电机接收原始电流数据转为实际转矩
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Raw_Current_to_Torque(Motor_Ktech_t *motor);

/**
  * @brief          将电机期望输出扭矩转为原始电流数据,用于发送数据的处理
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Torque_to_Raw_Current(Motor_Ktech_t *motor);

/**
  * @brief          将编码器值转化为弧度制，并计算电机角度和
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Encoder_to_Motor_Angle(Motor_Ktech_t *motor);

/**
  * @brief          计算电机旋转角度和
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Angle_Sum_Cal(Motor_Ktech_t *motor);

/**
  * @brief          >>电机组控制专用<<将电机期望输出扭矩转为原始电流数据,用于发送数据的处理
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Group_Torque_to_Raw_Current(Motor_Ktech_t *motor);

/**
  * @brief          单电机读取状态1，包括电机温度，电压，错误标志位
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Single_Motor_Read_State_1(Motor_Ktech_t *motor);

/**
  * @brief          单电机设置PID参数到RAM 0 ROM 1
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Single_Motor_Set_PI(Motor_Ktech_t *motor, uint8_t type);


/**
  * @brief          向电机发送指令请求获得电机信息，会给数组第一个位赋值,内含发送操作
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @param[in]      uint8_t command         请求命令
  * @retval         none
  */
void Motor_Pull_Request(Motor_Ktech_t *motor, uint8_t command)
{
    memset(motor->tx_info->single_tx_buff, 0, 8);

    switch(command)
    {
        case TORQUE_CLOSE_LOOP_ID://返回电机温度，转矩，速度
            motor->tx_info->single_tx_buff[0] = TORQUE_CLOSE_LOOP_ID;
            break;
        case ENCODER_RX_ID://返回编码器三个值
            motor->tx_info->single_tx_buff[0] = ENCODER_RX_ID;
            break;
        case MOTOR_ANGLE_ID://返回总角度 int64_t
            motor->tx_info->single_tx_buff[0] = MOTOR_ANGLE_ID;
            break;
        default:
            break;
    }
		
		CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
		
		memset(motor->tx_info->single_tx_buff, 0, 8);
}

/**
  * @brief          电机CAN中断接收数据处理
  * @param[in]      Motor_Ktech_t *motor      电机本体
  * @param[in]      uint8_t *rxBuf						CAN接收数据包
  * @retval         none
  */
void Motor_Get_Info(Motor_Ktech_t *motor, uint8_t *rxBuf)
{
	uint8_t command_bit = rxBuf[0];
	
	switch(command_bit)
	{
		case TORQUE_CLOSE_LOOP_ID://返回电机温度，转矩电流，速度,编码器位置
				motor->rx_info->temperature = rxBuf[1];
		
				motor->rx_info->torque_current_raw = rxBuf[3];
				motor->rx_info->torque_current_raw <<= 8;
				motor->rx_info->torque_current_raw |= rxBuf[2];
		    Raw_Current_to_Torque(motor);
		
				motor->rx_info->speed = rxBuf[5];
				motor->rx_info->speed <<= 8;
				motor->rx_info->speed |= rxBuf[4];
		
				motor->rx_info->encoder = rxBuf[7];//类似6020云台，
				motor->rx_info->encoder <<= 8;
				motor->rx_info->encoder |= rxBuf[6];
				Encoder_to_Motor_Angle(motor);
		
          break;
    case ENCODER_RX_ID://返回编码器三个值
			  motor->rx_info->encoder_raw = rxBuf[5];
		    motor->rx_info->encoder_raw <<= 8;
		    motor->rx_info->encoder_raw |= rxBuf[4];
		
				motor->rx_info->encoder_offset = rxBuf[7];
		    motor->rx_info->encoder_offset <<= 8;
		    motor->rx_info->encoder_offset |= rxBuf[6];
        break;
    case MOTOR_ANGLE_ID://返回总角度 int64_t
        break;
		case STATE1_ID:
			  motor->rx_info->temperature = rxBuf[1];
		    motor->rx_info->voltage = (uint16_t)((rxBuf[3]<<8)|rxBuf[2]);
//		    motor->rx_info->voltage <<= 8;
//        motor->rx_info->voltage |= rxBuf[3];		
			  motor->rx_info->error_state = rxBuf[7];
//			  motor->rx_info->voltage_state = 0x01 && motor->rx_info->error_state;
//		    motor->rx_info->temper_state = 0x08 && motor->rx_info->error_state;
			  break;
		case PID_RX_ID:
			  motor->rx_info->angle_PID_Kp = rxBuf[2];
		    motor->rx_info->angle_PID_Ki = rxBuf[3];
				motor->rx_info->speed_PID_Kp = rxBuf[4];
		    motor->rx_info->speed_PID_Ki = rxBuf[5];
				motor->rx_info->torque_PID_Kp = rxBuf[6];
		    motor->rx_info->torque_PID_Ki = rxBuf[7];
			  break;
		case PID_TX_RAM_ID:
			  motor->rx_info->angle_PID_Kp = rxBuf[2];
		    motor->rx_info->angle_PID_Ki = rxBuf[3];
				motor->rx_info->speed_PID_Kp = rxBuf[4];
		    motor->rx_info->speed_PID_Ki = rxBuf[5];
				motor->rx_info->torque_PID_Kp = rxBuf[6];
		    motor->rx_info->torque_PID_Ki = rxBuf[7];
			  break;
    default:
        break;
	}
}

/**
  * @brief          单电机控制输出转矩,含有CAN发送操作
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
void Motor_Set_Torque(Motor_Ktech_t *motor)
{
	  if(motor != NULL)
		{
			  Torque_to_Raw_Current(motor);
	
		    motor->tx_info->single_tx_buff[0]    = TORQUE_CLOSE_LOOP_ID; //扭矩闭环控制
		    motor->tx_info->single_tx_buff[1]    = 0x00;
		    motor->tx_info->single_tx_buff[2]    = 0x00;
        motor->tx_info->single_tx_buff[3]    = 0x00;
        motor->tx_info->single_tx_buff[4]    = (uint8_t) (motor->tx_info->torque_current_raw);
        motor->tx_info->single_tx_buff[5]    = (uint8_t) (motor->tx_info->torque_current_raw >> 8);
		    motor->tx_info->single_tx_buff[6]    = 0x00;
        motor->tx_info->single_tx_buff[7]    = 0x00;
		    
		    CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
		    
		    memset(motor->tx_info->single_tx_buff,0,8);
		}
}

/**
  * @brief          单电机卸力
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
void Single_Motor_Sleep(Motor_Ktech_t *motor)
{
	if(motor != NULL)
	{
		  motor->tx_info->single_tx_buff[0]    = TORQUE_CLOSE_LOOP_ID; //扭矩闭环控制
		  
		  memset(&motor->tx_info->single_tx_buff[1],0,7);
		  
		  CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
	}
}

/**
  * @brief          多电机（1~4个）电机控制输出转矩,4个电机必须为同一类型；含有CAN发送操作
  * @param[in]      Motor_Ktech_Group_t *group     电机组
  * @retval         none
  */
void Group_Motor_Set_Torque(Motor_Ktech_Group_t *group)
{	
		int16_t torque_current_1 = 0, torque_current_2 = 0, torque_current_3 = 0, torque_current_4 = 0;
		
		if(group->motor_1 != NULL)
		{
			Group_Torque_to_Raw_Current(group->motor_1);//转化函数与单电机不同
			torque_current_1 = group->motor_1->tx_info->torque_current_raw;
		}
		if(group->motor_2 != NULL)
		{
			Group_Torque_to_Raw_Current(group->motor_2);
			torque_current_2 = group->motor_2->tx_info->torque_current_raw;
		}
		if(group->motor_3 != NULL)
		{
			Group_Torque_to_Raw_Current(group->motor_3);
			torque_current_3 = group->motor_3->tx_info->torque_current_raw;
		}
		if(group->motor_4 != NULL)
		{
			Group_Torque_to_Raw_Current(group->motor_4);
			torque_current_4 = group->motor_4->tx_info->torque_current_raw;
		}
		
		group->tx_buff[0]    = (uint8_t) (torque_current_1); 
		group->tx_buff[1]    = (uint8_t) (torque_current_1 >> 8);
		group->tx_buff[2]    = (uint8_t) (torque_current_2);
    group->tx_buff[3]    = (uint8_t) (torque_current_2 >> 8);
    group->tx_buff[4]    = (uint8_t) (torque_current_3);
    group->tx_buff[5]    = (uint8_t) (torque_current_3 >> 8);
		group->tx_buff[6]    = (uint8_t) (torque_current_4);
    group->tx_buff[7]    = (uint8_t) (torque_current_4 >> 8);
		
		memcpy(group->tx_buff_read, group->tx_buff, 8);
		
		CAN_SendData(group->motor_1->born_info->hcan, MULTI_KT_MOTOR_GROUP, group->tx_buff);
		
		memset(group->tx_buff, 0, 8);
}

/**
  * @brief          电机组卸力
  * @param[in]      Motor_Ktech_Group_t *group     电机组
  * @retval         none
  */
void Group_Motor_Sleep(Motor_Ktech_Group_t *group)
{		
		memset(group->tx_buff,0,8);
		
		CAN_SendData(group->motor_1->born_info->hcan, MULTI_KT_MOTOR_GROUP, group->tx_buff);
}

/**
  * @brief          单电机心跳包
  * @param[in]      Motor_Ktech_t *motor    电机本体
  * @retval         none
  */
void Ktech_Motor_Hearbeat(Motor_Ktech_t *motor)
{
	motor->state->offline_cnt++;
	
	if(motor->state->offline_cnt > motor->state->offline_cnt_max) 
	{
		motor->state->offline_cnt = motor->state->offline_cnt_max;
		motor->state->status = DEV_OFFLINE;
	}
	else 
	{
		if(motor->state->status == DEV_OFFLINE)
			motor->state->status = DEV_ONLINE;
	}
}

/**
  * @brief          电机组心跳包
  * @param[in]      Motor_Ktech_Group_t *group     电机组
  * @retval         none
  */
void Group_Motor_Heartbeat(Motor_Ktech_Group_t *group)
{
	if(group->motor_1 != NULL)
	{
		group->motor_1->single_heart_beat(group->motor_1);
	}
	  
	if(group->motor_2 != NULL)
	{
		group->motor_2->single_heart_beat(group->motor_2);
	}

  if(group->motor_3 != NULL)
	{
		group->motor_3->single_heart_beat(group->motor_3);
	}
	  
	if(group->motor_4 != NULL)
	{
		group->motor_4->single_heart_beat(group->motor_4);
	}
}

/**
  * @brief          单电机初始化
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
void Single_Motor_Init(Motor_Ktech_t *motor)
{
		motor->pull_request = Motor_Pull_Request;
	  
		motor->single_set_torque = Motor_Set_Torque;
	
	  motor->rx = Motor_Get_Info;
	
	  motor->single_sleep = Single_Motor_Sleep;
	
	  motor->single_heart_beat = Ktech_Motor_Hearbeat;
	
	  motor->single_read_state = Single_Motor_Read_State_1;
	
	  motor->single_set_pid = Single_Motor_Set_PI;
	
	  motor->state->offline_cnt_max = 100;
}

/**
  * @brief          电机组初始化
  * @param[in]      Motor_Ktech_Group_t *group     电机组
  * @retval         none
  */
void Group_Motor_Init(Motor_Ktech_Group_t *group)
{
	  if(group->motor_1 != NULL)
		{
			group->motor_1->single_init = Single_Motor_Init;
	    group->motor_1->single_init(group->motor_1);
		}
	  
		if(group->motor_2 != NULL)
		{
			group->motor_2->single_init = Single_Motor_Init;
	    group->motor_2->single_init(group->motor_2);
		}

    if(group->motor_3 != NULL)
		{
			group->motor_3->single_init = Single_Motor_Init;
	    group->motor_3->single_init(group->motor_3);
		}
	  
		if(group->motor_4 != NULL)
		{
			group->motor_4->single_init = Single_Motor_Init;
	    group->motor_4->single_init(group->motor_4);
		}
	  
	
	  group->group_set_torque = Group_Motor_Set_Torque;
		group->group_heartbeat = Group_Motor_Heartbeat;
	  group->group_sleep = Group_Motor_Sleep;
}





/*..........................................工具函数..........................................*/

/**
  * @brief          将电机接收原始电流数据转为实际转矩,用于接收数据的处理
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Raw_Current_to_Torque(Motor_Ktech_t *motor)
{
	/*电流转为转矩*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		/*电流数值转为电流*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 16.5f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _9025_TORQUE_CONSTANT;
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		/*电流数值转为电流*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 33.f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _8016_TORQUE_CONSTANT * 6.f;
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		/*电流数值转为电流*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 33.f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _6012_TORQUE_CONSTANT;
	}
}

/**
  * @brief          将电机期望输出扭矩转为原始电流数据,用于发送数据的处理
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Torque_to_Raw_Current(Motor_Ktech_t *motor)
{
	/*转矩转化为转矩电流*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		motor->tx_info->torque_current = motor->tx_info->torque / _9025_TORQUE_CONSTANT;
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_9025_MAX_CURRENT, _9025_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 16.5f) * 2048.f);
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 6.f) / _8016_TORQUE_CONSTANT;//要算上减速比
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_8016_MAX_CURRENT, _8016_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 33.f) * 2048.f);
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque) / _6012_TORQUE_CONSTANT;//要算上减速比
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_6012_MAX_CURRENT, _6012_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 33.f) * 2048.f);
	}
}

/**
  * @brief          >>电机组控制专用<<将电机期望输出扭矩转为原始电流数据,用于发送数据的处理
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Group_Torque_to_Raw_Current(Motor_Ktech_t *motor)
{
	/*转矩转化为转矩电流*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		motor->tx_info->torque_current = motor->tx_info->torque / _9025_TORQUE_CONSTANT;
    motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_9025_MAX_CURRENT, _9025_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 16.5f) * 2048.f);//使用手册害人不浅
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 6.f) / _8016_TORQUE_CONSTANT;//要算上减速比
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_8016_MAX_CURRENT, _8016_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 32.f) * 2000.f);
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 8.f) / _6012_TORQUE_CONSTANT;//要算上减速比
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_6012_MAX_CURRENT, _6012_MAX_CURRENT);//最大电流限幅
		
		/*转矩电流转化为电流数值*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 32.f) * 2000.f);
	}
}

/**
  * @brief          将编码器值转化为弧度制，并计算电机角度和,分9025和8016
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Encoder_to_Motor_Angle(Motor_Ktech_t *motor)
{
	motor->rx_info->motor_angle = ((float)motor->rx_info->encoder / 65535.f) * 2.f * (float)PI;
	
	Angle_Sum_Cal(motor);
}

/**
  * @brief          计算电机旋转角度和
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Angle_Sum_Cal(Motor_Ktech_t *motor)
{
	float err = 0.f;
	
	float order_correction = 0.f;
	
	if(motor->born_info->order_correction == 1 || motor->born_info->order_correction == -1)
	{
		order_correction = (float)motor->born_info->order_correction;
	}
	else
	{
		order_correction = 1.f;
	}
	
	if(!motor->rx_info->motor_angle_last && !motor->rx_info->motor_angle_sum)//上一角度值为0且角度和为零时（电机启动），不计算误差
	{
		err = 0.f;
	}
	else
	{
		err = motor->rx_info->motor_angle - motor->rx_info->motor_angle_last;
	}
	
	if(abs(err) > (float)PI)//过零点
	{
		if(err > 0.f)
		{
			motor->rx_info->motor_angle_sum += (-(float)PI * 2.f + err) * order_correction;
		}
		else
		{
			motor->rx_info->motor_angle_sum += ((float)PI * 2.f + err) * order_correction;
		}
	}
	else
	{
		motor->rx_info->motor_angle_sum += err * order_correction;
	}
	
	motor->rx_info->motor_angle_last = motor->rx_info->motor_angle;
}

/**
  * @brief          单电机读取状态1，包括电机温度，电压，错误标志位
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Single_Motor_Read_State_1(Motor_Ktech_t *motor)
{
	memset(motor->tx_info->single_tx_buff,0,8);
	
	motor->tx_info->single_tx_buff[0] = STATE1_ID;
		
  CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
}

/**
  * @brief          单电机设置PID参数到RAM 0 ROM 1
  * @param[in]      Motor_Ktech_t *motor     电机本体
  * @retval         none
  */
static void Single_Motor_Set_PI(Motor_Ktech_t *motor, uint8_t type)
{
	memset(motor->tx_info->single_tx_buff,0,8);
	
	if(type == 0)
	{
		motor->tx_info->single_tx_buff[0] = PID_TX_RAM_ID;
	}
	else
	{
		motor->tx_info->single_tx_buff[0] = PID_TX_ROM_ID;
	}
	
	motor->tx_info->single_tx_buff[6] = motor->tx_info->ram_torque_kp;
	
	motor->tx_info->single_tx_buff[7] = motor->tx_info->ram_torque_ki;
	
	CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
}

