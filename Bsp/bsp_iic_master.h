#ifndef __BSP_IIC_MASTER_H
#define __BSP_IIC_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "iostm8s103F3.h"

#define HIGH 1
#define LOW  0

#define OUT 1
#define IN  0

#define DELAY_US_IIC  5

#define my_st(x)      do { x } while(0)
#define my_BV(n)      (1 << (n))

//type = 1
#define IIC_MASTER_SDA_CR1      PB_CR1_C15  //数据线SDA控制寄存器1
#define IIC_MASTER_SCL_CR1      PB_CR1_C14  //时钟线SCL

#define IIC_MASTER_SDA_CR2      PB_CR2_C25  //数据线SDA控制寄存器2
#define IIC_MASTER_SCL_CR2      PB_CR2_C24  //时钟线SCL

#define IIC_MASTER_SDA_PIN_OUT  PB_ODR_ODR5  //数据线SDA输出引脚
#define IIC_MASTER_SCL_PIN_OUT  PB_ODR_ODR4  //时钟线SCL

#define IIC_MASTER_SDA_PIN_IN   PB_IDR_IDR5  //数据线SDA输入引脚
#define IIC_MASTER_SCL_PIN_IN   PB_IDR_IDR4  //时钟线SCL

#define IIC_MASTER_SDA_DIR      PB_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER_SCL_DIR      PB_DDR

#define IIC_MASTER_SDA_DIR_BV   my_BV(5)  //SDA输入/输出寄存器位选择
#define IIC_MASTER_SCL_DIR_BV   my_BV(4)

#define IIC_MASTER_SDA_UP()           my_st(IIC_MASTER_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER_SDA_FLOAT()        my_st(IIC_MASTER_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER_SCL_UP()           my_st(IIC_MASTER_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER_SCL_FLOAT()        my_st(IIC_MASTER_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER_SDA_IQ_ON()        my_st(IIC_MASTER_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER_SDA_IQ_OFF()       my_st(IIC_MASTER_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER_SCL_IQ_ON()        my_st(IIC_MASTER_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER_SCL_IQ_OFF()       my_st(IIC_MASTER_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER_SDA_OUT(a)    my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER_SCL_OUT(a)    my_st(IIC_MASTER_SCL_PIN_OUT = a;)

#define IIC_MASTER_SDA_DIR_OUT() my_st(IIC_MASTER_SDA_DIR |= IIC_MASTER_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER_SDA_DIR_IN()  my_st(IIC_MASTER_SDA_DIR &= ~(IIC_MASTER_SDA_DIR_BV);)

#define IIC_MASTER_SCL_DIR_OUT() my_st(IIC_MASTER_SCL_DIR |= IIC_MASTER_SCL_DIR_BV;)
#define IIC_MASTER_SCL_DIR_IN()  my_st(IIC_MASTER_SCL_DIR &= ~(IIC_MASTER_SCL_DIR_BV);)

//type = 2
#define IIC_MASTER2_SDA_CR1      PD_CR1_C13  //数据线SDA控制寄存器1
#define IIC_MASTER2_SCL_CR1      PD_CR1_C12  //时钟线SCL

#define IIC_MASTER2_SDA_CR2      PD_CR2_C23  //数据线SDA控制寄存器2
#define IIC_MASTER2_SCL_CR2      PD_CR2_C22  //时钟线SCL

#define IIC_MASTER2_SDA_PIN_OUT  PD_ODR_ODR3  //数据线SDA输出引脚
#define IIC_MASTER2_SCL_PIN_OUT  PD_ODR_ODR2  //时钟线SCL

#define IIC_MASTER2_SDA_PIN_IN   PD_IDR_IDR3  //数据线SDA输入引脚
#define IIC_MASTER2_SCL_PIN_IN   PD_IDR_IDR2  //时钟线SCL

#define IIC_MASTER2_SDA_DIR      PD_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER2_SCL_DIR      PD_DDR

#define IIC_MASTER2_SDA_DIR_BV   my_BV(3)  //SDA输入/输出寄存器位选择
#define IIC_MASTER2_SCL_DIR_BV   my_BV(2)

