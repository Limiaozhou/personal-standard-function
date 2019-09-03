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

//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef struct
{
	uint8_t port;  //IIC�˿�
	uint16_t device_adr;  //IIC�ӻ��豸��ַ
    uint8_t dev_adr_tenbit_flag;  //IIC�ӻ��豸10λ��ַģʽ��־λ
	uint8_t * data;    //IIC��д����ָ��
	uint32_t len;  //IIC��д���ݳ���
    uint8_t error_resend_times;  //ʧ���ط�����
}IIC_Master_WRInfo_TypeDef;  //IIC������д��Ϣ

typedef IIC_Master_WRInfo_TypeDef* pIIC_Master_WRInfo_TypeDef;  //IIC������д��Ϣָ��

typedef struct
{
	uint8_t port;  //IIC�˿�
	uint16_t device_adr;  //IIC�ӻ��豸��ַ
    uint8_t dev_adr_tenbit_flag;  //IIC�ӻ��豸10λ��ַģʽ��־λ
	uint16_t register_adr;  //IIC�ӻ��Ĵ�����ַ
	uint8_t reg_adr_twobyte_flag;  //IIC�ӻ�2�ֽڼĴ�����ַ��־λ
	uint8_t readreg_delay_flag;  //IIC���Ĵ������Ͷ��źź��Ƿ���ʱ��־
	uint32_t readreg_delay_nms;  //IIC���Ĵ������Ͷ��źź���ʱʱ�� ms
	uint8_t * data;    //IIC��д����ָ��
	uint32_t len;  //IIC��д���ݳ���
    uint8_t error_resend_times;  //ʧ���ط�����
}IIC_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣ

typedef IIC_Master_ReadReg_Info_TypeDef* pIIC_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣָ��

#define HIGH 1  //���������ƽ-��
#define LOW  0

#define OUT 1  //�����������
#define IN  0

#define DELAY_US_IIC  5  //IIC�����ʱus

#define my_st(x)  do { x } while(0)  //�������
#define my_BV(n)  (1 << (n))  //ȡnλ

//IIC�������üĴ�������
//port = 1��IIC��1
#define IIC_MASTER_SDA_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_7))  //������SDA��������
#define IIC_MASTER_SCL_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_6))  //ʱ����SCL

#define IIC_MASTER_SDA_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_7;)  //SDA�������룬�������
#define IIC_MASTER_SDA_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_7);)  //SDA�������룬��©���

#define IIC_MASTER_SCL_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_6;)  //SCL�������룬�������
#define IIC_MASTER_SCL_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_6);)  //SCL�������룬��©���

#define IIC_MASTER_SDA_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_7;)  //SDA�ж����뿪���������
#define IIC_MASTER_SDA_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_7);)  //SDA�ж�����أ��������

#define IIC_MASTER_SCL_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_6;)  //SCL�ж����뿪���������
#define IIC_MASTER_SCL_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_6);)  //SCL�ж�����أ��������

#define IIC_MASTER_SDA_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_7; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_7);)  //SDA�����ƽ
#define IIC_MASTER_SCL_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_6; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_6);)

#define IIC_MASTER_SDA_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_7;)  //SDA���ģʽ
#define IIC_MASTER_SDA_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_7);)

#define IIC_MASTER_SCL_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_6;)
#define IIC_MASTER_SCL_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_6);)

// #define IIC_MASTER_SDA_CR1       PB_CR1_C15  //������SDA���ƼĴ���1
// #define IIC_MASTER_SCL_CR1       PB_CR1_C14  //ʱ����SCL

// #define IIC_MASTER_SDA_CR2       PB_CR2_C25  //������SDA���ƼĴ���2
// #define IIC_MASTER_SCL_CR2       PB_CR2_C24  //ʱ����SCL

// #define IIC_MASTER_SDA_PIN_OUT   PB_ODR_ODR5  //������SDA�������
// #define IIC_MASTER_SCL_PIN_OUT   PB_ODR_ODR4  //ʱ����SCL

