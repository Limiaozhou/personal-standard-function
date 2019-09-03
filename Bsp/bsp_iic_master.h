#ifndef __BSP_IIC_MASTER_H
#define __BSP_IIC_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

// #include "iostm8s103F3.h"
//#include "iostm8s207c8.h"
#include "stm8s_gpio.h"
#include "delay.h"

//#include "typedef.h"

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

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

typedef struct
{
	uint8_t port;  //IIC端口
	uint16_t device_adr;  //IIC从机设备地址
    uint8_t dev_adr_tenbit_flag;  //IIC从机设备10位地址模式标志位
	uint16_t register_adr;  //IIC从机寄存器地址
	uint8_t reg_adr_twobyte_flag;  //IIC从机2字节寄存器地址标志位
	uint8_t readreg_delay_flag;  //IIC读寄存器发送读信号后是否延时标志
	uint32_t readreg_delay_nms;  //IIC读寄存器发送读信号后延时时间 ms
	uint8_t * data;    //IIC读写数据指针
	uint32_t len;  //IIC读写数据长度
    uint8_t error_resend_times;  //失败重发次数
}IIC_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息

typedef IIC_Master_ReadReg_Info_TypeDef* pIIC_Master_ReadReg_Info_TypeDef;  //IIC主机读寄存器信息指针

#define HIGH 1  //引脚输出电平-高
#define LOW  0

#define OUT 1  //引脚配置输出
#define IN  0

#define DELAY_US_IIC  5  //IIC输出延时us

#define my_st(x)  do { x } while(0)  //函数语句
#define my_BV(n)  (1 << (n))  //取n位

//IIC引脚配置寄存器定义
//port = 1，IIC口1
#define IIC_MASTER_SDA_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_7))  //数据线SDA输入引脚
#define IIC_MASTER_SCL_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_6))  //时钟线SCL

#define IIC_MASTER_SDA_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_7;)  //SDA上拉输入，推挽输出
#define IIC_MASTER_SDA_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_7);)  //SDA浮空输入，开漏输出

#define IIC_MASTER_SCL_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_6;)  //SCL上拉输入，推挽输出
#define IIC_MASTER_SCL_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_6);)  //SCL浮空输入，开漏输出

#define IIC_MASTER_SDA_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_7;)  //SDA中断输入开，高速输出
#define IIC_MASTER_SDA_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_7);)  //SDA中断输入关，低速输出

#define IIC_MASTER_SCL_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_6;)  //SCL中断输入开，高速输出
#define IIC_MASTER_SCL_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_6);)  //SCL中断输入关，低速输出

#define IIC_MASTER_SDA_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_7; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_7);)  //SDA输出电平
#define IIC_MASTER_SCL_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_6; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_6);)

#define IIC_MASTER_SDA_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_7;)  //SDA输出模式
#define IIC_MASTER_SDA_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_7);)

#define IIC_MASTER_SCL_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_6;)
#define IIC_MASTER_SCL_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_6);)

// #define IIC_MASTER_SDA_CR1       PB_CR1_C15  //数据线SDA控制寄存器1
// #define IIC_MASTER_SCL_CR1       PB_CR1_C14  //时钟线SCL

// #define IIC_MASTER_SDA_CR2       PB_CR2_C25  //数据线SDA控制寄存器2
// #define IIC_MASTER_SCL_CR2       PB_CR2_C24  //时钟线SCL

// #define IIC_MASTER_SDA_PIN_OUT   PB_ODR_ODR5  //数据线SDA输出引脚
// #define IIC_MASTER_SCL_PIN_OUT   PB_ODR_ODR4  //时钟线SCL

// #define IIC_MASTER_SDA_PIN_IN    PB_IDR_IDR5  //数据线SDA输入引脚
// #define IIC_MASTER_SCL_PIN_IN    PB_IDR_IDR4  //时钟线SCL

// #define IIC_MASTER_SDA_DIR       PB_DDR  //SDA输入/输出选择寄存器
// #define IIC_MASTER_SCL_DIR       PB_DDR

// #define IIC_MASTER_SDA_DIR_BV    my_BV(5)  //SDA输入/输出寄存器位选择
// #define IIC_MASTER_SCL_DIR_BV    my_BV(4)

