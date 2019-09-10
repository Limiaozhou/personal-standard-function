#include "bsp_iic.h"

IIC_Hard_Master_WRInfo_TypeDef iic_wrinfo;
IIC_Hard_Master_ReadReg_Info_TypeDef iic_reginfo;

//内部函数
//__weak static void delay_us(uint16_t nus);  //延时n us，内部使用
//__weak static void delay_ms(uint16_t nms);  //延时n ms

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

//初始化输出时钟、本机作从机的地址、地址模式
void IIC_Hard_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, I2C_AddMode_TypeDef AddMode)
{
	I2C_DeInit();
	//配置输出时钟、本机作从机的地址、高速模式占空比、应答模式、地址模式、输入时钟
	I2C_Init(OutputClockFrequencyHz, OwnAddress, I2C_DUTYCYCLE_2, I2C_ACK_CURR, AddMode, CLK_GetClockFreq()/1000000);
	I2C_Cmd(ENABLE);
	I2C_ITConfig(I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);  //使能事件、收发、错误中断
}

//启动IIC读写
uint8_t IIC_Hard_Master_Write_Read(pIIC_Hard_Master_WRInfo_TypeDef piic, piic_hard_master_write_read pfun)
{
	iic_wrinfo = *piic;
	
	while(iic_wrinfo.error_resend_times--)
	{
		if(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))  //判断总线忙，
			continue;
		
		I2C_GenerateSTART(ENABLE);  //开始
		
		return 0;
	}
	
	return 1;
}

//写设备，结构体输入设备地址及其10位地址模式标志、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Hard_Master_Write(pIIC_Hard_Master_WRInfo_TypeDef piic)
{
	static uint8_t i = 0, j = 0;  //发、收数据数索引
	static int8_t write_read_flag = -1;  //发送接收状态缓存
	if(I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION))  //判断开始
	{
		if(iic_wrinfo.dev_adr_tenbit_flag)  //10位地址模式，先发送高位字节
			I2C_SendData((uint8_t)(iic_wrinfo.device_adr >> 8));
		else
			I2C_Send7bitAddress((uint8_t)(iic_wrinfo.device_adr & 0xFE), I2C_DIRECTION_TX);  //发低地址
	}
	if(I2C_GetFlagStatus(I2C_FLAG_HEADERSENT))  //判断10位地址高字节已发送
		I2C_Send7bitAddress((uint8_t)(iic_wrinfo.device_adr & 0xFE), I2C_DIRECTION_TX);  //发低地址
	if(I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED))  //判断地址发送完成
		write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER)  //判断发送接收状态，1为接收，0为发送
	if(write_read_flag == 0)  //判断为发送
	{
		if(I2C_GetFlagStatus(I2C_FLAG_TXEMPTY))  //判断发送数据寄存器为空，启动传输就触发
		{
			i++;
			if(i >= iic_wrinfo.len)  //判断发送完成
			{
				if(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))  //没写入新数据
				{
					i = 0;  //重新发送计数
					write_read_flag = -1;  //清发送接收标志
					I2C_GenerateSTOP(ENABLE);  //停止
				}
			}
			I2C_SendData(*(iic_wrinfo.data++));  //发送数据，写入寄存器就启动传输
		}
	}
	else if(write_read_flag == 1)  //判断为接收
	{
		// I2C_GenerateSTART(ENABLE);  //开始
		if(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY))  //判断接收数据寄存器非空，传输完触发
		{
			*(iic_wrinfo.data++) = I2C_ReceiveData();  //接收数据
			j++;
			if(j >= iic_wrinfo.len)
			{
				j = 0;  //重新接收计数
				write_read_flag = -1;  //清发送接收标志
				I2C_AcknowledgeConfig(I2C_ACK_CURR);  //当前字节返回应答，为下次传输准备
			}
			else if(j >= iic_wrinfo.len - 1)  //倒数第二个字节
			{
				I2C_AcknowledgeConfig(I2C_ACK_NONE);  //不返回应答
				I2C_GenerateSTOP(ENABLE);  //停止
			}
		}
	}
	if(I2C_GetFlagStatus(I2C_FLAG_BUSERROR))  //判断总线错误，接收错误起始或停止
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_BUSERROR);  //清0
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ACKNOWLEDGEFAILURE))  //判断应答失败，接收非应答
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_ACKNOWLEDGEFAILURE);  //清0
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ARBITRATIONLOSS))  //判断仲裁失败
	{
		I2C_ClearFlag(I2C_FLAG_ARBITRATIONLOSS);  //清0
	}
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
    
	while(piic->wr_info.error_resend_times--)
	{
		//先写寄存器地址
		IIC_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))  //设备低地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))  //设备地址 + 写信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		if(piic->reg_adr_twobyte_flag)  //从机2字节寄存器地址模式判断
		{
			if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr >> 8)))  //寄存器高地址
			{
				IIC_Master_Stop(piic->wr_info.port);
				continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
			}
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //寄存器低地址
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
		}
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //寄存器地址
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		
		if(piic->readreg_delay_flag == Delay_Front)
			delay_ms(piic->readreg_delay_nms);  //延时等待数据准备完成
		//再读数据
		IIC_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))  //设备高地址
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))  //设备低地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
        }
		if(piic->readreg_delay_flag == Delay_Back)
			delay_ms(piic->readreg_delay_nms);  //延时等待数据准备完成
		for(i = 0; i < piic->wr_info.len; i++)  //接收指定长度数据
		{
			if(i < piic->wr_info.len - 1)
				*(piic->wr_info.data + i) = IIC_Master_ReceiveByte(piic->wr_info.port, 0);  //数据没接收完，发送有效应答
			else
				*(piic->wr_info.data + i) = IIC_Master_ReceiveByte(piic->wr_info.port, 1);  //数据接收完，发送非应答
		}
		IIC_Master_Stop(piic->wr_info.port);
		
		return 0;  //若没有出错，停止后结束函数，返回0
	}
    
    return 1;  //发送3次都错误，返回1
}

//I2C中断处理
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
}