// #define IIC_MASTER_SDA_PIN_IN    PB_IDR_IDR5  //������SDA��������
// #define IIC_MASTER_SCL_PIN_IN    PB_IDR_IDR4  //ʱ����SCL

// #define IIC_MASTER_SDA_DIR       PB_DDR  //SDA����/���ѡ��Ĵ���
// #define IIC_MASTER_SCL_DIR       PB_DDR

// #define IIC_MASTER_SDA_DIR_BV    my_BV(5)  //SDA����/����Ĵ���λѡ��
// #define IIC_MASTER_SCL_DIR_BV    my_BV(4)

// #define IIC_MASTER_SDA_UP()      my_st(IIC_MASTER_SDA_CR1 = 1;)  //SDA�������룬�������
// #define IIC_MASTER_SDA_FLOAT()   my_st(IIC_MASTER_SDA_CR1 = 0;)  //SDA�������룬��©���

// #define IIC_MASTER_SCL_UP()      my_st(IIC_MASTER_SCL_CR1 = 1;)  //SCL�������룬�������
// #define IIC_MASTER_SCL_FLOAT()   my_st(IIC_MASTER_SCL_CR1 = 0;)  //SCL�������룬��©���

// #define IIC_MASTER_SDA_IQ_ON()   my_st(IIC_MASTER_SDA_CR2 = 1;)  //SDA�ж����뿪���������
// #define IIC_MASTER_SDA_IQ_OFF()  my_st(IIC_MASTER_SDA_CR2 = 0;)  //SDA�ж�����أ��������

// #define IIC_MASTER_SCL_IQ_ON()   my_st(IIC_MASTER_SCL_CR2 = 1;)  //SCL�ж����뿪���������
// #define IIC_MASTER_SCL_IQ_OFF()  my_st(IIC_MASTER_SCL_CR2 = 0;)  //SCL�ж�����أ��������

// #define IIC_MASTER_SDA_OUT(a)    my_st(IIC_MASTER_SDA_PIN_OUT = a;)  //SDA�����ƽ
// #define IIC_MASTER_SCL_OUT(a)    my_st(IIC_MASTER_SCL_PIN_OUT = a;)

// #define IIC_MASTER_SDA_DIR_OUT() my_st(IIC_MASTER_SDA_DIR |= IIC_MASTER_SDA_DIR_BV;)  //SDA���ģʽ
// #define IIC_MASTER_SDA_DIR_IN()  my_st(IIC_MASTER_SDA_DIR &= ~(IIC_MASTER_SDA_DIR_BV);)

// #define IIC_MASTER_SCL_DIR_OUT() my_st(IIC_MASTER_SCL_DIR |= IIC_MASTER_SCL_DIR_BV;)
// #define IIC_MASTER_SCL_DIR_IN()  my_st(IIC_MASTER_SCL_DIR &= ~(IIC_MASTER_SCL_DIR_BV);)


//port = 2
#define IIC_MASTER2_SDA_PIN_IN    (!!(GPIOB->IDR & GPIO_PIN_4))  //������SDA��������
#define IIC_MASTER2_SCL_PIN_IN    (!!(GPIOB->IDR & GPIO_PIN_5))  //ʱ����SCL

#define IIC_MASTER2_SDA_UP()      my_st(GPIOB->CR1 |= (uint8_t)GPIO_PIN_4;)  //SDA�������룬�������
#define IIC_MASTER2_SDA_FLOAT()   my_st(GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_4);)  //SDA�������룬��©���

#define IIC_MASTER2_SCL_UP()      my_st(GPIOB->CR1 |= (uint8_t)GPIO_PIN_5;)  //SCL�������룬�������
#define IIC_MASTER2_SCL_FLOAT()   my_st(GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_5);)  //SCL�������룬��©���

