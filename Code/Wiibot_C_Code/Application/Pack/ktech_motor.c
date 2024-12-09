#include "ktech_motor.h"

/*�д����ƣ�

����������ݴ�����ǶȺ�

*/


#define _9025_TORQUE_CONSTANT     0.32f	  //9025ת�س�������λ��N*m/A 32f
#define _8016_TORQUE_CONSTANT			0.24f		//8016ת�س�������λ��N*m/A
#define _6012_TORQUE_CONSTANT     1.09f   //6012ת�س�������λ��N*m/A
#define _9025_MAX_CURRENT         16.f    //9025������������ֲ�-16.5~16.5
#define _8016_MAX_CURRENT         32.f    //8016������������ֲ�-33~33
#define _6012_MAX_CURRENT         14.f    //8016������������ֲ�-33~33

/**
  * @brief          ���������ԭʼ��������תΪʵ��ת��
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Raw_Current_to_Torque(Motor_Ktech_t *motor);

/**
  * @brief          ������������Ť��תΪԭʼ��������,���ڷ������ݵĴ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Torque_to_Raw_Current(Motor_Ktech_t *motor);

/**
  * @brief          ��������ֵת��Ϊ�����ƣ����������ǶȺ�
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Encoder_to_Motor_Angle(Motor_Ktech_t *motor);

/**
  * @brief          ��������ת�ǶȺ�
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Angle_Sum_Cal(Motor_Ktech_t *motor);

/**
  * @brief          >>��������ר��<<������������Ť��תΪԭʼ��������,���ڷ������ݵĴ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Group_Torque_to_Raw_Current(Motor_Ktech_t *motor);

/**
  * @brief          �������ȡ״̬1����������¶ȣ���ѹ�������־λ
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Single_Motor_Read_State_1(Motor_Ktech_t *motor);

/**
  * @brief          ���������PID������RAM 0 ROM 1
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Single_Motor_Set_PI(Motor_Ktech_t *motor, uint8_t type);


/**
  * @brief          ��������ָ�������õ����Ϣ����������һ��λ��ֵ,�ں����Ͳ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @param[in]      uint8_t command         ��������
  * @retval         none
  */
void Motor_Pull_Request(Motor_Ktech_t *motor, uint8_t command)
{
    memset(motor->tx_info->single_tx_buff, 0, 8);

    switch(command)
    {
        case TORQUE_CLOSE_LOOP_ID://���ص���¶ȣ�ת�أ��ٶ�
            motor->tx_info->single_tx_buff[0] = TORQUE_CLOSE_LOOP_ID;
            break;
        case ENCODER_RX_ID://���ر���������ֵ
            motor->tx_info->single_tx_buff[0] = ENCODER_RX_ID;
            break;
        case MOTOR_ANGLE_ID://�����ܽǶ� int64_t
            motor->tx_info->single_tx_buff[0] = MOTOR_ANGLE_ID;
            break;
        default:
            break;
    }
		
		CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
		
		memset(motor->tx_info->single_tx_buff, 0, 8);
}

/**
  * @brief          ���CAN�жϽ������ݴ���
  * @param[in]      Motor_Ktech_t *motor      �������
  * @param[in]      uint8_t *rxBuf						CAN�������ݰ�
  * @retval         none
  */
