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
#define IIC_SLAVE_SDA_CR1       PB_CR1_C15  //������SDA���ƼĴ���1
#define IIC_SLAVE_SCL_CR1       PB_CR1_C14  //ʱ����SCL

#define IIC_SLAVE_SDA_CR2       PB_CR2_C25  //������SDA���ƼĴ���2
#define IIC_SLAVE_SCL_CR2       PB_CR2_C24  //ʱ����SCL

#define IIC_SLAVE_SDA_PIN_OUT   PB_ODR_ODR5  //������SDA�������
#define IIC_SLAVE_SCL_PIN_OUT   PB_ODR_ODR4  //ʱ����SCL

#define IIC_SLAVE_SDA_PIN_IN    PB_IDR_IDR5  //������SDA��������
#define IIC_SLAVE_SCL_PIN_IN    PB_IDR_IDR4  //ʱ����SCL

#define IIC_SLAVE_SDA_DIR       PB_DDR  //SDA����/���ѡ��Ĵ���
#define IIC_SLAVE_SCL_DIR       PB_DDR

#define IIC_SLAVE_SDA_DIR_BV    my_BV(5)  //SDA����/����Ĵ���λѡ��
#define IIC_SLAVE_SCL_DIR_BV    my_BV(4)

#define IIC_SLAVE_SDA_EXTI_CR1  EXTI_CR1_PBIS  //������SDA�жϼĴ���1
#define IIC_SLAVE_SCL_EXTI_CR1  EXTI_CR1_PBIS  //ʱ����SCL�жϼĴ���1

#define IIC_SLAVE_SDA_UP()           my_st(IIC_SLAVE_SDA_CR1 = 1;)  //SDA�������룬�������
#define IIC_SLAVE_SDA_FLOAT()        my_st(IIC_SLAVE_SDA_CR1 = 0;)  //SDA�������룬��©���

#define IIC_SLAVE_SCL_UP()           my_st(IIC_SLAVE_SCL_CR1 = 1;)  //SCL�������룬�������
#define IIC_SLAVE_SCL_FLOAT()        my_st(IIC_SLAVE_SCL_CR1 = 0;)  //SCL�������룬��©���

#define IIC_SLAVE_SDA_IQ_ON()        my_st(IIC_SLAVE_SDA_CR2 = 1;)  //SDA�ж����뿪���������
#define IIC_SLAVE_SDA_IQ_OFF()       my_st(IIC_SLAVE_SDA_CR2 = 0;)  //SDA�ж�����أ��������

#define IIC_SLAVE_SCL_IQ_ON()        my_st(IIC_SLAVE_SCL_CR2 = 1;)  //SCL�ж����뿪���������
#define IIC_SLAVE_SCL_IQ_OFF()       my_st(IIC_SLAVE_SCL_CR2 = 0;)  //SCL�ж�����أ��������

#define IIC_SLAVE_SDA_OUT(a)         my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA�����ƽ
#define IIC_SLAVE_SCL_OUT(a)         my_st(IIC_MASTER_SCL_PIN_OUT = a;)

#define IIC_SLAVE_SDA_DIR_OUT()      my_st(IIC_SLAVE_SDA_DIR |= IIC_SLAVE_SDA_DIR_BV;)  //SDA���ģʽ
#define IIC_SLAVE_SDA_DIR_IN()       my_st(IIC_SLAVE_SDA_DIR &= ~(IIC_SLAVE_SDA_DIR_BV);)

#define IIC_SLAVE_SCL_DIR_OUT()      my_st(IIC_SLAVE_SCL_DIR |= IIC_SLAVE_SCL_DIR_BV;)
#define IIC_SLAVE_SCL_DIR_IN()       my_st(IIC_SLAVE_SCL_DIR &= ~(IIC_SLAVE_SCL_DIR_BV);)

#define IIC_SLAVE_SDA_EXTI_UP()      my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x01;)  //SDA�ж������ش���
#define IIC_SLAVE_SDA_EXTI_DOWN()    my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x02;)  //SDA�ж��½��ش���
#define IIC_SLAVE_SDA_EXTI_UPDOWN()  my_st(IIC_SLAVE_SDA_EXTI_CR1 = 0x03;)  //SDA�ж��������½��ش���
    
#define IIC_SLAVE_SCL_EXTI_UP()      my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x01;)  //SCL�ж������ش���
#define IIC_SLAVE_SCL_EXTI_DOWN()    my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x02;)  //SCL�ж��½��ش���
#define IIC_SLAVE_SCL_EXTI_UPDOWN()  my_st(IIC_SLAVE_SCL_EXTI_CR1 = 0x03;)  //SCL�ж��������½��ش���

void IIC_Slave_Init(unsigned char type);

#ifdef __cplusplus
}
#endif

#endif