#define IIC_MASTER2_SDA_UP()           my_st(IIC_MASTER2_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER2_SDA_FLOAT()        my_st(IIC_MASTER2_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER2_SCL_UP()           my_st(IIC_MASTER2_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER2_SCL_FLOAT()        my_st(IIC_MASTER2_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER2_SDA_IQ_ON()        my_st(IIC_MASTER2_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER2_SDA_IQ_OFF()       my_st(IIC_MASTER2_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER2_SCL_IQ_ON()        my_st(IIC_MASTER2_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER2_SCL_IQ_OFF()       my_st(IIC_MASTER2_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER2_SDA_OUT(a)    my_st(IIC_MASTER2_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER2_SCL_OUT(a)    my_st(IIC_MASTER2_SCL_PIN_OUT = a;)

#define IIC_MASTER2_SDA_DIR_OUT() my_st(IIC_MASTER2_SDA_DIR |= IIC_MASTER2_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER2_SDA_DIR_IN()  my_st(IIC_MASTER2_SDA_DIR &= ~(IIC_MASTER2_SDA_DIR_BV);)

#define IIC_MASTER2_SCL_DIR_OUT() my_st(IIC_MASTER2_SCL_DIR |= IIC_MASTER2_SCL_DIR_BV;)
#define IIC_MASTER2_SCL_DIR_IN()  my_st(IIC_MASTER2_SCL_DIR &= ~(IIC_MASTER2_SCL_DIR_BV);)

//type = 3
#define IIC_MASTER3_SDA_CR1      PC_CR1_C17  //数据线SDA控制寄存器1
#define IIC_MASTER3_SCL_CR1      PC_CR1_C16  //时钟线SCL

#define IIC_MASTER3_SDA_CR2      PC_CR2_C27  //数据线SDA控制寄存器2
#define IIC_MASTER3_SCL_CR2      PC_CR2_C26  //时钟线SCL

#define IIC_MASTER3_SDA_PIN_OUT  PC_ODR_ODR7  //数据线SDA输出引脚
#define IIC_MASTER3_SCL_PIN_OUT  PC_ODR_ODR6  //时钟线SCL

#define IIC_MASTER3_SDA_PIN_IN   PC_IDR_IDR7  //数据线SDA输入引脚
#define IIC_MASTER3_SCL_PIN_IN   PC_IDR_IDR6  //时钟线SCL

#define IIC_MASTER3_SDA_DIR      PC_DDR  //SDA输入/输出选择寄存器
#define IIC_MASTER3_SCL_DIR      PC_DDR

#define IIC_MASTER3_SDA_DIR_BV   my_BV(7)  //SDA输入/输出寄存器位选择
#define IIC_MASTER3_SCL_DIR_BV   my_BV(6)

#define IIC_MASTER3_SDA_UP()           my_st(IIC_MASTER3_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_MASTER3_SDA_FLOAT()        my_st(IIC_MASTER3_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_MASTER3_SCL_UP()           my_st(IIC_MASTER3_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_MASTER3_SCL_FLOAT()        my_st(IIC_MASTER3_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_MASTER3_SDA_IQ_ON()        my_st(IIC_MASTER3_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_MASTER3_SDA_IQ_OFF()       my_st(IIC_MASTER3_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_MASTER3_SCL_IQ_ON()        my_st(IIC_MASTER3_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_MASTER3_SCL_IQ_OFF()       my_st(IIC_MASTER3_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_MASTER3_SDA_OUT(a)    my_st(IIC_MASTER3_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_MASTER3_SCL_OUT(a)    my_st(IIC_MASTER3_SCL_PIN_OUT = a;)

#define IIC_MASTER3_SDA_DIR_OUT() my_st(IIC_MASTER3_SDA_DIR |= IIC_MASTER3_SDA_DIR_BV;)  //SDA输出模式
#define IIC_MASTER3_SDA_DIR_IN()  my_st(IIC_MASTER3_SDA_DIR &= ~(IIC_MASTER3_SDA_DIR_BV);)

#define IIC_MASTER3_SCL_DIR_OUT() my_st(IIC_MASTER3_SCL_DIR |= IIC_MASTER3_SCL_DIR_BV;)
#define IIC_MASTER3_SCL_DIR_IN()  my_st(IIC_MASTER3_SCL_DIR &= ~(IIC_MASTER3_SCL_DIR_BV);)

void delay_us(unsigned int utime);
void my_delay_ms(unsigned int mtime);

void IIC_Master_Init(unsigned char type);

unsigned char IIC_Master_Write(unsigned char type, unsigned char device_adr, unsigned char reg_adr, unsigned char * data, unsigned char len);
void IIC_Master_Read(unsigned char type, unsigned char device_adr, unsigned char reg_adr, unsigned char * data, unsigned char len);

#ifdef __cplusplus
}
#endif

#endif