void Motor_Get_Info(Motor_Ktech_t *motor, uint8_t *rxBuf)
{
	uint8_t command_bit = rxBuf[0];
	
	switch(command_bit)
	{
		case TORQUE_CLOSE_LOOP_ID://���ص���¶ȣ�ת�ص������ٶ�,������λ��
				motor->rx_info->temperature = rxBuf[1];
		
				motor->rx_info->torque_current_raw = rxBuf[3];
				motor->rx_info->torque_current_raw <<= 8;
				motor->rx_info->torque_current_raw |= rxBuf[2];
		    Raw_Current_to_Torque(motor);
		
				motor->rx_info->speed = rxBuf[5];
				motor->rx_info->speed <<= 8;
				motor->rx_info->speed |= rxBuf[4];
		
				motor->rx_info->encoder = rxBuf[7];//����6020��̨��
				motor->rx_info->encoder <<= 8;
				motor->rx_info->encoder |= rxBuf[6];
				Encoder_to_Motor_Angle(motor);
		
          break;
    case ENCODER_RX_ID://���ر���������ֵ
			  motor->rx_info->encoder_raw = rxBuf[5];
		    motor->rx_info->encoder_raw <<= 8;
		    motor->rx_info->encoder_raw |= rxBuf[4];
		
				motor->rx_info->encoder_offset = rxBuf[7];
		    motor->rx_info->encoder_offset <<= 8;
		    motor->rx_info->encoder_offset |= rxBuf[6];
        break;
    case MOTOR_ANGLE_ID://�����ܽǶ� int64_t
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
  * @brief          ������������ת��,����CAN���Ͳ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
void Motor_Set_Torque(Motor_Ktech_t *motor)
{
	  if(motor != NULL)
		{
			  Torque_to_Raw_Current(motor);
	
		    motor->tx_info->single_tx_buff[0]    = TORQUE_CLOSE_LOOP_ID; //Ť�رջ�����
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
  * @brief          �����ж��
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
void Single_Motor_Sleep(Motor_Ktech_t *motor)
{
	if(motor != NULL)
	{
		  motor->tx_info->single_tx_buff[0]    = TORQUE_CLOSE_LOOP_ID; //Ť�رջ�����
		  
		  memset(&motor->tx_info->single_tx_buff[1],0,7);
		  
		  CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
	}
}

/**
  * @brief          ������1~4��������������ת��,4���������Ϊͬһ���ͣ�����CAN���Ͳ���
  * @param[in]      Motor_Ktech_Group_t *group     �����
  * @retval         none
  */
void Group_Motor_Set_Torque(Motor_Ktech_Group_t *group)
{	
		int16_t torque_current_1 = 0, torque_current_2 = 0, torque_current_3 = 0, torque_current_4 = 0;
		
		if(group->motor_1 != NULL)
		{
			Group_Torque_to_Raw_Current(group->motor_1);//ת�������뵥�����ͬ
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
  * @brief          �����ж��
  * @param[in]      Motor_Ktech_Group_t *group     �����
  * @retval         none
  */
void Group_Motor_Sleep(Motor_Ktech_Group_t *group)
{		
		memset(group->tx_buff,0,8);
		
		CAN_SendData(group->motor_1->born_info->hcan, MULTI_KT_MOTOR_GROUP, group->tx_buff);
}

/**
  * @brief          �����������
  * @param[in]      Motor_Ktech_t *motor    �������
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
  * @brief          �����������
  * @param[in]      Motor_Ktech_Group_t *group     �����
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
  * @brief          �������ʼ��
  * @param[in]      Motor_Ktech_t *motor     �������
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
  * @brief          ������ʼ��
  * @param[in]      Motor_Ktech_Group_t *group     �����
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





/*..........................................���ߺ���..........................................*/

/**
  * @brief          ���������ԭʼ��������תΪʵ��ת��,���ڽ������ݵĴ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Raw_Current_to_Torque(Motor_Ktech_t *motor)
{
	/*����תΪת��*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		/*������ֵתΪ����*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 16.5f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _9025_TORQUE_CONSTANT;
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		/*������ֵתΪ����*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 33.f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _8016_TORQUE_CONSTANT * 6.f;
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		/*������ֵתΪ����*/
	  motor->rx_info->torque_current = ((float)motor->rx_info->torque_current_raw / 2048.f) * 33.f;
		
		motor->rx_info->torque = motor->rx_info->torque_current * _6012_TORQUE_CONSTANT;
	}
}

/**
  * @brief          ������������Ť��תΪԭʼ��������,���ڷ������ݵĴ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Torque_to_Raw_Current(Motor_Ktech_t *motor)
{
	/*ת��ת��Ϊת�ص���*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		motor->tx_info->torque_current = motor->tx_info->torque / _9025_TORQUE_CONSTANT;
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_9025_MAX_CURRENT, _9025_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 16.5f) * 2048.f);
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 6.f) / _8016_TORQUE_CONSTANT;//Ҫ���ϼ��ٱ�
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_8016_MAX_CURRENT, _8016_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 33.f) * 2048.f);
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque) / _6012_TORQUE_CONSTANT;//Ҫ���ϼ��ٱ�
		
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_6012_MAX_CURRENT, _6012_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
	  motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 33.f) * 2048.f);
	}
}

/**
  * @brief          >>��������ר��<<������������Ť��תΪԭʼ��������,���ڷ������ݵĴ���
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Group_Torque_to_Raw_Current(Motor_Ktech_t *motor)
{
	/*ת��ת��Ϊת�ص���*/
	if(motor->born_info->ktech_type == motor_9025)
	{
		motor->tx_info->torque_current = motor->tx_info->torque / _9025_TORQUE_CONSTANT;
    motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_9025_MAX_CURRENT, _9025_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 16.5f) * 2048.f);//ʹ���ֲẦ�˲�ǳ
	}
	else if(motor->born_info->ktech_type == motor_8016)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 6.f) / _8016_TORQUE_CONSTANT;//Ҫ���ϼ��ٱ�
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_8016_MAX_CURRENT, _8016_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 32.f) * 2000.f);
	}
	else if(motor->born_info->ktech_type == motor_6012)
	{
		motor->tx_info->torque_current = (motor->tx_info->torque / 8.f) / _6012_TORQUE_CONSTANT;//Ҫ���ϼ��ٱ�
		motor->tx_info->torque_current = constrain(motor->tx_info->torque_current, -_6012_MAX_CURRENT, _6012_MAX_CURRENT);//�������޷�
		
		/*ת�ص���ת��Ϊ������ֵ*/
		motor->tx_info->torque_current_raw = (int16_t)((motor->tx_info->torque_current / 32.f) * 2000.f);
	}
}

/**
  * @brief          ��������ֵת��Ϊ�����ƣ����������ǶȺ�,��9025��8016
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Encoder_to_Motor_Angle(Motor_Ktech_t *motor)
{
	motor->rx_info->motor_angle = ((float)motor->rx_info->encoder / 65535.f) * 2.f * (float)PI;
	
	Angle_Sum_Cal(motor);
}

/**
  * @brief          ��������ת�ǶȺ�
  * @param[in]      Motor_Ktech_t *motor     �������
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
	
	if(!motor->rx_info->motor_angle_last && !motor->rx_info->motor_angle_sum)//��һ�Ƕ�ֵΪ0�ҽǶȺ�Ϊ��ʱ����������������������
	{
		err = 0.f;
	}
	else
	{
		err = motor->rx_info->motor_angle - motor->rx_info->motor_angle_last;
	}
	
	if(abs(err) > (float)PI)//�����
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
  * @brief          �������ȡ״̬1����������¶ȣ���ѹ�������־λ
  * @param[in]      Motor_Ktech_t *motor     �������
  * @retval         none
  */
static void Single_Motor_Read_State_1(Motor_Ktech_t *motor)
{
	memset(motor->tx_info->single_tx_buff,0,8);
	
	motor->tx_info->single_tx_buff[0] = STATE1_ID;
		
  CAN_SendData(motor->born_info->hcan, motor->born_info->stdId, motor->tx_info->single_tx_buff);
}

/**
  * @brief          ���������PID������RAM 0 ROM 1
  * @param[in]      Motor_Ktech_t *motor     �������
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