#define IIC_MASTER2_SDA_IQ_ON()   my_st(GPIOB->CR2 |= (uint8_t)GPIO_PIN_4;)  //SDA�ж����뿪���������
#define IIC_MASTER2_SDA_IQ_OFF()  my_st(GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_4);)  //SDA�ж�����أ��������

#define IIC_MASTER2_SCL_IQ_ON()   my_st(GPIOB->CR2 |= (uint8_t)GPIO_PIN_5;)  //SCL�ж����뿪���������
#define IIC_MASTER2_SCL_IQ_OFF()  my_st(GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_5);)  //SCL�ж�����أ��������

#define IIC_MASTER2_SDA_OUT(a)    my_st(if(a) GPIOB->ODR |= (uint8_t)GPIO_PIN_4; else GPIOB->ODR &= (uint8_t)(~GPIO_PIN_4);)  //SDA�����ƽ
#define IIC_MASTER2_SCL_OUT(a)    my_st(if(a) GPIOB->ODR |= (uint8_t)GPIO_PIN_5; else GPIOB->ODR &= (uint8_t)(~GPIO_PIN_5);)

#define IIC_MASTER2_SDA_DIR_OUT() my_st(GPIOB->DDR |= (uint8_t)GPIO_PIN_4;)  //SDA���ģʽ
#define IIC_MASTER2_SDA_DIR_IN()  my_st(GPIOB->DDR &= (uint8_t)(~GPIO_PIN_4);)

#define IIC_MASTER2_SCL_DIR_OUT() my_st(GPIOB->DDR |= (uint8_t)GPIO_PIN_5;)
#define IIC_MASTER2_SCL_DIR_IN()  my_st(GPIOB->DDR &= (uint8_t)(~GPIO_PIN_5);)

// #define IIC_MASTER2_SDA_CR1       PD_CR1_C13  //������SDA���ƼĴ���1
// #define IIC_MASTER2_SCL_CR1       PD_CR1_C12  //ʱ����SCL

// #define IIC_MASTER2_SDA_CR2       PD_CR2_C23  //������SDA���ƼĴ���2
// #define IIC_MASTER2_SCL_CR2       PD_CR2_C22  //ʱ����SCL

// #define IIC_MASTER2_SDA_PIN_OUT   PD_ODR_ODR3  //������SDA�������
// #define IIC_MASTER2_SCL_PIN_OUT   PD_ODR_ODR2  //ʱ����SCL

// #define IIC_MASTER2_SDA_PIN_IN    PD_IDR_IDR3  //������SDA��������
// #define IIC_MASTER2_SCL_PIN_IN    PD_IDR_IDR2  //ʱ����SCL

// #define IIC_MASTER2_SDA_DIR       PD_DDR  //SDA����/���ѡ��Ĵ���
// #define IIC_MASTER2_SCL_DIR       PD_DDR

// #define IIC_MASTER2_SDA_DIR_BV    my_BV(3)  //SDA����/����Ĵ���λѡ��
// #define IIC_MASTER2_SCL_DIR_BV    my_BV(2)

// #define IIC_MASTER2_SDA_UP()      my_st(IIC_MASTER2_SDA_CR1 = 1;)  //SDA�������룬�������
// #define IIC_MASTER2_SDA_FLOAT()   my_st(IIC_MASTER2_SDA_CR1 = 0;)  //SDA�������룬��©���

// #define IIC_MASTER2_SCL_UP()      my_st(IIC_MASTER2_SCL_CR1 = 1;)  //SCL�������룬�������
// #define IIC_MASTER2_SCL_FLOAT()   my_st(IIC_MASTER2_SCL_CR1 = 0;)  //SCL�������룬��©���

// #define IIC_MASTER2_SDA_IQ_ON()   my_st(IIC_MASTER2_SDA_CR2 = 1;)  //SDA�ж����뿪���������
// #define IIC_MASTER2_SDA_IQ_OFF()  my_st(IIC_MASTER2_SDA_CR2 = 0;)  //SDA�ж�����أ��������

