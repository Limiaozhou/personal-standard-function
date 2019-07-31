#include "app_uart.h"
#include "bsp_time.h"

static unsigned long int time_uart_send, time_uart_send1;
static unsigned char time_uartsend_num, time_uartsend_num1;

/* uart1读取数据处理， pdata：数据指针， len：数据长度 */
void uart1_read_deal(unsigned char *pdata, unsigned char len)
{
	unsigned char i;
	unsigned char tdata[20];
	
	if((pdata[0] == 0x66) && (pdata[1] == 0x06) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x060))
	{  //66 06 01 00 60，01为数据长度，00为数据，顺序输出
		for(i = 0; i < len; i++)
		{
			tdata[i] = pdata[i];
		}
		
		uart_write(UART1_Select, tdata, len);
	}
	else if((pdata[0] == 0x77) && (pdata[1] == 0x07) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x070))
	{  //77 07 01 00 70，01为数据长度，00为数据，倒序输出
		for(i = 0; i < len; i++)
			tdata[i] = pdata[len - 1 - i];
		
		uart_write(UART1_Select, tdata, len);
	}
}

//定时发送函数初始化，注册定时器
void uart1_send_ontime_int(void)
{
    time_uartsend_num = time_register(&time_uart_send);
    time_start(time_uartsend_num);
    time_uartsend_num1 = time_register(&time_uart_send1);
}

//定时发送函数
void uart1_send_ontime(void)
{
    unsigned char tx_data[5] = {0x11, 0x02, 0x0B, 0x01, 0xE1};  //读取数据命令
    unsigned char tx_data1[6] = {0x11, 0x03, 0x0C, 0x02, 0x1E, 0xC0};  //打开风扇命令
    static unsigned char count = 0;
    
    if(time_uart_send >= 1000)  //每秒读取数据一次
    {
        time_clean(time_uartsend_num);
        uart_write(UART1_Select, tx_data, 5);  //读取数据
        time_start(time_uartsend_num);
        if(++count >= 10)  //读取10次数据后，隔500ms打开风扇一次
        {
            count = 0;
            time_start(time_uartsend_num1);
        }
    }
    
    if(time_uart_send1 >= 500)
    {
        time_clean(time_uartsend_num1);
        uart_write(UART1_Select, tx_data1, 6);  //打开风扇
    }
}
