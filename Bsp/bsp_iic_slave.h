#ifndef __BSP_IIC_SLAVE_H
#define __BSP_IIC_SLAVE_H

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

#define my_st(x)  do { x } while(0)  //函数语句
#define my_BV(n)  (1 << (n))  //取n位为1

#define IIC_DEVICE_ADDRESS 0x3C  //本机IIC设备地址

//IIC引脚配置寄存器定义
//port = 1
#define IIC_SLAVE_SDA_PULL       PB_CR1_C15  //数据线SDA控制寄存器1，控制上下拉
#define IIC_SLAVE_SCL_PULL       PB_CR1_C14  //时钟线SCL

#define IIC_SLAVE_SDA_IQ         PB_CR2_C25  //数据线SDA控制寄存器2，控制中断开关
#define IIC_SLAVE_SCL_IQ         PB_CR2_C24  //时钟线SCL

#define IIC_SLAVE_SDA_PIN_OUT    PB_ODR_ODR5  //数据线SDA输出引脚
#define IIC_SLAVE_SCL_PIN_OUT    PB_ODR_ODR4  //时钟线SCL

#define IIC_SLAVE_SDA_PIN_IN     PB_IDR_IDR5  //数据线SDA输入引脚
#define IIC_SLAVE_SCL_PIN_IN     PB_IDR_IDR4  //时钟线SCL

#define IIC_SLAVE_SDA_DIR        PB_DDR  //SDA输入/输出选择寄存器
#define IIC_SLAVE_SCL_DIR        PB_DDR

#define IIC_SLAVE_SDA_DIR_BV     my_BV(5)  //SDA输入/输出寄存器位选择
#define IIC_SLAVE_SCL_DIR_BV     my_BV(4)

#define IIC_SLAVE_SDA_EXTI_EDGE  EXTI_CR1_PBIS  //数据线SDA中断寄存器1
#define IIC_SLAVE_SCL_EXTI_EDGE  EXTI_CR1_PBIS  //时钟线SCL中断寄存器1

#define IIC_SLAVE_SDA_UP()           my_st(IIC_SLAVE_SDA_PULL = 1;)  //SDA上拉输入，推挽输出
#define IIC_SLAVE_SDA_FLOAT()        my_st(IIC_SLAVE_SDA_PULL = 0;)  //SDA浮空输入，开漏输出

#define IIC_SLAVE_SCL_UP()           my_st(IIC_SLAVE_SCL_PULL = 1;)  //SCL上拉输入，推挽输出
#define IIC_SLAVE_SCL_FLOAT()        my_st(IIC_SLAVE_SCL_PULL = 0;)  //SCL浮空输入，开漏输出

#define IIC_SLAVE_SDA_IQ_ON()        my_st(IIC_SLAVE_SDA_IQ = 1;)  //SDA中断输入开，高速输出
#define IIC_SLAVE_SDA_IQ_OFF()       my_st(IIC_SLAVE_SDA_IQ = 0;)  //SDA中断输入关，低速输出

#define IIC_SLAVE_SCL_IQ_ON()        my_st(IIC_SLAVE_SCL_IQ = 1;)  //SCL中断输入开，高速输出
#define IIC_SLAVE_SCL_IQ_OFF()       my_st(IIC_SLAVE_SCL_IQ = 0;)  //SCL中断输入关，低速输出

#define IIC_SLAVE_SDA_OUT(a)         my_st(IIC_SLAVE_SDA_PIN_OUT = a;)  //SDA输出电平
#define IIC_SLAVE_SCL_OUT(a)         my_st(IIC_SLAVE_SCL_PIN_OUT = a;)

#define IIC_SLAVE_SDA_DIR_OUT()      my_st(IIC_SLAVE_SDA_DIR |= IIC_SLAVE_SDA_DIR_BV;)  //SDA输出模式
#define IIC_SLAVE_SDA_DIR_IN()       my_st(IIC_SLAVE_SDA_DIR &= ~(IIC_SLAVE_SDA_DIR_BV);)

#define IIC_SLAVE_SCL_DIR_OUT()      my_st(IIC_SLAVE_SCL_DIR |= IIC_SLAVE_SCL_DIR_BV;)
#define IIC_SLAVE_SCL_DIR_IN()       my_st(IIC_SLAVE_SCL_DIR &= ~(IIC_SLAVE_SCL_DIR_BV);)

#define IIC_SLAVE_SDA_EXTI_UP()      my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x01;)  //SDA中断上升沿触发
#define IIC_SLAVE_SDA_EXTI_DOWN()    my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x02;)  //SDA中断下降沿触发
#define IIC_SLAVE_SDA_EXTI_UPDOWN()  my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x03;)  //SDA中断上升和下降沿触发
    
#define IIC_SLAVE_SCL_EXTI_UP()      my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x01;)  //SCL中断上升沿触发
#define IIC_SLAVE_SCL_EXTI_DOWN()    my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x02;)  //SCL中断下降沿触发
#define IIC_SLAVE_SCL_EXTI_UPDOWN()  my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x03;)  //SCL中断上升和下降沿触发

typedef void (*iic_slave_be_written)(uint16_t reg_adr, uint8_t *data, uint16_t len);  //IIC从机被写回调函数，接收指定长度数据到指定寄存器
typedef void (*iic_slave_be_read)(uint16_t reg_adr, uint8_t *data, uint16_t len);  //IIC从机被读回调函数，发送指定长度数据

void IIC_Slave_Init(uint8_t port, iic_slave_be_written be_written_cb, iic_slave_be_read be_read_cb);  //IIC从机初始化，注册读写回调函数

#ifdef __cplusplus
}
#endif

#endif