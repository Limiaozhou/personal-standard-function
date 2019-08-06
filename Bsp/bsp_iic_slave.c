#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

iic_slave_be_written  port1_be_written;  //IIC端口1被写函数
iic_slave_be_read     port1_be_read;  //IIC端口1被读函数

uint8_t start_flag = 1;  //接收起始信号标志，成功为0，即可开始接收地址、数据信号
uint8_t stop_receive_flag = 1;  //被写字节时，可同时接收停止信号标志，开为0
uint8_t sda_interrupt_flag = 1;  //sda引脚中断开关标志，开为0，防止SDA与SCL引脚同一端口时，SCL引起误判断

static uint8_t IIC_Slave_Start(uint8_t port);  //从机检测起始信号，成功放回0
static uint8_t IIC_Slave_Stop(uint8_t port);  //从机检测停止信号，成功放回0

static void IIC_Slave_SendAck(uint8_t port, uint8_t ack);  //从机发送应答信号，0:ACK，1:NAK
static uint8_t IIC_Slave_ReceiveAck(uint8_t port);  //从机接收应答信号，0:ACK，1:NAK

static int8_t IIC_Slave_SendByte(uint8_t port, uint8_t byte);  //从机发送一字节，发完一字节，在第9个脉冲返回应答信号，否则返回-1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte);  //从机接收一字节，接收完一个字节，在第9个脉冲时返回0，否则返回1

static int8_t IIC_Slave_Write(uint8_t port, uint8_t * data, uint16_t len);  //从机发送指定长度数据，发送一个字节中返回-1，发完一个字节返回0，发完结束所有数据或收到非应答信号返回1，并开启接收停止
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint16_t len);  //从机接收指定长度数据，读一个字节发送应答后，开启接收停止

static void IIC_Slave_SoP_Judge(uint8_t port);  //TTC从机接收判断启停信号

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
			
			sda_interrupt_flag = 0;  //sda引脚中断开关标志开
        break;
        
        default :
        break;
    }
}

//从机检测起始信号，SDA下降沿，SCL处于高电平，成功放回0。SDA关中断，SCL开上升沿中断，以接收数据
static uint8_t IIC_Slave_Start(uint8_t port)
{
    switch(port)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x02) && (!sda_interrupt_flag))  //SCL处于高电平，SDA下降沿
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA关中断
				// IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
                IIC_SLAVE_SCL_EXTI_UP();  //设置SCL上升沿触发，以在SCL高电平时检测SDA电平，接收数据信号
                IIC_SLAVE_SCL_IQ_ON();  //SCL开中断
				
				sda_interrupt_flag = 1;  //sda引脚中断开关标志关
				
                return 0;  //成功返回0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //失败返回1
}

//从机检测停止信号，SDA上升沿，SCL处于高电平，成功放回0。SCL关中断，SDA开下降沿中断，以检测开始信号
static uint8_t IIC_Slave_Stop(uint8_t port)
{
   switch(port)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x01) && (!sda_interrupt_flag))  //SCL处于高电平，SDA上升沿
           {
			   IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发，以接收起始信号
               // IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
               IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
               
			   sda_interrupt_flag = 0;  //sda引脚中断开关标志开
			   
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
           IIC_SLAVE_SDA_OUT(ack);  //引脚输出ack应答
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
           ack = IIC_SLAVE_SDA_PIN_IN;  //接收引脚ack应答
       break;
       
       default :
       break;
   }
   
   return ack;
}

//从机发送一字节，发完一字节，再第9个脉冲返回应答信号，否则返回-1
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
				IIC_SLAVE_SDA_OUT(bit);  //引脚输出bit
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

//从机接收一字节，接收完一个字节，在第9个脉冲时返回0，否则返回1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //数据bit位数记录
			
			if(!i)
				*byte = 0;  //开始清0，之后再赋值
			
            if(i++ < 8)  //前8个脉冲接收数据各bit位
                *byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
			else  //第9个脉冲返回0，否则返回1
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

