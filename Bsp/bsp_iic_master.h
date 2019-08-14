#ifndef __BSP_IIC_MASTER_H
#define __BSP_IIC_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "iostm8s103F3.h"

//数据类型声明
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int uint16_t;
typedef int int16_t;  //8位机int为16,32位机int为32
typedef unsigned long int uint32_t;
typedef long int int32_t;

#define HIGH 1  //引脚输出电平-高
#define LOW  0

#define OUT 1  //引脚配置输出
#define IN  0

#define DELAY_US_IIC  5  //IIC输出延时us

#define my_st(x)  do { x } while(0)  //函数语句
#define my_BV(n)  (1 << (n))  //取n位

//IIC引脚配置寄存器定义
//type = 1，IIC口1
#define IIC_MASTER_SDA_CR1       PB_CR1_C15  //数据线SDA控制寄存器1
#define IIC_MASTER_SCL_CR1       PB_CR1_C14  //时钟线SCL

#define IIC_MASTER_SDA_CR2       PB_CR2_C25  //数据线SDA控制寄存器2
#define IIC_MASTER_SCL_CR2       PB_CR2_C24  //时钟线SCL

#define IIC_MASTER_SDA_PIN_OUT   PB_ODR_ODR5  //数据线SDA输出引脚
#define IIC_MASTER_SCL_PIN_OUT   PB_ODR_ODR4  //时钟线SCL

#define IIC_MASTER_SDA_PIN_IN    PB_IDR_IDR5  //数据线SDA输入引脚
#define IIC_MASTER_SCL_PIN_IN    PB_IDR_IDR4  //时钟线SCL

#define IIC_MASTER_SDA_DIR       PB_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER_SCL_DIR       PB_DDR

#define IIC_MASTER_SDA_DIR_BV    my_BV(5)  //SDA输入/输出寄存器位选择
#define IIC_MASTER_SCL_DIR_BV    my_BV(4)

#define IIC_MASTER_SDA_UP()      my_st(IIC_MASTER_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER_SDA_FLOAT()   my_st(IIC_MASTER_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER_SCL_UP()      my_st(IIC_MASTER_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER_SCL_FLOAT()   my_st(IIC_MASTER_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER_SDA_IQ_ON()   my_st(IIC_MASTER_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER_SDA_IQ_OFF()  my_st(IIC_MASTER_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER_SCL_IQ_ON()   my_st(IIC_MASTER_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER_SCL_IQ_OFF()  my_st(IIC_MASTER_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER_SDA_OUT(a)    my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER_SCL_OUT(a)    my_st(IIC_MASTER_SCL_PIN_OUT = a;)

#define IIC_MASTER_SDA_DIR_OUT() my_st(IIC_MASTER_SDA_DIR |= IIC_MASTER_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER_SDA_DIR_IN()  my_st(IIC_MASTER_SDA_DIR &= ~(IIC_MASTER_SDA_DIR_BV);)

#define IIC_MASTER_SCL_DIR_OUT() my_st(IIC_MASTER_SCL_DIR |= IIC_MASTER_SCL_DIR_BV;)
#define IIC_MASTER_SCL_DIR_IN()  my_st(IIC_MASTER_SCL_DIR &= ~(IIC_MASTER_SCL_DIR_BV);)

//type = 2
#define IIC_MASTER2_SDA_CR1       PD_CR1_C13  //数据线SDA控制寄存器1
#define IIC_MASTER2_SCL_CR1       PD_CR1_C12  //时钟线SCL

#define IIC_MASTER2_SDA_CR2       PD_CR2_C23  //数据线SDA控制寄存器2
#define IIC_MASTER2_SCL_CR2       PD_CR2_C22  //时钟线SCL

#define IIC_MASTER2_SDA_PIN_OUT   PD_ODR_ODR3  //数据线SDA输出引脚
#define IIC_MASTER2_SCL_PIN_OUT   PD_ODR_ODR2  //时钟线SCL

#define IIC_MASTER2_SDA_PIN_IN    PD_IDR_IDR3  //数据线SDA输入引脚
#define IIC_MASTER2_SCL_PIN_IN    PD_IDR_IDR2  //时钟线SCL

#define IIC_MASTER2_SDA_DIR       PD_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER2_SCL_DIR       PD_DDR

#define IIC_MASTER2_SDA_DIR_BV    my_BV(3)  //SDA输入/输出寄存器位选择
#define IIC_MASTER2_SCL_DIR_BV    my_BV(2)