// #define IIC_MASTER_SDA_UP()      my_st(IIC_MASTER_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
// #define IIC_MASTER_SDA_FLOAT()   my_st(IIC_MASTER_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

// #define IIC_MASTER_SCL_UP()      my_st(IIC_MASTER_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
// #define IIC_MASTER_SCL_FLOAT()   my_st(IIC_MASTER_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

// #define IIC_MASTER_SDA_IQ_ON()   my_st(IIC_MASTER_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
// #define IIC_MASTER_SDA_IQ_OFF()  my_st(IIC_MASTER_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

// #define IIC_MASTER_SCL_IQ_ON()   my_st(IIC_MASTER_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
// #define IIC_MASTER_SCL_IQ_OFF()  my_st(IIC_MASTER_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

// #define IIC_MASTER_SDA_OUT(a)    my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA输出电平
// #define IIC_MASTER_SCL_OUT(a)    my_st(IIC_MASTER_SCL_PIN_OUT = a;)

// #define IIC_MASTER_SDA_DIR_OUT() my_st(IIC_MASTER_SDA_DIR |= IIC_MASTER_SDA_DIR_BV;)  //SDA输出模式
// #define IIC_MASTER_SDA_DIR_IN()  my_st(IIC_MASTER_SDA_DIR &= ~(IIC_MASTER_SDA_DIR_BV);)

// #define IIC_MASTER_SCL_DIR_OUT() my_st(IIC_MASTER_SCL_DIR |= IIC_MASTER_SCL_DIR_BV;)
// #define IIC_MASTER_SCL_DIR_IN()  my_st(IIC_MASTER_SCL_DIR &= ~(IIC_MASTER_SCL_DIR_BV);)


//port = 2
#define IIC_MASTER2_SDA_PIN_IN    (!!(GPIOB->IDR & GPIO_PIN_4))  //数据线SDA输入引脚
#define IIC_MASTER2_SCL_PIN_IN    (!!(GPIOB->IDR & GPIO_PIN_5))  //时钟线SCL

#define IIC_MASTER2_SDA_UP()      my_st(GPIOB->CR1 |= (uint8_t)GPIO_PIN_4;)  //SDA上拉输入，推挽输出
#define IIC_MASTER2_SDA_FLOAT()   my_st(GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_4);)  //SDA浮空输入，开漏输出

#define IIC_MASTER2_SCL_UP()      my_st(GPIOB->CR1 |= (uint8_t)GPIO_PIN_5;)  //SCL上拉输入，推挽输出
#define IIC_MASTER2_SCL_FLOAT()   my_st(GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_5);)  //SCL浮空输入，开漏输出

#define IIC_MASTER2_SDA_IQ_ON()   my_st(GPIOB->CR2 |= (uint8_t)GPIO_PIN_4;)  //SDA中断输入开，高速输出
#define IIC_MASTER2_SDA_IQ_OFF()  my_st(GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_4);)  //SDA中断输入关，低速输出

#define IIC_MASTER2_SCL_IQ_ON()   my_st(GPIOB->CR2 |= (uint8_t)GPIO_PIN_5;)  //SCL中断输入开，高速输出
#define IIC_MASTER2_SCL_IQ_OFF()  my_st(GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_5);)  //SCL中断输入关，低速输出

#define IIC_MASTER2_SDA_OUT(a)    my_st(if(a) GPIOB->ODR |= (uint8_t)GPIO_PIN_4; else GPIOB->ODR &= (uint8_t)(~GPIO_PIN_4);)  //SDA输出电平
#define IIC_MASTER2_SCL_OUT(a)    my_st(if(a) GPIOB->ODR |= (uint8_t)GPIO_PIN_5; else GPIOB->ODR &= (uint8_t)(~GPIO_PIN_5);)

#define IIC_MASTER2_SDA_DIR_OUT() my_st(GPIOB->DDR |= (uint8_t)GPIO_PIN_4;)  //SDA输出模式
#define IIC_MASTER2_SDA_DIR_IN()  my_st(GPIOB->DDR &= (uint8_t)(~GPIO_PIN_4);)

