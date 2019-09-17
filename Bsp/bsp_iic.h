#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm8s_i2c.h"
#include "stm8s_clk.h"
#include "delay.h"

//#include "typedef.h"

//数据类型声明
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8位机int为16,32位机int为32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

typedef struct
{
	uint16_t device_adr;  //IIC从机设备地址
    uint8_t dev_adr_tenbit_flag;  //IIC从机设备10位地址模式标志位
	uint8_t * data;    //IIC读写数据指针
	uint32_t len;  //IIC读写数据长度
    uint8_t error_resend_times;  //失败重发次数
}IIC_Hard_Master_WRInfo_TypeDef;  //IIC主机读写信息

typedef IIC_Hard_Master_WRInfo_TypeDef* pIIC_Hard_Master_WRInfo_TypeDef;  //IIC主机读写信息指针

typedef enum
{
	Delay_Hard_No = 0,  //不延时
	Delay_Hard_Front,  //读之前延时
	Delay_Hard_Back  //读之后延时
}Delay_Hard_ReadReg_TypeDef;  //读寄存器延时

typedef struct
{
	IIC_Hard_Master_WRInfo_TypeDef wr_info;  //IIC主机读写信息
	uint16_t register_adr;  //IIC从机寄存器地址
	uint8_t reg_adr_twobyte_flag;  //IIC从机2字节寄存器地址标志位
	Delay_Hard_ReadReg_TypeDef readreg_delay_flag;  //IIC读寄存器发送读信号前后延时标志
	uint32_t readreg_delay_nms;  //IIC读寄存器发送读信号前后延时时间 ms
}IIC_Hard_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息

typedef IIC_Hard_Master_ReadReg_Info_TypeDef* pIIC_Hard_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息指针

typedef enum
{
	Write_Sel = 0,  //写
	ReadDirect_Sel,  //直接读
}IIC_WriteRead_Select_TypeDef;

typedef void (*piic_hard_read_data_deal)(uint8_t * buf, uint16_t len);  //硬件IIC口数据读取处理函数

//硬件IIC初始化，配置输出时钟、本机作从机的地址、地址模式
void IIC_Hard_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, I2C_AddMode_TypeDef AddMode, piic_hard_read_data_deal pdeal);

//硬件IIC读写启动，输入内容指针、读写选择，成功返回0，失败（总线忙）返回1
uint8_t IIC_Hard_Master_WriteRead_Start(pIIC_Hard_Master_WRInfo_TypeDef piic, IIC_WriteRead_Select_TypeDef sel);

#ifdef __cplusplus
}
#endif

#endif