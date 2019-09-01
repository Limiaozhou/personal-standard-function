#include "mid_uart.h"

/* uart1��ȡ���ݴ��� pdata������ָ�룬 len�����ݳ��� */
void uart1_read_deal(uint8_t * pdata, uint32_t len)
{
	uint32_t i;
	uint8_t tdata[500];
	
	if((pdata[0] == 'f') && (pdata[1] == 'r'))
	{
		for(i = 0; i < len; i++)
			tdata[i] = pdata[i];
		
		uart_write(UART1_Select, tdata, len);
	}
	else if((pdata[0] == 'b') && (pdata[1] == 'a'))
	{
		for(i = 0; i < len; i++)
			tdata[i] = pdata[len - 1 - i];
		
		uart_write(UART1_Select, tdata, len);
	}
}
