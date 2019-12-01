#ifndef __BSP_I2C_SIMULATION_MASTER_H
#define __BSP_I2C_SIMULATION_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"
#include "delay.h"

#elif defined STM8
#include "stm8s_gpio.h"
#include "delay.h"
#endif

//#include "typedef.h"

//数据类型声明
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8位机int为16,32位机int为32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

#define DELAY_US_I2C  5  //I2C协议电平变换延时us

typedef enum
{
	I2C_SimPort0 = 0,
    number_of_i2c_simulation
}I2C_Simulaton_Port;  //模拟I2C端口

//模拟I2C端口列表
#if defined (STM32_STANDARD) || defined (STM8)
#if defined STM32_STANDARD
#define I2C_GPIO_LIST {\
	{\
		.GPIO_scl = GPIOA,\
		.Pin_scl = GPIO_Pin_4,\
		.GPIO_sda = GPIOA,\
		.Pin_sda = GPIO_Pin_5\
	},\
}

typedef struct
{
    GPIO_TypeDef* GPIO_scl;
	GPIO_TypeDef* GPIO_sda;
    uint32_t RCC_APB2Periph_scl;
    uint32_t RCC_APB2Periph_sda;
    uint16_t Pin_scl;
    uint16_t Pin_sda;
}I2C_Simulation_GPIO_TypeDef;  //模拟I2C端口信息

#elif defined STM8
#define I2C_GPIO_LIST {\
	{\
		.GPIO_scl = GPIOE,\
		.Pin_scl = GPIO_PIN_6,\
		.GPIO_sda = GPIOE,\
		.Pin_sda = GPIO_PIN_7\
	},\
	{\
		.GPIO_scl = GPIOB,\
		.Pin_scl = GPIO_PIN_5,\
		.GPIO_sda = GPIOB,\
		.Pin_sda = GPIO_PIN_4\
	},\
    {\
		.GPIO_scl = GPIOB,\
		.Pin_scl = GPIO_PIN_6,\
		.GPIO_sda = GPIOB,\
		.Pin_sda = GPIO_PIN_7\
	},\
    {\
		.GPIO_scl = GPIOE,\
		.Pin_scl = GPIO_PIN_1,\
		.GPIO_sda = GPIOE,\
		.Pin_sda = GPIO_PIN_2\
	},\
}

typedef struct
{
    GPIO_TypeDef* GPIO_scl;
    GPIO_Pin_TypeDef Pin_scl;
	GPIO_TypeDef* GPIO_sda;
    GPIO_Pin_TypeDef Pin_sda;
}I2C_Simulation_GPIO_TypeDef;
#endif

typedef enum
{
	GPIO_Low_Level = 0,  //低电平
	GPIO_High_Level = !GPIO_Low_Level
}GPIO_OutLevel_TypeDef;  //IO输出电平

typedef enum
{
	GPIO_In = 0,  //输入
	GPIO_Out = !GPIO_In
}GPIO_Direction_TypeDef;  //IO输入/输出
#endif

typedef struct
{
	uint8_t port;  //I2C端口
	uint16_t device_adr;  //I2C从机设备地址
    uint8_t dev_adr_tenbit_flag;  //I2C从机设备10位地址模式标志位
	uint8_t * data;    //I2C读写数据指针
	uint32_t len;  //I2C读写数据长度
    uint8_t error_resend_times;  //失败重发次数
}I2C_Master_WRInfo_TypeDef;  //I2C主机读写信息

typedef I2C_Master_WRInfo_TypeDef* pI2C_Master_WRInfo_TypeDef;  //I2C主机读写信息指针

typedef enum
{
	Delay_No = 0,  //不延时
	Delay_Front,  //读之前延时
	Delay_Back  //读之后延时
}Delay_ReadReg_TypeDef;  //读寄存器延时

typedef struct
{
	I2C_Master_WRInfo_TypeDef wr_info;  //I2C主机读写信息
	uint16_t register_adr;  //I2C从机寄存器地址
	uint8_t reg_adr_twobyte_flag;  //I2C从机2字节寄存器地址标志位
	Delay_ReadReg_TypeDef readreg_delay_flag;  //I2C读寄存器发送读信号前后延时标志
	uint32_t readreg_delay_nms;  //I2C读寄存器发送读信号前后延时时间 ms
}I2C_Master_ReadReg_Info_TypeDef;  //I2C主机读寄存器信息

typedef I2C_Master_ReadReg_Info_TypeDef* pI2C_Master_ReadReg_Info_TypeDef;  //I2C主机读寄存器信息指针

void I2C_Simulation_Master_Init(void);  //模拟I2C初始化

uint8_t I2C_Simulation_Master_Write(pI2C_Master_WRInfo_TypeDef piic);  //写设备，成功返回0，失败返回1
uint8_t I2C_Simulation_Master_ReadDirect(pI2C_Master_WRInfo_TypeDef piic);  //读设备，成功返回0，失败返回1

uint8_t I2C_Simulation_Master_ReadRegister(pI2C_Master_ReadReg_Info_TypeDef piic);  //读设备寄存器，成功返回0，失败返回1

#ifdef __cplusplus
}
#endif

#endif