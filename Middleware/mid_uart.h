#ifndef __MID_UART_H
#define __MID_UART_H

#include "bsp_usart.h"

#include "data.h"

void uart1_read_deal(uint8_t * pdata, uint32_t len);
void uart3_read_deal(uint8_t * pdata, uint32_t len);

#endif
