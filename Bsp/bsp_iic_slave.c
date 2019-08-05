#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

iic_slave_be_written  port1_be_written;  //IIC端口1被写函数
iic_slave_be_read     port1_be_read;  //IIC端口1被读函数

uint8_t start_flag = 1;  //接收起始信号标志，成功清0
uint8_t stop_flag = 1;  //接收停止信号标志，成功清0

static uint8_t IIC_Slave_Start(uint8_t port);  //从机检测起始信号
static uint8_t IIC_Slave_Stop(uint8_t port);  //从机检测停止信号
static void IIC_Slave_SendAck(uint8_t port, uint8_t ack);  //从机发送应答信号
static uint8_t IIC_Slave_ReceiveAck(uint8_t port);  //从机接收应答信号


//初始化，SCL关中断，SDA开下降沿中断，以检测开始信号
void IIC_Slave_Init(uint8_t port, iic_slave_be_written be_written_cb, iic_slave_be_read be_read_cb)
{
    switch(port)
    {
        case 1 :
            port1_be_written = be_written_cb;  //将被写回调函数，赋给IIC端口1被写函数
			port1_be_read = be_read_cb;  //将被读回调函数，赋给IIC端口1被读函数
			
			IIC_SLAVE_SDA_DIR_IN();  //SDA输入
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_SLAVE_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发，以接收起始信号
            // IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发，与SDA为同一端口，与SDA之间选一配置
            IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
            IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
        break;
        
        default :
        break;
    }
}

//从机检测起始信号，SDA下降沿，SCL处于高电平，成功放回0
static uint8_t IIC_Slave_Start(uint8_t port)
{
    switch(port)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x02))  //SCL处于高电平，SDA下降沿
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA关中断
				// IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
                IIC_SLAVE_SCL_EXTI_UP();  //设置SCL上升沿触发，以在SCL高电平时检测SDA电平，接收数据信号
                IIC_SLAVE_SCL_IQ_ON();  //SCL开中断
                return 0;  //成功返回0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //失败返回1
}

//从机检测停止信号，SDA上升沿，SCL处于高电平，成功放回0
static uint8_t IIC_Slave_Stop(uint8_t port)
{
   switch(port)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x01))  //SCL处于高电平，SDA上升沿
           {
               IIC_SLAVE_SDA_DIR_IN();  //SDA输入
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发，以接收起始信号
               // IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
               IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
               IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
               return 0;  //成功返回0
           }
       break;
       
       default :
       break;
   }
   
   return 1;  //失败返回1
}

//从机发送应答信号，0:ACK，1:NAK
static void IIC_Slave_SendAck(uint8_t port, uint8_t ack)
{
   switch(port)
   {
       case 1 :
		   IIC_SLAVE_SDA_DIR_OUT();
           IIC_SLAVE_SDA_OUT(ack);
           IIC_SLAVE_SDA_DIR_IN();
       break;
       
       default :
       break;
   }
}

//从机接收应答信号，0:ACK，1:NAK
static uint8_t IIC_Slave_ReceiveAck(uint8_t port)
{
   uint8_t ack = 1;
   
   switch(port)
   {
       case 1 :
           ack = IIC_SLAVE_SDA_PIN_IN;
       break;
       
       default :
       break;
   }
   
   return ack;
}

//从机发送数据信号，发完一字节返回应答信号，否则返回-1
static int8_t IIC_Slave_SendByte(uint8_t port, uint8_t byte)
{
	uint8_t bit = 1;
	
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //数据bit位数记录
			
			IIC_SLAVE_SDA_DIR_OUT();
			
			if(i++ < 8)  //前8个脉冲发送数据各bit位
			{
				bit = !!(byte & (0x80 >> i));  //取逻辑值0或1，否则P0_0=bit时，只读取bit数据的最低位bit0的值
				IIC_SLAVE_SDA_OUT(bit);
			}
			else  //第9个脉冲接收应答，并返回应答信号
			{
				IIC_SLAVE_SDA_DIR_IN();
				i = 0;
				return IIC_Slave_ReceiveAck(port);
			}
        break;
        
        default :
        break;
    }
	
	return -1;
}

//从机接收一字节，接收完一个字节，再接收第9个脉冲时返回0，否则返回1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //数据bit位数记录
			
			if(!i)
				*byte = 0;  //开始清0，之后再赋值
			
            if(i++ < 8)
                *byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
			else
			{
				i = 0;
				return 0;
			}
        break;
        
        default :
        break;
    }

    return 1;
}

//从机发送指定长度数据


//从机接收指定长度数据
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint8_t len)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //数据字节数记录
			static uint8_t byte = 0;  //数据缓存
			
			if(!IIC_Slave_ReceiveByte(port, &byte))
			{
				*(data + i) = byte;  //接收完一个字节再赋值
				if(i < len - 1)
				{
					IIC_Slave_SendAck(port, 0);  //接收完一个字节，发送一个有效应答信号
					i++;
				}
				else
				{
					IIC_Slave_SendAck(port, 1);  //接收完所有数据，发送一个非应答信号
					i = 0;
				}
			}
        break;
        
        default :
        break;
    }
}

//TTC从机接收启停信号
void IIC_Slave_Start_Judge(uint8_t port)
{
	if(start_flag)  //接收起始信号后自锁，不再接收，直到满足重新接收条件
		start_flag = IIC_Slave_Start(port);  //接收起始成功，为0
	
	if(!IIC_Slave_Stop(port))  //接收停止信号成功，为0
		start_flag = 1;
}

//IIC从机处理数据信号
void IIC_Slave_Data_Deal(uint8_t port)
{
	
	
	if(!start_flag)  
	{
		
	}
}

//STM8 PB口接收中断函数，SDA、SCL引脚
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
}
