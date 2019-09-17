#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm8s_i2c.h"
#include "stm8s_clk.h"
#include "delay.h"

//#include "typedef.h"

//������������
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

typedef struct
{
	uint16_t device_adr;  //IIC�ӻ��豸��ַ
    uint8_t dev_adr_tenbit_flag;  //IIC�ӻ��豸10λ��ַģʽ��־λ
	uint8_t * data;    //IIC��д����ָ��
	uint32_t len;  //IIC��д���ݳ���
    uint8_t error_resend_times;  //ʧ���ط�����
}IIC_Hard_Master_WRInfo_TypeDef;  //IIC������д��Ϣ

typedef IIC_Hard_Master_WRInfo_TypeDef* pIIC_Hard_Master_WRInfo_TypeDef;  //IIC������д��Ϣָ��

typedef enum
{
	Delay_Hard_No = 0,  //����ʱ
	Delay_Hard_Front,  //��֮ǰ��ʱ
	Delay_Hard_Back  //��֮����ʱ
}Delay_Hard_ReadReg_TypeDef;  //���Ĵ�����ʱ

typedef struct
{
	IIC_Hard_Master_WRInfo_TypeDef wr_info;  //IIC������д��Ϣ
	uint16_t register_adr;  //IIC�ӻ��Ĵ�����ַ
	uint8_t reg_adr_twobyte_flag;  //IIC�ӻ�2�ֽڼĴ�����ַ��־λ
	Delay_Hard_ReadReg_TypeDef readreg_delay_flag;  //IIC���Ĵ������Ͷ��ź�ǰ����ʱ��־
	uint32_t readreg_delay_nms;  //IIC���Ĵ������Ͷ��ź�ǰ����ʱʱ�� ms
}IIC_Hard_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣ

typedef IIC_Hard_Master_ReadReg_Info_TypeDef* pIIC_Hard_Master_ReadReg_Info_TypeDef;  //IIC�������Ĵ�����Ϣָ��

typedef enum
{
	Write_Sel = 0,  //д
	ReadDirect_Sel,  //ֱ�Ӷ�
}IIC_WriteRead_Select_TypeDef;

typedef void (*piic_hard_read_data_deal)(uint8_t * buf, uint16_t len);  //Ӳ��IIC�����ݶ�ȡ������

//Ӳ��IIC��ʼ�����������ʱ�ӡ��������ӻ��ĵ�ַ����ַģʽ
void IIC_Hard_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, I2C_AddMode_TypeDef AddMode, piic_hard_read_data_deal pdeal);

//Ӳ��IIC��д��������������ָ�롢��дѡ�񣬳ɹ�����0��ʧ�ܣ�����æ������1
uint8_t IIC_Hard_Master_WriteRead_Start(pIIC_Hard_Master_WRInfo_TypeDef piic, IIC_WriteRead_Select_TypeDef sel);

#ifdef __cplusplus
}
#endif

#endif