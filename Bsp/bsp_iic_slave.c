#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

//初始化，SCL关中断，SDA开下降沿中断，以检测开始信号
void IIC_Slave_Init(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_SLAVE_SDA_DIR_IN();  //SDA输入
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_SLAVE_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发，以接收起始信号
            IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
            IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出，以接收起始信号
            IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
        break;
        
        default :
        break;
    }
}

//从机检测起始信号，SDA下降沿，SCL处于高电平，成功放回0
static uint8_t IIC_Slave_Start(uint8_t type)
{
    switch(type)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN)  //SCL处于高电平，成功
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA关中断
                IIC_SLAVE_SCL_EXTI_UP();  //设置SCL上升沿触发，以在SCL高电平时检测SDA电平，接收数据信号
                IIC_SLAVE_SCL_IQ_ON();  //SCL开中断
                return 0;  //返回0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //失败返回1
}

//从机检测停止信号，SDA上升沿，SCL处于高电平，成功放回0
static uint8_t IIC_Slave_Stop(uint8_t type)
{
   switch(type)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN)  //SCL处于高电平，成功
           {
               IIC_SLAVE_SDA_DIR_IN();  //SDA输入
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发，以接收起始信号
               IIC_SLAVE_SCL_EXTI_UP();  //SCL上升沿触发
               IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出，以接收起始信号
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
static void IIC_Slave_SendAck(uint8_t type, uint8_t ack)
{
   switch(type)
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
static uint8_t IIC_Slave_ReceiveAck(uint8_t type)
{
   uint8_t ack = 1;
   
   switch(type)
   {
       case 1 :
           ack = IIC_SLAVE_SDA_PIN_IN;
       break;
       
       default :
       break;
   }
   
   return ack;
}

//从机发送数据信号
static void IIC_Slave_SendBit(uint8_t type, uint8_t bit)
{
    switch(type)
    {
        case 1 :
			IIC_SLAVE_SDA_DIR_OUT();
			IIC_SLAVE_SDA_OUT(bit);
			IIC_SLAVE_SDA_DIR_IN();
        break;
        
        default :
        break;
    }

    return byte;
}

//从机接收一字节
static uint8_t IIC_Slave_ReceiveByte(uint8_t type)
{
    uint8_t byte = 0;  //数据字节缓存
    
    switch(type)
    {
        case 1 :
			static uint8_t i = 0;  //数据bit位数记录
			
            if(i++ >= 7)  //接收7个bit后返回一个字节
            {
                byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
				i = 0;
            }
        break;
        
        default :
        break;
    }

    return byte;
}

//从机接收指定长度数据
static uint8_t IIC_Slave_Read(uint8_t type, uint8_t * data, uint8_t len)
{
    uint8_t i;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < len; i++)
			{
				*(data + i) = IIC_Slave_ReceiveByte(type);
				if(i < len - 1)
					IIC_Slave_SendAck(type, 0);  //接收一个字节，发送一个有效应答信号
			}
			IIC_Slave_SendAck(type, 1);  //接收完数据，发送一个非应答信号
        break;
        
        default :
        break;
    }

    return byte;
}

//
void IIC_Slave_Deal(uint8_t type)
{
    static uint8_t state = 0;
    
    
    if((!state) && (!IIC_Slave_Start(type)))
        state = 1;  //收到起始信号，并自锁
    
    if(state)
    {
        if(IIC_Slave_ReceiveByte(type) == SlaveAddress)
            ;
    }
}

//STM8 PB口接收中断函数，SDA、SCL引脚
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
    
}