#define IIC_MASTER2_SCL_DIR_OUT() my_st(GPIOB->DDR |= (uint8_t)GPIO_PIN_5;)
#define IIC_MASTER2_SCL_DIR_IN()  my_st(GPIOB->DDR &= (uint8_t)(~GPIO_PIN_5);)

// #define IIC_MASTER2_SDA_CR1       PD_CR1_C13  //数据线SDA控制寄存器1
// #define IIC_MASTER2_SCL_CR1       PD_CR1_C12  //时钟线SCL

// #define IIC_MASTER2_SDA_CR2       PD_CR2_C23  //数据线SDA控制寄存器2
// #define IIC_MASTER2_SCL_CR2       PD_CR2_C22  //时钟线SCL

// #define IIC_MASTER2_SDA_PIN_OUT   PD_ODR_ODR3  //数据线SDA输出引脚
// #define IIC_MASTER2_SCL_PIN_OUT   PD_ODR_ODR2  //时钟线SCL

// #define IIC_MASTER2_SDA_PIN_IN    PD_IDR_IDR3  //数据线SDA输入引脚
// #define IIC_MASTER2_SCL_PIN_IN    PD_IDR_IDR2  //时钟线SCL

// #define IIC_MASTER2_SDA_DIR       PD_DDR  //SDA输入/输出选择寄存器
// #define IIC_MASTER2_SCL_DIR       PD_DDR

// #define IIC_MASTER2_SDA_DIR_BV    my_BV(3)  //SDA输入/输出寄存器位选择
// #define IIC_MASTER2_SCL_DIR_BV    my_BV(2)

// #define IIC_MASTER2_SDA_UP()      my_st(IIC_MASTER2_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
// #define IIC_MASTER2_SDA_FLOAT()   my_st(IIC_MASTER2_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

// #define IIC_MASTER2_SCL_UP()      my_st(IIC_MASTER2_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
// #define IIC_MASTER2_SCL_FLOAT()   my_st(IIC_MASTER2_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

// #define IIC_MASTER2_SDA_IQ_ON()   my_st(IIC_MASTER2_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
// #define IIC_MASTER2_SDA_IQ_OFF()  my_st(IIC_MASTER2_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

// #define IIC_MASTER2_SCL_IQ_ON()   my_st(IIC_MASTER2_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
// #define IIC_MASTER2_SCL_IQ_OFF()  my_st(IIC_MASTER2_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

// #define IIC_MASTER2_SDA_OUT(a)    my_st(IIC_MASTER2_SDA_PIN_OUT = a;)  //SDA输出电平
// #define IIC_MASTER2_SCL_OUT(a)    my_st(IIC_MASTER2_SCL_PIN_OUT = a;)

// #define IIC_MASTER2_SDA_DIR_OUT() my_st(IIC_MASTER2_SDA_DIR |= IIC_MASTER2_SDA_DIR_BV;)  //SDA输出模式
// #define IIC_MASTER2_SDA_DIR_IN()  my_st(IIC_MASTER2_SDA_DIR &= ~(IIC_MASTER2_SDA_DIR_BV);)

// #define IIC_MASTER2_SCL_DIR_OUT() my_st(IIC_MASTER2_SCL_DIR |= IIC_MASTER2_SCL_DIR_BV;)
// #define IIC_MASTER2_SCL_DIR_IN()  my_st(IIC_MASTER2_SCL_DIR &= ~(IIC_MASTER2_SCL_DIR_BV);)


//port = 3
#define IIC_MASTER3_SDA_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_7))  //数据线SDA输入引脚
#define IIC_MASTER3_SCL_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_6))  //时钟线SCL

#define IIC_MASTER3_SDA_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_7;)  //SDA上拉输入，推挽输出
#define IIC_MASTER3_SDA_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_7);)  //SDA浮空输入，开漏输出

#define IIC_MASTER3_SCL_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_6;)  //SCL上拉输入，推挽输出
#define IIC_MASTER3_SCL_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_6);)  //SCL浮空输入，开漏输出

#define IIC_MASTER3_SDA_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_7;)  //SDA中断输入开，高速输出
#define IIC_MASTER3_SDA_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_7);)  //SDA中断输入关，低速输出