#define IIC_MASTER2_SDA_UP()      my_st(IIC_MASTER2_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER2_SDA_FLOAT()   my_st(IIC_MASTER2_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER2_SCL_UP()      my_st(IIC_MASTER2_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER2_SCL_FLOAT()   my_st(IIC_MASTER2_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER2_SDA_IQ_ON()   my_st(IIC_MASTER2_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER2_SDA_IQ_OFF()  my_st(IIC_MASTER2_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER2_SCL_IQ_ON()   my_st(IIC_MASTER2_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER2_SCL_IQ_OFF()  my_st(IIC_MASTER2_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER2_SDA_OUT(a)    my_st(IIC_MASTER2_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER2_SCL_OUT(a)    my_st(IIC_MASTER2_SCL_PIN_OUT = a;)

#define IIC_MASTER2_SDA_DIR_OUT() my_st(IIC_MASTER2_SDA_DIR |= IIC_MASTER2_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER2_SDA_DIR_IN()  my_st(IIC_MASTER2_SDA_DIR &= ~(IIC_MASTER2_SDA_DIR_BV);)

#define IIC_MASTER2_SCL_DIR_OUT() my_st(IIC_MASTER2_SCL_DIR |= IIC_MASTER2_SCL_DIR_BV;)
#define IIC_MASTER2_SCL_DIR_IN()  my_st(IIC_MASTER2_SCL_DIR &= ~(IIC_MASTER2_SCL_DIR_BV);)

//type = 3
#define IIC_MASTER3_SDA_CR1       PC_CR1_C17  //数据线SDA控制寄存器1
#define IIC_MASTER3_SCL_CR1       PC_CR1_C16  //时钟线SCL

#define IIC_MASTER3_SDA_CR2       PC_CR2_C27  //数据线SDA控制寄存器2
#define IIC_MASTER3_SCL_CR2       PC_CR2_C26  //时钟线SCL

#define IIC_MASTER3_SDA_PIN_OUT   PC_ODR_ODR7  //数据线SDA输出引脚
#define IIC_MASTER3_SCL_PIN_OUT   PC_ODR_ODR6  //时钟线SCL

#define IIC_MASTER3_SDA_PIN_IN    PC_IDR_IDR7  //数据线SDA输入引脚
#define IIC_MASTER3_SCL_PIN_IN    PC_IDR_IDR6  //时钟线SCL

#define IIC_MASTER3_SDA_DIR       PC_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER3_SCL_DIR       PC_DDR

#define IIC_MASTER3_SDA_DIR_BV    my_BV(7)  //SDA输入/输出寄存器位选择
#define IIC_MASTER3_SCL_DIR_BV    my_BV(6)

#define IIC_MASTER3_SDA_UP()      my_st(IIC_MASTER3_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER3_SDA_FLOAT()   my_st(IIC_MASTER3_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER3_SCL_UP()      my_st(IIC_MASTER3_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER3_SCL_FLOAT()   my_st(IIC_MASTER3_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER3_SDA_IQ_ON()   my_st(IIC_MASTER3_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER3_SDA_IQ_OFF()  my_st(IIC_MASTER3_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER3_SCL_IQ_ON()   my_st(IIC_MASTER3_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER3_SCL_IQ_OFF()  my_st(IIC_MASTER3_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER3_SDA_OUT(a)    my_st(IIC_MASTER3_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER3_SCL_OUT(a)    my_st(IIC_MASTER3_SCL_PIN_OUT = a;)

#define IIC_MASTER3_SDA_DIR_OUT() my_st(IIC_MASTER3_SDA_DIR |= IIC_MASTER3_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER3_SDA_DIR_IN()  my_st(IIC_MASTER3_SDA_DIR &= ~(IIC_MASTER3_SDA_DIR_BV);)

#define IIC_MASTER3_SCL_DIR_OUT() my_st(IIC_MASTER3_SCL_DIR |= IIC_MASTER3_SCL_DIR_BV;)
#define IIC_MASTER3_SCL_DIR_IN()  my_st(IIC_MASTER3_SCL_DIR &= ~(IIC_MASTER3_SCL_DIR_BV);)

void IIC_Master_Init(uint8_t type);  //IIC口初始化，为空闲状态

uint8_t IIC_Master_Write(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len);  //写数据到设备寄存器，成功返回0，失败返回1
uint8_t IIC_Master_Read(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len);  //读设备指定长度数据

#ifdef __cplusplus
}
#endif

#endif