#ifndef __BSP_IIC_SIMULATION_MASTER_H
#define __BSP_IIC_SIMULATION_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm8s_gpio.h"
#include "delay.h"

//#include "typedef.h"

//������������
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

#define IIC_SIMULATION_MASTER_NUM 4  //ģ��IIC������
#define DELAY_US_IIC  5  //IICЭ���ƽ�任��ʱus

//ģ��IIC�˿��б�
#define IIC_GPIO_LIST {\
	{\
		.scl_port = GPIOE,\
		.scl_pin = GPIO_PIN_6,\
		.sda_port = GPIOE,\
		.sda_pin = GPIO_PIN_7\
	},\
	{\
		.scl_port = GPIOB,\
		.scl_pin = GPIO_PIN_5,\
		.sda_port = GPIOB,\
		.sda_pin = GPIO_PIN_4\
	},\
    {\
		.scl_port = GPIOB,\
		.scl_pin = GPIO_PIN_6,\
		.sda_port = GPIOB,\
		.sda_pin = GPIO_PIN_7\
	},\
    {\
		.scl_port = GPIOE,\
		.scl_pin = GPIO_PIN_1,\
		.sda_port = GPIOE,\
		.sda_pin = GPIO_PIN_2\
	},\
}

typedef enum
{
	Pe6Pe7 = 0,  //ģ��IIC�˿�0
	Pb5Pb4,
    Pb6Pb7,
    Pe1Pe2
}IIC_Simulaton_Port_TypeDef;  //ģ��IIC�˿�����

typedef enum
{
	GPIO_Low_Level = 0,  //IO�͵�ƽ
	GPIO_High_Level = !GPIO_Low_Level
}GPIO_OutLevel_TypeDef;  //IO��ƽ

typedef enum
{
	GPIO_In = 0,  //IO����
	GPIO_Out = !GPIO_In
}GPIO_Direction_TypeDef;  //IO����/���

typedef struct
{
    GPIO_TypeDef* scl_port;
    GPIO_Pin_TypeDef scl_pin;
	GPIO_TypeDef* sda_port;
    GPIO_Pin_TypeDef sda_pin;
}IIC_Simulation_GPIO_TypeDef;  //ģ��IIC�˿���Ϣ

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

typedef enum
{
	Delay_No = 0,  //����ʱ
	Delay_Front,  //��֮ǰ��ʱ
	Delay_Back  //��֮����ʱ
}Delay_ReadReg_TypeDef;  //���Ĵ�����ʱ

typedef struct
{
	IIC_Master_WRInfo_TypeDef wr_info;  //IIC������д��Ϣ
	uint16_t register_adr;  //IIC�ӻ��Ĵ�����ַ
	uint8_t reg_adr_twobyte_flag;  //IIC�ӻ�2�ֽڼĴ�����ַ��־λ
	Delay_ReadReg_TypeDef readreg_delay_flag;  //IIC���Ĵ������Ͷ��ź�ǰ����ʱ��־
	uint32_t readreg_delay_nms;  //IIC���Ĵ������Ͷ��ź�ǰ����ʱʱ�� ms
}IIC_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣ

typedef IIC_Master_ReadReg_Info_TypeDef* pIIC_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣָ��

void IIC_Simulation_Master_Init(void);  //ģ��IIC��ʼ��

uint8_t IIC_Simulation_Master_Write(pIIC_Master_WRInfo_TypeDef piic);  //д�豸���ɹ�����0��ʧ�ܷ���1
uint8_t IIC_Simulation_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic);  //���豸���ɹ�����0��ʧ�ܷ���1

uint8_t IIC_Simulation_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic);  //���豸�Ĵ������ɹ�����0��ʧ�ܷ���1

#ifdef __cplusplus
}
#endif

#endif