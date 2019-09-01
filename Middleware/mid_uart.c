#include "mid_uart.h"

/* uart1��ȡ���ݴ��� pdata������ָ�룬 len�����ݳ��� */
void uart1_read_deal(unsigned char *pdata, unsigned char len)
{
	unsigned char i;
	unsigned char tdata[100];
	
	if((pdata[0] == 0x66) && (pdata[1] == 0x06) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x060))
	{  //66 06 01 00 60��01Ϊ���ݳ��ȣ�00Ϊ���ݣ�˳�����
		for(i = 0; i < len; i++)
		{
			tdata[i] = pdata[i];
		}
		
		uart_write(UART1_Select, tdata, len);
	}
	else if((pdata[0] == 0x77) && (pdata[1] == 0x07) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x070))
	{  //77 07 01 00 70��01Ϊ���ݳ��ȣ�00Ϊ���ݣ��������
		for(i = 0; i < len; i++)
			tdata[i] = pdata[len - 1 - i];
		
		uart_write(UART1_Select, tdata, len);
	}
}
