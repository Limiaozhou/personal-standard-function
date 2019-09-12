#include "bsp_iic.h"

static uint8_t write_index = 0, read_index = 0;  //发、收数据数索引
static int8_t write_read_flag = -1;  //发送接收状态缓存
static uint8_t addr10_read_flag = 0;  //10位地址读模式标志

static pIIC_Hard_Master_WRInfo_TypeDef piic_wrinfo = NULL;
static pIIC_Hard_Master_ReadReg_Info_TypeDef piic_reginfo = NULL;

static IIC_WriteRead_Select_TypeDef iic_wr_sel = Write_Sel;

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

//硬件IIC读写启动，成功返回0，失败（总线忙）返回1
uint8_t IIC_Hard_Master_WriteRead_Start(pIIC_Hard_Master_WRInfo_TypeDef piic, IIC_WriteRead_Select_TypeDef sel)
{
	uint8_t resent_times = 1;
	
	piic_wrinfo = piic;
	iic_wr_sel = sel;
	resent_times = piic_wrinfo->error_resend_times;
	
	while(resent_times--)
	{
		if(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))  //判断总线忙
		{
			delay_us(1);
			continue;
		}
		
		I2C_GenerateSTART(ENABLE);  //开始
		
		return 0;
	}
	
	return 1;
}

//硬件IIC读写
void IIC_Hard_Master_WriteRead(pIIC_Hard_Master_WRInfo_TypeDef piic)
{
	if(I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION))  //判断开始
	{
		if(piic->dev_adr_tenbit_flag)  //判断10位地址模式
		{
			if(iic_wr_sel == ReadDirect_Sel)  //判断读模式
			{
				if(!addr10_read_flag)
					I2C_SendData((uint8_t)(piic->.device_adr >> 8));  //发送高位头字节
				else
					I2C_Send7bitAddress((uint8_t)(piic->device_adr >> 8), I2C_DIRECTION_TX);  //头字节读
			}
			else
				I2C_SendData((uint8_t)(piic->.device_adr >> 8));
		}
		else
		{
			if(iic_wr_sel == ReadDirect_Sel)
				I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_RX);  //发低地址
			else
				I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_TX);
		}
	}
	if(I2C_GetFlagStatus(I2C_FLAG_HEADERSENT))  //判断10位地址高字节已发送
		I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_TX);
	if(I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED))  //判断地址发送完成
	{
		if(piic->dev_adr_tenbit_flag)
		{
			if(iic_wr_sel == ReadDirect_Sel)
			{
				if(!addr10_read_flag)
				{
					I2C_GenerateSTART(ENABLE);
					addr10_read_flag = 1;
				}
				else
				{
					write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);
					addr10_read_flag = 0;
				}
			}
			else
				write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);
		}
		else
			write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);  //发送接收状态，1为接收，0为发送
	}
	if(write_read_flag == 0)  //判断为发送
	{
		if(I2C_GetFlagStatus(I2C_FLAG_TXEMPTY))  //判断发送数据寄存器为空，启动传输就触发
		{
			write_index++;
			if(write_index >= piic->len)  //判断发送完成
			{
				if(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))  //没写入新数据
				{
					write_index = 0;  //重新发送计数
					write_read_flag = -1;  //清发送接收标志
					I2C_GenerateSTOP(ENABLE);  //停止
				}
			}
			I2C_SendData(*(piic->data++));  //发送数据，写入寄存器就启动传输
		}
	}
	else if(write_read_flag == 1)  //判断为接收
	{
		if(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY))  //判断接收数据寄存器非空，传输完触发
		{
			read_index++;
			if(read_index >= piic->len)
			{
				read_index = 0;  //重新接收计数
				write_read_flag = -1;  //清发送接收标志
				I2C_AcknowledgeConfig(I2C_ACK_CURR);  //当前字节返回应答，为下次传输准备
			}
			else if(read_index >= piic->len - 1)  //倒数第二个字节
			{
				I2C_AcknowledgeConfig(I2C_ACK_NONE);  //不返回应答
				I2C_GenerateSTOP(ENABLE);  //停止
			}
			*(piic->data++) = I2C_ReceiveData();  //接收数据
		}
	}
}

//错误判断处理
void IIC_Hard_Error_Deal(void)
{
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

//I2C中断处理
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
	if((piic_wrinfo != NULL) && (iic_wr_sel != 0))
	{
		
	}
	
	IIC_Hard_Error_Deal();
}
