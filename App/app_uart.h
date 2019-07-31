#ifndef __APP_UART_H
#define __APP_UART_H

#include "bsp_usart.h"

extern unsigned long int send_time;

void uart1_read_deal(unsigned char *pdata, unsigned char len);
void uart1_send_ontime_int(void);
void uart1_send_ontime(void);

#endif
