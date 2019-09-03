#include "bsp_iic_master.h"

//内部函数
//__weak static void delay_us(uint16_t nus);  //延时n us，内部使用
//__weak static void delay_ms(uint16_t nms);  //延时n ms

static void IIC_Master_Start(uint8_t port);  //起始信号
static void IIC_Master_Stop(uint8_t port);  //停止信号

static void IIC_Master_SendAck(uint8_t port, uint8_t ack);  //发送应答信号
static uint8_t IIC_Master_ReceiveAck(uint8_t port);  //接收应答信号

static uint8_t IIC_Master_SendByte(uint8_t port, uint8_t byte);  //发送一个字节
static uint8_t IIC_Master_ReceiveByte(uint8_t port, uint8_t ack);  //接收一个字节

//延时n us，内部使用
//__weak static void delay_us(uint16_t nus)
//{
//	while (nus--)
//	{
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//	}
//}
//
////延时n ms
//__weak static void delay_ms(uint16_t nms)
//{
//	while (nms--)
//		delay_us(1000);
//}

//初始化为空闲状态，scl与sda都为高，scl高电平收发稳定sda电平，低电平才能改sda电平
void IIC_Master_Init(uint8_t port)
{
    switch(port)
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
static void IIC_Master_Start(uint8_t port)
{
    switch(port)
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
static void IIC_Master_Stop(uint8_t port)
{
    switch(port)
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
static void IIC_Master_SendAck(uint8_t port, uint8_t ack)
{
    switch(port)
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
static uint8_t IIC_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
    switch(port)
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
static uint8_t IIC_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
    switch(port)
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

    return IIC_Master_ReceiveAck(port);
}

//接收一字节，并发送应答
static uint8_t IIC_Master_ReceiveByte(uint8_t port, uint8_t ack)
{
    uint8_t i, byte = 0;
    
    switch(port)
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
	
	IIC_Master_SendAck(port, ack);  //接收完一个字节，发送应答
	
    return byte;
}

//写设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Master_Write(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //发送数据字节数记录
	uint8_t error_flag = 0;  //发送错误标志
    
	while(piic->error_resend_times--)
	{
		IIC_Master_Start(piic->port);
        
        if(piic->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //设备低地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //设备地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		for(i = 0; i < piic->len; i++)
		{
			if(IIC_Master_SendByte(piic->port, *(piic->data++)))  //数据
			{
				IIC_Master_Stop(piic->port);
				error_flag = 1;  //置1发送错误标志
				break;  //发送出错就停止，跳当前for循环
			}
		}
		
		if(!error_flag)
		{
			IIC_Master_Stop(piic->port);
			return 0;  //若没有出错，停止后结束函数，返回0
		}
		
		error_flag = 0;  //清0发送错误标志
	}
    
    return 1;  //发送3次都错误，返回1
}

//读设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //接收数据字节数记录
    
	while(piic->error_resend_times--)
	{
		IIC_Master_Start(piic->port);
		
        if(piic->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //设备低地址 + 读信号，bit0=0为写，bit0=1为读，注意运算符优先级+高于&
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		for(i = 0; i < piic->len; i++)  //接收指定长度数据
		{
			if(i < piic->len - 1)
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 0);  //数据没接收完，发送有效应答
			else
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 1);  //数据接收完，发送非应答
		}
		
		IIC_Master_Stop(piic->port);
		
		return 0;  //若没有出错，停止后结束函数，返回0
	}
	
	return 1;  //发送3次都错误，返回1
}

//读设备寄存器，结构体输入端口、设备地址及其10位地址模式标志、寄存器地址和其长度、是否延时及延时时间、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic)
{
	uint8_t i;  //接收数据字节数记录
    
	while(piic->error_resend_times--)
	{
		//先写寄存器地址
		IIC_Master_Start(piic->port);
        if(piic->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //设备低地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //设备地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		if(piic->reg_adr_twobyte_flag)  //从机2字节寄存器地址模式判断
		{
			if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr >> 8)))  //寄存器高地址
			{
				IIC_Master_Stop(piic->port);
				continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
			}
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr & 0xFF)))  //寄存器低地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
		}
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr & 0xFF)))  //寄存器地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		
		//再读数据
		IIC_Master_Start(piic->port);
        if(piic->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //设备低地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		if(piic->readreg_delay_flag)
			delay_ms(piic->readreg_delay_nms);  //延时等待数据准备完成
		for(i = 0; i < piic->len; i++)  //接收指定长度数据
		{
			if(i < piic->len - 1)
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 0);  //数据没接收完，发送有效应答
			else
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 1);  //数据接收完，发送非应答
		}
		IIC_Master_Stop(piic->port);
		
		return 0;  //若没有出错，停止后结束函数，返回0
	}
    
    return 1;  //发送3次都错误，返回1
}
