/**
 ******************************************************************************
 * @file           : config_uart.c\h
 * @brief          :
 * @note           :
 ******************************************************************************
 */

#include "config_uart.h"
#include "string.h"
#include "remote.h"
int16_t channels[16];
uint8_t data_playload[25];
void USART1_rxDataHandler(uint8_t *rxBuf)
{
}


void USART3_rxDataHandler(uint8_t *rxBuf)
{

    channels[0] = ((rxBuf[1] | rxBuf[2] << 8) & 0x07FF);
    channels[1] = ((rxBuf[2] >> 3 | rxBuf[3] << 5) & 0x07FF);
    channels[2] = ((rxBuf[3] >> 6 | rxBuf[4] << 2 | rxBuf[5] << 10) & 0x07FF);
    channels[3] = ((rxBuf[5] >> 1 | rxBuf[6] << 7) & 0x07FF);
    channels[4] = ((rxBuf[6] >> 4 | rxBuf[7] << 4) & 0x07FF);
    channels[5] = ((rxBuf[7] >> 7 | rxBuf[8] << 1 | rxBuf[9] << 9) & 0x07FF);
    channels[6] = ((rxBuf[9] >> 2 | rxBuf[10] << 6) & 0x07FF);
    channels[7] = ((rxBuf[10] >> 5 | rxBuf[11] << 3) & 0x07FF);

    memcpy(data_playload, rxBuf, sizeof(data_playload));

    rc_base_info_update(&rc, rxBuf);
}



void USART6_rxDataHandler(uint8_t *rxBuf)
{
}

