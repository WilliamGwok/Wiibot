/**
  ******************************************************************************
  * @file           : math_support.c\h
  * @brief          : 
  * @note           : 2022-3-3 15:27:58
  ******************************************************************************
  */
	
#include "math_support.h"
#include "math.h"


/**
  * @brief  ���ٿ���
  * @param  
  * @retval 
  */
float my_sqrt(float num)
{
    float halfnum = 0.5f * num;
    float y = num;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfnum * y * y));
    return y;
}


/**
  * @brief  ��ͨ�˲�
  * @param  
  * @retval 
  */
float lowpass(float X_last, float X_new, float K)
{
	return (X_last + (X_new - X_last) * K);
}

/**
  * @brief  ��ƽ��ֵ
  * @param  
  * @retval 
  */
float ave(int16_t sum, float ave_last, float new_value)
{
	if(sum < 0)
	{
		return 0;
	}
	return ( (ave_last * sum + new_value) / (sum + 1) );
}

/**
  * @brief  �ַ���ת��ֵ����֧��������֧�ָ�����
  * @param  
  * @retval 
  */
int16_t str_to_num(uint8_t *str,uint16_t len)
{
	uint8_t minus_flag;
	uint16_t i;
	float result = 0;
	
	if(str[0] == '-')
	{
		minus_flag = 1;
		i = 1;
	}
	else
	{
		minus_flag = 0;
		i = 0;
	}
	
	for(;i<len;i++)
	{
		result += (str[i] - '0') * pow(10,len - i - 1);
	}
	
	if(minus_flag == 1)
	{
		result *= -1;
	}
	
	return result;
}

/**
  * @brief  ��ֵת�ַ�������֧�־�λ����λ����������֧�ָ�����
  * @param  
  * @retval 
  */
uint8_t num_to_str(int16_t num, uint8_t *str, uint16_t *len)
{
	uint8_t str_temp[10] = "0000000000";
	uint16_t str_index_temp = 0;
	uint16_t str_len_temp;
	int16_t num_temp = num;

	for(;num != 0;num /= 10)
	{
		str_temp[str_index_temp] = '0' + abs(num % 10);
		str_index_temp++;
	}

	if(str_index_temp > 9)
	{
		return 1;
	}

	/* �����ж� */
	if(num_temp < 0)
	{
		str_temp[str_index_temp] = '-';
		str_index_temp++;
	}

	*len = str_index_temp;

	for(str_len_temp = str_index_temp; str_index_temp > 0; str_index_temp--)
	{
		str[str_len_temp - str_index_temp] = str_temp[str_index_temp-1];
	}

	return 0;
}

/**
  * @brief  ��ֵת�ַ�������ȷ�����ȣ�
  * @param  
  * @retval 
  */
void num_to_str_2(uint16_t num, uint8_t *str, uint16_t len)
{
	uint8_t str_temp[10] = "0000000000";
	uint16_t str_index_temp = 0;

	for(;num != 0;num /= 10)
	{
		str_temp[str_index_temp] = '0' + abs(num % 10);
		str_index_temp++;
	}
	
	int8_t pass = len - str_index_temp;
	if(pass < 0)
	{
		return;
	}
	
	for(int i = len - 1; i >= 0; i--)
	{
		if(i < pass)
		{
			str[i] = '0';
		}
		else 
		{
			str[i] = str_temp[len - 1 - i];
		}
	}
}

/**
  * @brief  ��ֵת�ַ�������ȷ�����ȣ�
  * @param  
  * @retval 
  */
void num_to_str_3(int16_t num, uint8_t *str, uint16_t len)
{
	uint8_t str_temp[10] = "0000000000";
	uint16_t str_index_temp = 0;
	int16_t num_temp = num;

	for(;num != 0;num /= 10)
	{
		str_temp[str_index_temp] = '0' + abs(num % 10);
		str_index_temp++;
	}
	
	/* �����ж� */
	if(num_temp < 0)
	{
		str_temp[str_index_temp] = '-';
		str_index_temp++;
	}
	
	int8_t pass = len - str_index_temp;
	if(pass < 0)
	{
		return;
	}
	
	for(int i = len - 1; i >= 0; i--)
	{
		if(i < pass)
		{
			str[i] = ' ';
		}
		else 
		{
			str[i] = str_temp[len - 1 - i];
		}
	}
}

int16_t RampInt(int16_t final, int16_t now, int16_t ramp)
{
	int32_t buffer = 0;
	
	buffer = final - now;
	if (buffer > 0)
	{
		if (buffer > ramp)
			now += ramp;
		else
			now += buffer;
	}
	else
	{
		if (buffer < -ramp)
			now += -ramp;
		else
			now += buffer;
	}
	return now;
}

float RampFloat(float final, float now, float ramp)
{
	float buffer = 0;
	
	buffer = final - now;
	if (buffer > 0)
	{
		if (buffer > ramp)
			now += ramp;
		else
			now += buffer;
	}
	else
	{
		if (buffer < -ramp)
			now += -ramp;
		else
			now += buffer;
	}

	return now;	
}

float DeathZoom(float input, float center, float death)
{
	if(abs(input - center) < death)
		return center;
	return input;
}


