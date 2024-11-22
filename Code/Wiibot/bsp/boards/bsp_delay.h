#ifndef BSP_DELAY_H
#define BSP_DELAY_H
#include "struct_typedef.h"

extern void delay_init(void);
extern void delay_us(uint32_t us);
extern void delay_ms(uint32_t ms);

uint32_t micros(void);
#endif

