#ifndef __BSP_IIC_SIMULATION_MASTER_H
#define __BSP_IIC_SIMULATION_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm8s_gpio.h"
#include "delay.h"

//#include "typedef.h"

//数据类型声明
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8位机int为16,32位机int为32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

#define IIC_SIMULATION_MASTER_NUM 1  //模拟IIC口数量
#define DELAY_US_IIC  5  //IIC输出延时us

typedef enum
{
	GPIO_Low_Level = 0,  //IO低电平
	GPIO_High_Level
}GPIO_OutLevel_TypeDef;  //IO电平

typedef enum
{
	GPIO_In = 0,  //IO输入
	GPIO_Out
}GPIO_Direction_TypeDef;  //IO电平

typedef struct
{
    GPIO_TypeDef* scl_port;
    GPIO_Pin_TypeDef scl_pin;
	GPIO_TypeDef* sda_port;
    GPIO_Pin_TypeDef sda_pin;
}IIC_Simulation_GPIO_TypeDef;  //模拟IIC端口信息

typedef struct
{
	uint8_t port;  //IIC端口
	uint16_t device_adr;  //IIC从机设备地址
    uint8_t dev_adr_tenbit_flag;  //IIC从机设备10位地址模式标志位
	uint8_t * data;    //IIC读写数据指针
	uint32_t len;  //IIC读写数据长度
    uint8_t error_resend_times;  //失败重发次数
}IIC_Master_WRInfo_TypeDef;  //IIC主机读写信息

typedef IIC_Master_WRInfo_TypeDef* pIIC_Master_WRInfo_TypeDef;  //IIC主机读写信息指针

typedef enum
{
	Delay_No = 0,  //不延时
	Delay_Front,  //读之前延时
	Delay_Back  //读之后延时
}Delay_ReadReg_TypeDef;  //读寄存器延时

typedef struct
{
	IIC_Master_WRInfo_TypeDef wr_info;  //IIC主机读写信息
	uint16_t register_adr;  //IIC从机寄存器地址
	uint8_t reg_adr_twobyte_flag;  //IIC从机2字节寄存器地址标志位
	Delay_ReadReg_TypeDef readreg_delay_flag;  //IIC读寄存器发送读信号前后延时标志
	uint32_t readreg_delay_nms;  //IIC读寄存器发送读信号前后延时时间 ms
}IIC_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息

typedef IIC_Master_ReadReg_Info_TypeDef* pIIC_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息指针

void IIC_Simulation_Master_Init(void);  //模拟IIC初始化

uint8_t IIC_Simulation_Master_Write(pIIC_Master_WRInfo_TypeDef piic);  //写设备，成功返回0，失败返回1
uint8_t IIC_Simulation_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic);  //读设备，成功返回0，失败返回1

uint8_t IIC_Simulation_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic);  //读设备寄存器，成功返回0，失败返回1

#ifdef __cplusplus
}
#endif

#endif