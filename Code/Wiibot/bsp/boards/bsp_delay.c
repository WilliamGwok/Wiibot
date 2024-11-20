#include "bsp_delay.h"
#include "main.h"
#include "drv_haltick.h"
static uint8_t fac_us = 0;
static uint32_t fac_ms = 0;

void delay_init(void)
{
    fac_us = SystemCoreClock / 1000000;
    fac_ms = SystemCoreClock / 1000;

}

uint32_t haltick = 0;

uint32_t micros(void)
{
	register uint32_t ms, us;

	ms = HAL_GetTick();
	// 选用定时器2作为HAL时基的TimeBase
	// Freq:1MHz => 1Tick = 1us
	// Period:1ms
	us = TIM2->CNT;
	
    haltick = ms*1000 + us;
    
	return haltick;
}

void delay_us(uint32_t us)
{
  uint32_t now = micros();
	
	while((micros() - now) < us);
}

void delay_ms(uint32_t ms)
{
    while(ms--)
		delay_us(1000);
}