#define IIC_MASTER3_SCL_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_6;)  //SCL中断输入开，高速输出
#define IIC_MASTER3_SCL_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_6);)  //SCL中断输入关，低速输出

#define IIC_MASTER3_SDA_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_7; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_7);)  //SDA输出电平
#define IIC_MASTER3_SCL_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_6; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_6);)

#define IIC_MASTER3_SDA_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_7;)  //SDA输出模式
#define IIC_MASTER3_SDA_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_7);)

#define IIC_MASTER3_SCL_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_6;)
#define IIC_MASTER3_SCL_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_6);)

// #define IIC_MASTER3_SDA_CR1       PC_CR1_C17  //数据线SDA控制寄存器1
// #define IIC_MASTER3_SCL_CR1       PC_CR1_C16  //时钟线SCL

// #define IIC_MASTER3_SDA_CR2       PC_CR2_C27  //数据线SDA控制寄存器2
// #define IIC_MASTER3_SCL_CR2       PC_CR2_C26  //时钟线SCL

// #define IIC_MASTER3_SDA_PIN_OUT   PC_ODR_ODR7  //数据线SDA输出引脚
// #define IIC_MASTER3_SCL_PIN_OUT   PC_ODR_ODR6  //时钟线SCL

// #define IIC_MASTER3_SDA_PIN_IN    PC_IDR_IDR7  //数据线SDA输入引脚
// #define IIC_MASTER3_SCL_PIN_IN    PC_IDR_IDR6  //时钟线SCL

// #define IIC_MASTER3_SDA_DIR       PC_DDR  //SDA输入/输出选择寄存器
// #define IIC_MASTER3_SCL_DIR       PC_DDR

// #define IIC_MASTER3_SDA_DIR_BV    my_BV(7)  //SDA输入/输出寄存器位选择
// #define IIC_MASTER3_SCL_DIR_BV    my_BV(6)

// #define IIC_MASTER3_SDA_UP()      my_st(IIC_MASTER3_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
// #define IIC_MASTER3_SDA_FLOAT()   my_st(IIC_MASTER3_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

// #define IIC_MASTER3_SCL_UP()      my_st(IIC_MASTER3_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
// #define IIC_MASTER3_SCL_FLOAT()   my_st(IIC_MASTER3_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

// #define IIC_MASTER3_SDA_IQ_ON()   my_st(IIC_MASTER3_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
// #define IIC_MASTER3_SDA_IQ_OFF()  my_st(IIC_MASTER3_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

// #define IIC_MASTER3_SCL_IQ_ON()   my_st(IIC_MASTER3_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
// #define IIC_MASTER3_SCL_IQ_OFF()  my_st(IIC_MASTER3_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

// #define IIC_MASTER3_SDA_OUT(a)    my_st(IIC_MASTER3_SDA_PIN_OUT = a;)  //SDA输出电平
// #define IIC_MASTER3_SCL_OUT(a)    my_st(IIC_MASTER3_SCL_PIN_OUT = a;)

// #define IIC_MASTER3_SDA_DIR_OUT() my_st(IIC_MASTER3_SDA_DIR |= IIC_MASTER3_SDA_DIR_BV;)  //SDA输出模式
// #define IIC_MASTER3_SDA_DIR_IN()  my_st(IIC_MASTER3_SDA_DIR &= ~(IIC_MASTER3_SDA_DIR_BV);)

// #define IIC_MASTER3_SCL_DIR_OUT() my_st(IIC_MASTER3_SCL_DIR |= IIC_MASTER3_SCL_DIR_BV;)
// #define IIC_MASTER3_SCL_DIR_IN()  my_st(IIC_MASTER3_SCL_DIR &= ~(IIC_MASTER3_SCL_DIR_BV);)

void IIC_Master_Init(uint8_t port);  //初始化选择的IIC口，为空闲状态

uint8_t IIC_Master_Write(pIIC_Master_WRInfo_TypeDef piic);  //写设备，成功返回0，失败返回1
uint8_t IIC_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic);  //读设备，成功返回0，失败返回1

uint8_t IIC_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic);  //读设备寄存器，成功返回0，失败返回1

#ifdef __cplusplus
}
#endif

#endif