// #define IIC_MASTER2_SCL_IQ_ON()   my_st(IIC_MASTER2_SCL_CR2 = 1;)  //SCL�ж����뿪���������
// #define IIC_MASTER2_SCL_IQ_OFF()  my_st(IIC_MASTER2_SCL_CR2 = 0;)  //SCL�ж�����أ��������

// #define IIC_MASTER2_SDA_OUT(a)    my_st(IIC_MASTER2_SDA_PIN_OUT = a;)  //SDA�����ƽ
// #define IIC_MASTER2_SCL_OUT(a)    my_st(IIC_MASTER2_SCL_PIN_OUT = a;)

// #define IIC_MASTER2_SDA_DIR_OUT() my_st(IIC_MASTER2_SDA_DIR |= IIC_MASTER2_SDA_DIR_BV;)  //SDA���ģʽ
// #define IIC_MASTER2_SDA_DIR_IN()  my_st(IIC_MASTER2_SDA_DIR &= ~(IIC_MASTER2_SDA_DIR_BV);)

// #define IIC_MASTER2_SCL_DIR_OUT() my_st(IIC_MASTER2_SCL_DIR |= IIC_MASTER2_SCL_DIR_BV;)
// #define IIC_MASTER2_SCL_DIR_IN()  my_st(IIC_MASTER2_SCL_DIR &= ~(IIC_MASTER2_SCL_DIR_BV);)


//port = 3
#define IIC_MASTER3_SDA_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_7))  //������SDA��������
#define IIC_MASTER3_SCL_PIN_IN    (!!(GPIOE->IDR & GPIO_PIN_6))  //ʱ����SCL

#define IIC_MASTER3_SDA_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_7;)  //SDA�������룬�������
#define IIC_MASTER3_SDA_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_7);)  //SDA�������룬��©���

#define IIC_MASTER3_SCL_UP()      my_st(GPIOE->CR1 |= (uint8_t)GPIO_PIN_6;)  //SCL�������룬�������
#define IIC_MASTER3_SCL_FLOAT()   my_st(GPIOE->CR1 &= (uint8_t)(~GPIO_PIN_6);)  //SCL�������룬��©���

#define IIC_MASTER3_SDA_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_7;)  //SDA�ж����뿪���������
#define IIC_MASTER3_SDA_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_7);)  //SDA�ж�����أ��������

#define IIC_MASTER3_SCL_IQ_ON()   my_st(GPIOE->CR2 |= (uint8_t)GPIO_PIN_6;)  //SCL�ж����뿪���������
#define IIC_MASTER3_SCL_IQ_OFF()  my_st(GPIOE->CR2 &= (uint8_t)(~GPIO_PIN_6);)  //SCL�ж�����أ��������

#define IIC_MASTER3_SDA_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_7; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_7);)  //SDA�����ƽ
#define IIC_MASTER3_SCL_OUT(a)    my_st(if(a) GPIOE->ODR |= (uint8_t)GPIO_PIN_6; else GPIOE->ODR &= (uint8_t)(~GPIO_PIN_6);)

#define IIC_MASTER3_SDA_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_7;)  //SDA���ģʽ
#define IIC_MASTER3_SDA_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_7);)

#define IIC_MASTER3_SCL_DIR_OUT() my_st(GPIOE->DDR |= (uint8_t)GPIO_PIN_6;)
#define IIC_MASTER3_SCL_DIR_IN()  my_st(GPIOE->DDR &= (uint8_t)(~GPIO_PIN_6);)

// #define IIC_MASTER3_SDA_CR1       PC_CR1_C17  //������SDA���ƼĴ���1
// #define IIC_MASTER3_SCL_CR1       PC_CR1_C16  //ʱ����SCL

