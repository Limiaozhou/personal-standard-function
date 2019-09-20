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

#define IIC_SIMULATION_MASTER_NUM 1  //ģ��IIC������
#define DELAY_US_IIC  5  //IIC�����ʱus

typedef enum
{
	GPIO_Low_Level = 0,  //IO�͵�ƽ
	GPIO_High_Level
}GPIO_OutLevel_TypeDef;  //IO��ƽ

typedef enum
{
	GPIO_In = 0,  //IO����
	GPIO_Out
}GPIO_Direction_TypeDef;  //IO��ƽ

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