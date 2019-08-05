#ifndef __BSP_IIC_SLAVE_H
#define __BSP_IIC_SLAVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "iostm8s103F3.h"

//������������
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int uint16_t;
typedef int int16_t;  //8λ��intΪ16,32λ��intΪ32
typedef unsigned long int uint32_t;
typedef long int int32_t;

#define HIGH 1  //���������ƽ-��
#define LOW  0

#define my_st(x)  do { x } while(0)  //�������
#define my_BV(n)  (1 << (n))  //ȡnλΪ1

#define IICAddress 0x3C  //����IIC��ַ

//IIC�������üĴ�������
//port = 1
#define IIC_SLAVE_SDA_PULL       PB_CR1_C15  //������SDA���ƼĴ���1������������
#define IIC_SLAVE_SCL_PULL       PB_CR1_C14  //ʱ����SCL

#define IIC_SLAVE_SDA_IQ         PB_CR2_C25  //������SDA���ƼĴ���2�������жϿ���
#define IIC_SLAVE_SCL_IQ         PB_CR2_C24  //ʱ����SCL

#define IIC_SLAVE_SDA_PIN_OUT    PB_ODR_ODR5  //������SDA�������
#define IIC_SLAVE_SCL_PIN_OUT    PB_ODR_ODR4  //ʱ����SCL

#define IIC_SLAVE_SDA_PIN_IN     PB_IDR_IDR5  //������SDA��������
#define IIC_SLAVE_SCL_PIN_IN     PB_IDR_IDR4  //ʱ����SCL

#define IIC_SLAVE_SDA_DIR        PB_DDR  //SDA����/���ѡ��Ĵ���
#define IIC_SLAVE_SCL_DIR        PB_DDR

#define IIC_SLAVE_SDA_DIR_BV     my_BV(5)  //SDA����/����Ĵ���λѡ��
#define IIC_SLAVE_SCL_DIR_BV     my_BV(4)

#define IIC_SLAVE_SDA_EXTI_EDGE  EXTI_CR1_PBIS  //������SDA�жϼĴ���1
#define IIC_SLAVE_SCL_EXTI_EDGE  EXTI_CR1_PBIS  //ʱ����SCL�жϼĴ���1

#define IIC_SLAVE_SDA_UP()           my_st(IIC_SLAVE_SDA_PULL = 1;)  //SDA�������룬�������
#define IIC_SLAVE_SDA_FLOAT()        my_st(IIC_SLAVE_SDA_PULL = 0;)  //SDA�������룬��©���

#define IIC_SLAVE_SCL_UP()           my_st(IIC_SLAVE_SCL_PULL = 1;)  //SCL�������룬�������
#define IIC_SLAVE_SCL_FLOAT()        my_st(IIC_SLAVE_SCL_PULL = 0;)  //SCL�������룬��©���

#define IIC_SLAVE_SDA_IQ_ON()        my_st(IIC_SLAVE_SDA_IQ = 1;)  //SDA�ж����뿪���������
#define IIC_SLAVE_SDA_IQ_OFF()       my_st(IIC_SLAVE_SDA_IQ = 0;)  //SDA�ж�����أ��������

#define IIC_SLAVE_SCL_IQ_ON()        my_st(IIC_SLAVE_SCL_IQ = 1;)  //SCL�ж����뿪���������
#define IIC_SLAVE_SCL_IQ_OFF()       my_st(IIC_SLAVE_SCL_IQ = 0;)  //SCL�ж�����أ��������

#define IIC_SLAVE_SDA_OUT(a)         my_st(IIC_SLAVE_SDA_PIN_OUT = a;)  //SDA�����ƽ
#define IIC_SLAVE_SCL_OUT(a)         my_st(IIC_SLAVE_SCL_PIN_OUT = a;)

#define IIC_SLAVE_SDA_DIR_OUT()      my_st(IIC_SLAVE_SDA_DIR |= IIC_SLAVE_SDA_DIR_BV;)  //SDA���ģʽ
#define IIC_SLAVE_SDA_DIR_IN()       my_st(IIC_SLAVE_SDA_DIR &= ~(IIC_SLAVE_SDA_DIR_BV);)

#define IIC_SLAVE_SCL_DIR_OUT()      my_st(IIC_SLAVE_SCL_DIR |= IIC_SLAVE_SCL_DIR_BV;)
#define IIC_SLAVE_SCL_DIR_IN()       my_st(IIC_SLAVE_SCL_DIR &= ~(IIC_SLAVE_SCL_DIR_BV);)

#define IIC_SLAVE_SDA_EXTI_UP()      my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x01;)  //SDA�ж������ش���
#define IIC_SLAVE_SDA_EXTI_DOWN()    my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x02;)  //SDA�ж��½��ش���
#define IIC_SLAVE_SDA_EXTI_UPDOWN()  my_st(IIC_SLAVE_SDA_EXTI_EDGE = 0x03;)  //SDA�ж��������½��ش���
    
#define IIC_SLAVE_SCL_EXTI_UP()      my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x01;)  //SCL�ж������ش���
#define IIC_SLAVE_SCL_EXTI_DOWN()    my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x02;)  //SCL�ж��½��ش���
#define IIC_SLAVE_SCL_EXTI_UPDOWN()  my_st(IIC_SLAVE_SCL_EXTI_EDGE = 0x03;)  //SCL�ж��������½��ش���

typedef void (*iic_slave_be_written)(uint16_t reg_adr, uint8_t *data, uint16_t len);  //IIC�ӻ���д�ص�����������ָ���������ݵ�ָ���Ĵ���
typedef void (*iic_slave_be_read)(uint16_t reg_adr, uint8_t *data, uint16_t len);  //IIC�ӻ������ص�����������ָ����������

void IIC_Slave_Init(uint8_t port, iic_slave_be_written be_written_cb, iic_slave_be_read be_read_cb);  //IIC�ӻ���ʼ����ע���д�ص�����

#ifdef __cplusplus
}
#endif

#endif