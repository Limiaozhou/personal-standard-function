#include "bsp_iic_master.h"

//内部函数
static void delay_us(uint16_t nus);  //延时n us，内部使用

static void IIC_Master_Start(uint8_t type);  //起始信号
static void IIC_Master_Stop(uint8_t type);  //停止信号

static void IIC_Master_SendAck(uint8_t type, uint8_t ack);  //发送应答信号
static uint8_t IIC_Master_ReceiveAck(uint8_t type);  //接收应答信号

static uint8_t IIC_Master_SendByte(uint8_t type, uint8_t byte);  //发送一个字节
static uint8_t IIC_Master_ReceiveByte(uint8_t type);  //接收一个字节

//延时n us，内部使用
static void delay_us(uint16_t nus)
{
	while (nus--)
	{
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
}

//初始化为空闲状态，scl与sda都为高，scl高电平收发稳定sda电平，低电平才能改sda电平
void IIC_Master_Init(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_DIR_OUT();  //SDA输出
            IIC_MASTER_SCL_DIR_OUT();
            IIC_MASTER_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_MASTER_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_MASTER_SDA_IQ_OFF();  //SDA禁止输入中断，低速输出
            IIC_MASTER_SCL_IQ_OFF();  //SCL禁止输入中断，低速输出
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);  //SCL高电平，SDA上升，相当于发STOP信号
        break;
        
        case 2 :
            IIC_MASTER2_SDA_DIR_OUT();  //SDA输出
            IIC_MASTER2_SCL_DIR_OUT();
            IIC_MASTER2_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_MASTER2_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_MASTER2_SDA_IQ_OFF();  //SDA禁止输入中断，低速输出
            IIC_MASTER2_SCL_IQ_OFF();  //SCL禁止输入中断，低速输出
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);  //SCL高电平，SDA上升，相当于发STOP信号
        break;
        
        case 3 :
            IIC_MASTER3_SDA_DIR_OUT();  //SDA输出
            IIC_MASTER3_SCL_DIR_OUT();
            IIC_MASTER3_SDA_UP();  //SDA上拉输入，推挽输出
            IIC_MASTER3_SCL_UP();  //SCL上拉输入，推挽输出
            IIC_MASTER3_SDA_IQ_OFF();  //SDA禁止输入中断，低速输出
            IIC_MASTER3_SCL_IQ_OFF();  //SCL禁止输入中断，低速输出
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);  //SCL高电平，SDA上升，相当于发STOP信号
        break;
        
        default :
        break;
    }
}

//起始信号，当SCL处于高电平时，SDA由高电平变成低电平时
static void IIC_Master_Start(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//停止信号，当SCL处于高电平时，SDA由低电平变成高电平
static void IIC_Master_Stop(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(LOW);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(LOW);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(LOW);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//发送应答信号，0:ACK，1:NAK
static void IIC_Master_SendAck(uint8_t type, uint8_t ack)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(ack);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(ack);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(ack);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//接收应答信号，0:ACK，1:NAK
static uint8_t IIC_Master_ReceiveAck(uint8_t type)
{
    uint8_t ack = 1;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);

            IIC_MASTER_SDA_DIR_IN();

            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER_SDA_PIN_IN;
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);

            IIC_MASTER2_SDA_DIR_IN();

            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER2_SDA_PIN_IN;
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);

            IIC_MASTER3_SDA_DIR_IN();

            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER3_SDA_PIN_IN;
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return ack;
}

//发送一字节，返回应答信号
static uint8_t IIC_Master_SendByte(uint8_t type, uint8_t byte)
{
    uint8_t i, bit = 1;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));  //取逻辑值0或1，否则P0_0=bit时，只读取bit数据的最低位bit0的值
                IIC_MASTER_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 2 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER2_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 3 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER3_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        default :
        break;
    }

    return IIC_Master_ReceiveAck(type);
}

//接收一字节
static uint8_t IIC_Master_ReceiveByte(uint8_t type)
{
    uint8_t i, byte = 0;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);  //释放数据线，准备读取数据

            IIC_MASTER_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER_SDA_PIN_IN << (7 - i);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);  //释放数据线，准备读取数据

            IIC_MASTER2_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER2_SDA_PIN_IN << (7 - i);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);  //释放数据线，准备读取数据

            IIC_MASTER3_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER3_SDA_PIN_IN << (7 - i);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return byte;
}

//写数据到设备寄存器，成功返回0，失败返回1
uint8_t IIC_Master_Write(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len)
{
    uint8_t i;  //发送数据字节
	uint8_t resend_times = 3;  //失败重发次数
	uint8_t state;  //发送状态
    
	while(resend_times--)
	{
		IIC_Master_Start(type);

		if(IIC_Master_SendByte(type, device_adr + 0))  //设备地址 + 写信号，bit0=0为写，bit0=1为读
		{
			IIC_Master_Stop(type);
			return 1;
		}
		if(IIC_Master_SendByte(type, reg_adr))  //寄存器地址
		{
			IIC_Master_Stop(type);
			return 1;
		}
		for(i = 0; i < len; i++)
		{
			if(IIC_Master_SendByte(type, *(data++)))  //寄存器数据
			{
				IIC_Master_Stop(type);
				return 1;
			}
		}
		
		IIC_Master_Stop(type);
	}
    
    return 0;
}

//读设备指定长度数据，发读信号，发失败立即返回
void IIC_Master_Read(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len)
{
    uint8_t i;
    
    IIC_Master_Start(type);
    if(IIC_Master_SendByte(type, device_adr + 1))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
    {
        IIC_Master_Stop(type);
        return;
    }
    for(i = 0; i < len; i++)
    {
        *(data + i) = IIC_Master_ReceiveByte(type);
        if(i < len - 1)
            IIC_Master_SendAck(type, 0);  //接收一个字节，发送一个有效应答信号
    }
    IIC_Master_SendAck(type, 1);  //接收完数据，发送一个非应答信号

    IIC_Master_Stop(type);
}
