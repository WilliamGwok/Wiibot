#include "remote.h"
#include "string.h"
#include "stdbool.h"
#include "config.h"

bool rc_interrupt = false;                  //遥控器更新中断

rc_base_info_t rc_base_info;
rc_info_t rc_info;
rc_t rc = 
{
  .base_info = &rc_base_info,
  .info = &rc_info,
};

/**
  * @brief  遥控器初始化
  */
void RC_Init(rc_t *rc)
{
	/* 基本信息初始化 */
  rc_base_info_init(rc->base_info);
	/* 信息初始化 */
	rc_info_init(rc->info);
  /* 中断标志清除 */
  rc_interrupt_init(rc->base_info);
}

/**
  * @brief  基本信息初始化
  */
void rc_base_info_init(rc_base_info_t *info)
{
	/* 基本信息置零 */
  memset(info,0,sizeof(rc_base_info_t));
	/* 拨杆旋钮状态初始化 */
  info->s1.status = keep_R;
  info->s2.status = keep_R;
}


/**
  * @brief  信息初始化
  */
void rc_info_init(rc_info_t *info)
{
	/* 设置失联计数 */
  info->offline_cnt = REMOTE_OFFLINE_CNT_MAX;
  /* 设置状态 */
  info->status = DEV_OFFLINE;
}

/**
  * @brief  中断标志清除
  */
void rc_interrupt_init(rc_base_info_t *info)
{
  rc_interrupt = false;                  //遥控器更新中断
}



/**
  * @brief  遥控器滴答任务
  */
void rc_heart_beat(rc_t *rc)
{
  rc->info->offline_cnt ++;
  if(rc->info->offline_cnt > REMOTE_OFFLINE_CNT_MAX)
    RC_Init(rc);
	else 
		remote_soft_reset_check(rc);//软复位
  
}
/**
  * @brief  遥控信息更新
  */
void rc_base_info_update(rc_t *rc, uint8_t *rxBuf)
{
	rc_base_info_t *info = rc->base_info;
  info->ch0 = ((rxBuf[1]|rxBuf[2]<< 8) & 0x07FF);
  info->ch0 -= 1000;
  info->ch1 = ((rxBuf[2]>>3|rxBuf[3]<<5) & 0x07FF);
  info->ch1 -= 1000;
  info->ch2 = ((rxBuf[3]>>6|rxBuf[4]<<2|rxBuf[5]<<10) & 0x07FF);
  info->ch2 -= 1000;
  info->ch3 = ((rxBuf[5]>>1|rxBuf[6]<<7) & 0x07FF);
  info->ch3 -= 1000;
  info->s1.value = ((rxBuf[6]>>4|rxBuf[7]<<4) & 0x07FF)/500;
  info->s2.value = ((rxBuf[7]>>7|rxBuf[8]<<1|rxBuf[9]<<9) & 0x07FF)/500;
		
	/* 中断更新 */
	if(rxBuf[23]!=16)
	{
		rc_interrupt = true;
		rc->info->offline_cnt = 0;
		rc->info->status = DEV_ONLINE;
	}
}

/**
  * @brief  控制任务
  */
void rc_ctrl(rc_t *rc)
{
  static uint8_t last_rc_status = 0;
  if(rc_interrupt == true)
  {
    rc_interrupt = false;
    if(last_rc_status == rc->info->status)//遥控器状态没变
    {
      /* 遥控器拨杆状态更新 */
      rc_switch_status_interrupt_update(rc->base_info);
    }
    else//开控
    {
      rc->base_info->s1.value_last = rc->base_info->s1.value;
      rc->base_info->s2.value_last = rc->base_info->s2.value;
    }

  }

  /*储存上一次遥控器状态*/
  last_rc_status = rc->info->status;
}

/**
  * @brief  遥控器拨杆状态跳变判断并更新
  */
void rc_switch_status_interrupt_update(rc_base_info_t *info)
{
  /* 左拨杆判断 */
  if(info->s1.value != info->s1.value_last)
  {
    switch(info->s1.value)
    {
      case 1:
        info->s1.status = up_R;
        break;
      case 2:
        info->s1.status = mid_R;
        break;
      case 3:
        info->s1.status = down_R;
        break;
      default:
        break;
    }
    info->s1.value_last = info->s1.value;
  }
  else 
    info->s1.status = keep_R;
  
  /* 右拨杆判断 */
  if(info->s2.value != info->s2.value_last)
  {
    switch(info->s2.value)
    {
      case 1:
        info->s2.status = up_R;
        break;
      case 2:
        info->s2.status = mid_R;
        break;
      case 3:
        info->s2.status = down_R;
        break;
      default:
        break;
    }
    info->s2.value_last = info->s2.value;
  }
  else 
    info->s2.status = keep_R;
  
}



void remote_soft_reset_check(rc_t *rc)
{

    // if((rc->base_info->s1.value == 0x02)&&(rc->base_info->s2.value == 0x02))
    //   if(rc->base_info->thumbwheel.status == up_R || rc->base_info->thumbwheel.status == down_R)
    //     HAL_NVIC_SystemReset();

}
