#ifndef __BSP_IIC_SLAVE_H
#define __BSP_IIC_SLAVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "iostm8s103F3.h"
#include "bsp_delay.h"
#include "bsp_iic_master.h"

#define SlaveAddress 0x3C

//type = 1
#define IIC_SLAVE_SDA_CR1       PB_CR1_C15  //数据线SDA控制寄存器1
#define IIC_SLAVE_SCL_CR1       PB_CR1_C14  //时钟线SCL

#define IIC_SLAVE_SDA_CR2       PB_CR2_C25  //数据线SDA控制寄存器2
#define IIC_SLAVE_SCL_CR2       PB_CR2_C24  //时钟线SCL

#define IIC_SLAVE_SDA_PIN_OUT   PB_ODR_ODR5  //数据线SDA输出引脚
#define IIC_SLAVE_SCL_PIN_OUT   PB_ODR_ODR4  //时钟线SCL

#define IIC_SLAVE_SDA_PIN_IN    PB_IDR_IDR5  //数据线SDA输入引脚
#define IIC_SLAVE_SCL_PIN_IN    PB_IDR_IDR4  //时钟线SCL

#define IIC_SLAVE_SDA_DIR       PB_DDR  //SDA输入/输出选择寄存器
#define IIC_SLAVE_SCL_DIR       PB_DDR

#define IIC_SLAVE_SDA_DIR_BV    my_BV(5)  //SDA输入/输出寄存器位选择
#define IIC_SLAVE_SCL_DIR_BV    my_BV(4)

#define IIC_SLAVE_SDA_EXTI_CR1  EXTI_CR1_PBIS  //数据线SDA中断寄存器1
#define IIC_SLAVE_SCL_EXTI_CR1  EXTI_CR1_PBIS  //时钟线SCL中断寄存器1

#define IIC_SLAVE_SDA_UP()           my_st(IIC_SLAVE_SDA_CR1 = 1;)  //SDA上拉输入，推挽输出
#define IIC_SLAVE_SDA_FLOAT()        my_st(IIC_SLAVE_SDA_CR1 = 0;)  //SDA浮空输入，开漏输出

#define IIC_SLAVE_SCL_UP()           my_st(IIC_SLAVE_SCL_CR1 = 1;)  //SCL上拉输入，推挽输出
#define IIC_SLAVE_SCL_FLOAT()        my_st(IIC_SLAVE_SCL_CR1 = 0;)  //SCL浮空输入，开漏输出

#define IIC_SLAVE_SDA_IQ_ON()        my_st(IIC_SLAVE_SDA_CR2 = 1;)  //SDA中断输入开，高速输出
#define IIC_SLAVE_SDA_IQ_OFF()       my_st(IIC_SLAVE_SDA_CR2 = 0;)  //SDA中断输入关，低速输出

#define IIC_SLAVE_SCL_IQ_ON()        my_st(IIC_SLAVE_SCL_CR2 = 1;)  //SCL中断输入开，高速输出
#define IIC_SLAVE_SCL_IQ_OFF()       my_st(IIC_SLAVE_SCL_CR2 = 0;)  //SCL中断输入关，低速输出

#define IIC_SLAVE_SDA_OUT(a)         my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_SLAVE_SCL_OUT(a)         my_st(IIC_MASTER_SCL_PIN_OUT = a;)

#define IIC_SLAVE_SDA_DIR_OUT()      my_st(IIC_SLAVE_SDA_DIR |= IIC_SLAVE_SDA_DIR_BV;)  //SDA输出模式
#define IIC_SLAVE_SDA_DIR_IN()       my_st(IIC_SLAVE_SDA_DIR &= ~(IIC_SLAVE_SDA_DIR_BV);)

#define IIC_SLAVE_SCL_DIR_OUT()      my_st(IIC_SLAVE_SCL_DIR |= IIC_SLAVE_SCL_DIR_BV;)
#define IIC_SLAVE_SCL_DIR_IN()       my_st(IIC_SLAVE_SCL_DIR &= ~(IIC_SLAVE_SCL_DIR_BV);)

#define IIC_SLAVE_SDA_EXTI_UP()      my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x01;)  //SDA中断上升沿触发
#define IIC_SLAVE_SDA_EXTI_DOWN()    my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x02;)  //SDA中断下降沿触发
#define IIC_SLAVE_SDA_EXTI_UPDOWN()  my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x03;)  //SDA中断上升和下降沿触发
    
#define IIC_SLAVE_SCL_EXTI_UP()      my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x01;)  //SCL中断上升沿触发
#define IIC_SLAVE_SCL_EXTI_DOWN()    my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x02;)  //SCL中断下降沿触发
#define IIC_SLAVE_SCL_EXTI_UPDOWN()  my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x03;)  //SCL中断上升和下降沿触发

void IIC_Slave_Init(unsigned char type);

#ifdef __cplusplus
}
#endif

#endif