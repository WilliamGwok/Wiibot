/**
  ******************************************************************************
  * @file           : remote.c/h
  * @brief          : 
  * @note           : 更新时间：2022年1月11日17:24:36
	*                   鼠标速度滤波说明：
	*                   在更新中断对鼠标速度初始数据进行均值滤波
	*                   在控制任务中对鼠标速度进行低通滤波
  ******************************************************************************
  */

#ifndef __REMOTE_H
#define __REMOTE_H

#include "main.h"

#define REMOTE_OFFLINE_CNT_MAX  50          //遥控器失联计数上限
/*摇杆最大值*/
#define RC_CNT_MAX							(500.f)



#define RC_ONLINE	 (rc.info->status == DEV_ONLINE)


/* 遥控器拨杆旋钮状态枚举 */
typedef enum 
{
  keep_R,         //保持
  up_R,           //向上拨
  mid_R,          //向中拨
  down_R,         //向下拨
}remote_status_e;


/* 拨杆信息 */
typedef struct
{
  int16_t value_last;  //上一次值
  int16_t value;       //新值
  uint8_t status;      //状态
}remote_switch_info_t;


/* 遥控原始信息 */
typedef struct 
{
  /* 遥控器 */
  int16_t                 ch0;                  //右的左右
  int16_t                 ch1;                  //右的前后
  int16_t                 ch2;                  //左的左右
  int16_t                 ch3;                  //左的前后
  remote_switch_info_t    s1;                   //左拨杆
  remote_switch_info_t    s2;                   //右拨杆
}rc_base_info_t;

/* 遥控信息 */
typedef struct 
{
  int16_t             offline_cnt;  //失联计数
	uint8_t             status;       //状态
}rc_info_t;

/* 遥控 */
typedef struct
{
  rc_base_info_t     *base_info;
  rc_info_t          *info;
}rc_t;

/* 外部变量 */
extern rc_t rc;

/* 初始化 */
void RC_Init(rc_t *rc);
void rc_base_info_init(rc_base_info_t *info);
void rc_info_init(rc_info_t *info);
void rc_interrupt_init(rc_base_info_t *info);

/* 中断 */
void rc_base_info_update(rc_t *rc, uint8_t *rxBuf);

/* 心跳任务 */
void rc_heart_beat(rc_t *rc);

/* 控制任务 */
void rc_ctrl(rc_t *rc);


/* 中断状态更新 */
void rc_switch_status_interrupt_update(rc_base_info_t *info);
void remote_soft_reset_check(rc_t *rc);

#endif