// #define IIC_MASTER3_SDA_CR2       PC_CR2_C27  //������SDA���ƼĴ���2
// #define IIC_MASTER3_SCL_CR2       PC_CR2_C26  //ʱ����SCL

// #define IIC_MASTER3_SDA_PIN_OUT   PC_ODR_ODR7  //������SDA�������
// #define IIC_MASTER3_SCL_PIN_OUT   PC_ODR_ODR6  //ʱ����SCL

// #define IIC_MASTER3_SDA_PIN_IN    PC_IDR_IDR7  //������SDA��������
// #define IIC_MASTER3_SCL_PIN_IN    PC_IDR_IDR6  //ʱ����SCL

// #define IIC_MASTER3_SDA_DIR       PC_DDR  //SDA����/���ѡ��Ĵ���
// #define IIC_MASTER3_SCL_DIR       PC_DDR

// #define IIC_MASTER3_SDA_DIR_BV    my_BV(7)  //SDA����/����Ĵ���λѡ��
// #define IIC_MASTER3_SCL_DIR_BV    my_BV(6)

// #define IIC_MASTER3_SDA_UP()      my_st(IIC_MASTER3_SDA_CR1 = 1;)  //SDA�������룬�������
// #define IIC_MASTER3_SDA_FLOAT()   my_st(IIC_MASTER3_SDA_CR1 = 0;)  //SDA�������룬��©���

// #define IIC_MASTER3_SCL_UP()      my_st(IIC_MASTER3_SCL_CR1 = 1;)  //SCL�������룬�������
// #define IIC_MASTER3_SCL_FLOAT()   my_st(IIC_MASTER3_SCL_CR1 = 0;)  //SCL�������룬��©���

// #define IIC_MASTER3_SDA_IQ_ON()   my_st(IIC_MASTER3_SDA_CR2 = 1;)  //SDA�ж����뿪���������
// #define IIC_MASTER3_SDA_IQ_OFF()  my_st(IIC_MASTER3_SDA_CR2 = 0;)  //SDA�ж�����أ��������

// #define IIC_MASTER3_SCL_IQ_ON()   my_st(IIC_MASTER3_SCL_CR2 = 1;)  //SCL�ж����뿪���������
// #define IIC_MASTER3_SCL_IQ_OFF()  my_st(IIC_MASTER3_SCL_CR2 = 0;)  //SCL�ж�����أ��������

// #define IIC_MASTER3_SDA_OUT(a)    my_st(IIC_MASTER3_SDA_PIN_OUT = a;)  //SDA�����ƽ
// #define IIC_MASTER3_SCL_OUT(a)    my_st(IIC_MASTER3_SCL_PIN_OUT = a;)

// #define IIC_MASTER3_SDA_DIR_OUT() my_st(IIC_MASTER3_SDA_DIR |= IIC_MASTER3_SDA_DIR_BV;)  //SDA���ģʽ
// #define IIC_MASTER3_SDA_DIR_IN()  my_st(IIC_MASTER3_SDA_DIR &= ~(IIC_MASTER3_SDA_DIR_BV);)

// #define IIC_MASTER3_SCL_DIR_OUT() my_st(IIC_MASTER3_SCL_DIR |= IIC_MASTER3_SCL_DIR_BV;)
// #define IIC_MASTER3_SCL_DIR_IN()  my_st(IIC_MASTER3_SCL_DIR &= ~(IIC_MASTER3_SCL_DIR_BV);)

void IIC_Master_Init(uint8_t port);  //��ʼ��ѡ���IIC�ڣ�Ϊ����״̬

uint8_t IIC_Master_Write(pIIC_Master_WRInfo_TypeDef piic);  //д�豸���ɹ�����0��ʧ�ܷ���1
uint8_t IIC_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic);  //���豸���ɹ�����0��ʧ�ܷ���1

uint8_t IIC_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic);  //���豸�Ĵ������ɹ�����0��ʧ�ܷ���1

#ifdef __cplusplus
}
#endif

#endif