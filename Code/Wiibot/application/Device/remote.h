/**
  ******************************************************************************
  * @file           : remote.c/h
  * @brief          : 
  * @note           : ����ʱ�䣺2022��1��11��17:24:36
	*                   ����ٶ��˲�˵����
	*                   �ڸ����ж϶�����ٶȳ�ʼ���ݽ��о�ֵ�˲�
	*                   �ڿ��������ж�����ٶȽ��е�ͨ�˲�
  ******************************************************************************
  */

#ifndef __REMOTE_H
#define __REMOTE_H

#include "main.h"

#define REMOTE_OFFLINE_CNT_MAX  50          //ң����ʧ����������
/*ҡ�����ֵ*/
#define RC_CNT_MAX							(500.f)



#define RC_ONLINE	 (rc.info->status == DEV_ONLINE)


/* ң����������ť״̬ö�� */
typedef enum 
{
  keep_R,         //����
  up_R,           //���ϲ�
  mid_R,          //���в�
  down_R,         //���²�
}remote_status_e;


/* ������Ϣ */
typedef struct
{
  int16_t value_last;  //��һ��ֵ
  int16_t value;       //��ֵ
  uint8_t status;      //״̬
}remote_switch_info_t;


/* ң��ԭʼ��Ϣ */
typedef struct 
{
  /* ң���� */
  int16_t                 ch0;                  //�ҵ�����
  int16_t                 ch1;                  //�ҵ�ǰ��
  int16_t                 ch2;                  //�������
  int16_t                 ch3;                  //���ǰ��
  remote_switch_info_t    s1;                   //�󲦸�
  remote_switch_info_t    s2;                   //�Ҳ���
}rc_base_info_t;

/* ң����Ϣ */
typedef struct 
{
  int16_t             offline_cnt;  //ʧ������
	uint8_t             status;       //״̬
}rc_info_t;

/* ң�� */
typedef struct
{
  rc_base_info_t     *base_info;
  rc_info_t          *info;
}rc_t;

/* �ⲿ���� */
extern rc_t rc;

/* ��ʼ�� */
void RC_Init(rc_t *rc);
void rc_base_info_init(rc_base_info_t *info);
void rc_info_init(rc_info_t *info);
void rc_interrupt_init(rc_base_info_t *info);

/* �ж� */
void rc_base_info_update(rc_t *rc, uint8_t *rxBuf);

/* �������� */
void rc_heart_beat(rc_t *rc);

/* �������� */
void rc_ctrl(rc_t *rc);


/* �ж�״̬���� */
void rc_switch_status_interrupt_update(rc_base_info_t *info);
void remote_soft_reset_check(rc_t *rc);

#endif
