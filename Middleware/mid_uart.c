#include "mid_uart.h"

/* uart1读取数据处理， pdata：数据指针， len：数据长度 */
void uart1_read_deal(uint8_t * pdata, uint32_t len)
{
	uint32_t i;
	uint8_t tdata[200];
	
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

/* uart3读取数据处理， pdata：数据指针， len：数据长度 */
void uart3_read_deal(uint8_t * pdata, uint32_t len)
{
    uint32_t i = 0;  //数组索引
    uint8_t head_buf[2] = {0x42, 0x4D};
    
    while((len--) >= 32)
    {
        if((pdata[0+i] == head_buf[0]) && (pdata[1+i] == head_buf[1]))
        {
            Evndat.pm25 = ((uint16_t)pdata[12+i] << 8) + pdata[13+i];
            break;
        }
        i++;
    }
}
