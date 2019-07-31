#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

//初始化，SCL关中断，SDA开下降沿中断，以检测开始信号
void IIC_Slave_Init(unsigned char type)
{
    switch(type)
    {
        case 1 :
            IIC_SLAVE_SDA_DIR_IN();  //SDA输入
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_SLAVE_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA下降沿触发
            IIC_SLAVE_SCL_EXTI_DOWN();  //SCL下降沿触发
            IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
            IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
        break;
        
        default :
        break;
    }
}

//从机检测起始信号，SDA下降沿，SCL处于高电平时
static unsigned char IIC_Slave_Start(unsigned char type)
{
    switch(type)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN)  //SCL处于高电平，成功
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA关中断
                IIC_SLAVE_SCL_EXTI_UP();  //设置SCL上升沿触发
                IIC_SLAVE_SCL_IQ_ON();  //SCL开中断
                return 0;  //返回0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //失败返回1
}

//从机检测停止信号，当SCL处于高电平时，SDA由低电平变成高电平
//static unsigned char IIC_Slave_Stop(unsigned char type)
//{
//    switch(type)
//    {
//        case 1 :
//            if(IIC_SLAVE_SDA_PIN_IN && IIC_SLAVE_SCL_PIN_IN)
//            {
//                IIC_SLAVE_SDA_DIR_IN();  //SDA输入
//                IIC_SLAVE_SDA_EXTI_UPDOWN();  //SDA上升沿和下降沿触发
//                IIC_SLAVE_SCL_EXTI_UPDOWN();  //SCL上升沿和下降沿触发
//                IIC_SLAVE_SDA_IQ_ON();  //SDA中断输入开，高速输出
//                IIC_SLAVE_SCL_IQ_OFF();  //SCL中断输入关，低速输出
//                return 0;  //成功返回0
//            }
//        break;
//        
//        default :
//        break;
//    }
//    
//    return 1;  //失败返回1
//}

//从机发送应答信号，0:ACK，1:NAK
//static void IIC_Slave_SendAck(unsigned char type, unsigned char ack)
//{
//    switch(type)
//    {
//        case 1 :
//            IIC_SLAVE_SDA_DIR_OUT();
//            IIC_SLAVE_SDA_OUT(ack);
//            IIC_SLAVE_SDA_DIR_IN();
//        break;
//        
//        default :
//        break;
//    }
//}

//从机接收应答信号，0:ACK，1:NAK
//static unsigned char IIC_Slave_ReceiveAck(unsigned char type)
//{
//    unsigned char ack = 1;
//    
//    switch(type)
//    {
//        case 1 :
//            ack = IIC_SLAVE_SDA_PIN_IN;
//        break;
//        
//        default :
//        break;
//    }
//    
//    return ack;
//}

//从机接收一字节
static unsigned char IIC_Slave_ReceiveByte(unsigned char type)
{
    unsigned char i, byte = 0;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < 8; i++)
            {
                byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
            }
        break;
        
        default :
        break;
    }

    return byte;
}

//
void IIC_Slave_Deal(unsigned char type)
{
    static unsigned char state = 0;
    
    
    if((!state) && (!IIC_Slave_Start(type)))
        state = 1;  //收到起始信号，并自锁
    
    if(state)
    {
        if(IIC_Slave_ReceiveByte(type) == SlaveAddress)
            ;
    }
}

//STM8 PB口接收中断函数
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
    
}