//从机发送指定长度数据，发送一个字节中返回-1，发完一个字节返回0，发完结束所有数据或收到非应答信号返回1，并开启接收停止
static int8_t IIC_Slave_Write(uint8_t port, uint8_t * data, uint16_t len)
{
	static uint8_t i = 0;  //数据字节数记录
	int8_t state = -1;  //发送字节函数返回状态缓存
	
	state = IIC_Slave_SendByte(port, *(data + i));  //发送字节bit位，并返回发送状态
	
	if(state != (-1))  //发送完一字节，在第9个脉冲不是返回-1
	{
		if(state == 0)  //接收为ACK应答信号
			i++;
		else if(state == 1)  //接收为NACK非应答信号
			i = 0;
		
		if((i >= len) || (state == 1))  //发送字节数达到，或接收到非应答信号，停止发送，开始接收停止信号
		{
			IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
			IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
			IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
			
			sda_interrupt_flag = 0;  //sda引脚中断开关标志开
			start_flag = 1;  //停止接收地址、数据信号
			i = 0;
		}
	}
	
	return state;
}

//从机接收指定长度数据，读一个字节发送应答后，开启接收停止，接收完返回0，否则返回1
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint16_t len)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //数据字节数记录
			static uint8_t byte = 0;  //数据缓存
			
			if(!IIC_Slave_ReceiveByte(port, &byte))  //接收完一个字节，第9个脉冲返回0
			{
				*(data + i) = byte;  //接收完一个字节再赋值
				
				if(i < len - 1)
				{
					IIC_Slave_SendAck(port, 0);  //接收完一个字节，发送一个有效应答信号
					
					IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
					IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
					
					sda_interrupt_flag = 0;  //sda引脚中断开关标志开
					stop_receive_flag = 0;  //被写字节时，可同时接收停止信号标志开
					i++;
				}
				else    //接收字节数达到，停止接收数据，开始只接收停止信号
				{
					IIC_Slave_SendAck(port, 1);  //接收完所有数据，发送一个非应答信号
					
					IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
					IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
					// IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
					IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
					
					sda_interrupt_flag = 0;  //sda引脚中断开关标志开
					start_flag = 1;  //停止接收地址、数据信号
					i = 0;
					
					return 0;
				}
			}
        break;
        
        default :
        break;
    }
	
	return 1;
}

//TTC从机接收判断启停信号
static void IIC_Slave_SoP_Judge(uint8_t port)
{
	if(!IIC_Slave_Start(port))  //接收起始成功，为0
		start_flag = 0;  //开启接收数据
	
	if(!IIC_Slave_Stop(port))  //接收停止成功，为0
	{
		start_flag = 1;  //关闭接收数据
	}
	else if(!stop_receive_flag)  //接收停止失败，且为被写字节时接收停止状态下
	{
		IIC_SLAVE_SDA_IQ_OFF();  //SDA关中断
		// IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
		IIC_SLAVE_SCL_EXTI_UP();  //设置SCL上升沿触发，以在SCL高电平时检测SDA电平，接收数据信号
		
		sda_interrupt_flag = 1;  //sda引脚中断开关标志关
		stop_receive_flag = 1;  //被写字节时，可同时接收停止信号标志关
	}
		
}

//TTC从机接收判断设备7位地址，地址正确，且为写返回0，为读返回1；错误返回2；地址接收未完成返回-1
static int8_t IIC_Slave_Address_Judge(uint8_t port)
{
	uint8_t address = 0;  //读取地址缓存
	static uint8_t byte = 0;  //数据缓存
	int8_t state = -1;  //状态缓存
	
	if(!IIC_Slave_ReceiveByte(port, &byte))  //接收完一个字节，第9个脉冲返回0
	{
		address = byte;  //接收完一个字节再赋值
		
		if((address & (~0x01)) == (IIC_DEVICE_ADDRESS & (~0x01)))  //判断7位地址
		{
			IIC_Slave_SendAck(port, 0);  //地址正确发应答信号
			
			if(!(address & 0x01))  //bit0为0，为写
				state = 0;
			else  //bit0为1，为读
				state = 1;
		}
		else  //地址错误
			state = 2;
	}
	else
		state = -1;  //接收地址数据中，返回-1
	
	if(state == 2)  //地址错误，开始接收停止信号
	{
		IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
		IIC_SLAVE_SDA_EXTI_UP();  //SDA上升沿触发，以接收停止信号
		// IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
		IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
		
		sda_interrupt_flag = 0;  //sda引脚中断开关标志开
		start_flag = 1;  //停止接收地址、数据信号
	}
	
	return state;
}

//IIC从机处理数据信号
static void IIC_Slave_Data_Deal(uint8_t port)
{
	if(!start_flag)  //接收起始成功
	{
		if()
	}
}

//STM8 PB口接收中断函数，SDA、SCL引脚
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
}
