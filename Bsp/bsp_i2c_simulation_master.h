#ifndef __BSP_I2C_SIMULATION_MASTER_H
#define __BSP_I2C_SIMULATION_MASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"
#include "delay.h"

#elif defined STM8
#include "stm8s_gpio.h"
#include "delay.h"
#endif

//#include "typedef.h"

//������������
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

#define DELAY_US_I2C  5  //I2CЭ���ƽ�任��ʱus

typedef enum
{
	I2C_SimPort0 = 0,
    number_of_i2c_simulation
}I2C_Simulaton_Port;  //ģ��I2C�˿�

//ģ��I2C�˿��б�
#if defined (STM32_STANDARD) || defined (STM8)
#if defined STM32_STANDARD
#define I2C_GPIO_LIST {\
	{\
		.GPIO_scl = GPIOA,\
		.Pin_scl = GPIO_Pin_4,\
		.GPIO_sda = GPIOA,\
		.Pin_sda = GPIO_Pin_5\
	},\
}

typedef struct
{
    GPIO_TypeDef* GPIO_scl;
	GPIO_TypeDef* GPIO_sda;
    uint32_t RCC_APB2Periph_scl;
    uint32_t RCC_APB2Periph_sda;
    uint16_t Pin_scl;
    uint16_t Pin_sda;
}I2C_Simulation_GPIO_TypeDef;  //ģ��I2C�˿���Ϣ

#elif defined STM8
#define I2C_GPIO_LIST {\
	{\
		.GPIO_scl = GPIOE,\
		.Pin_scl = GPIO_PIN_6,\
		.GPIO_sda = GPIOE,\
		.Pin_sda = GPIO_PIN_7\
	},\
	{\
		.GPIO_scl = GPIOB,\
		.Pin_scl = GPIO_PIN_5,\
		.GPIO_sda = GPIOB,\
		.Pin_sda = GPIO_PIN_4\
	},\
    {\
		.GPIO_scl = GPIOB,\
		.Pin_scl = GPIO_PIN_6,\
		.GPIO_sda = GPIOB,\
		.Pin_sda = GPIO_PIN_7\
	},\
    {\
		.GPIO_scl = GPIOE,\
		.Pin_scl = GPIO_PIN_1,\
		.GPIO_sda = GPIOE,\
		.Pin_sda = GPIO_PIN_2\
	},\
}

typedef struct
{
    GPIO_TypeDef* GPIO_scl;
    GPIO_Pin_TypeDef Pin_scl;
	GPIO_TypeDef* GPIO_sda;
    GPIO_Pin_TypeDef Pin_sda;
}I2C_Simulation_GPIO_TypeDef;
#endif

typedef enum
{
	GPIO_Low_Level = 0,  //�͵�ƽ
	GPIO_High_Level = !GPIO_Low_Level
}GPIO_OutLevel_TypeDef;  //IO�����ƽ

typedef enum
{
	GPIO_In = 0,  //����
	GPIO_Out = !GPIO_In
}GPIO_Direction_TypeDef;  //IO����/���
#endif

typedef struct
{
	uint8_t port;  //I2C�˿�
	uint16_t device_adr;  //I2C�ӻ��豸��ַ
    uint8_t dev_adr_tenbit_flag;  //I2C�ӻ��豸10λ��ַģʽ��־λ
	uint8_t * data;    //I2C��д����ָ��
	uint32_t len;  //I2C��д���ݳ���
    uint8_t error_resend_times;  //ʧ���ط�����
}I2C_Master_WRInfo_TypeDef;  //I2C������д��Ϣ

typedef I2C_Master_WRInfo_TypeDef* pI2C_Master_WRInfo_TypeDef;  //I2C������д��Ϣָ��

typedef enum
{
	Delay_No = 0,  //����ʱ
	Delay_Front,  //��֮ǰ��ʱ
	Delay_Back  //��֮����ʱ
}Delay_ReadReg_TypeDef;  //���Ĵ�����ʱ

typedef struct
{
	I2C_Master_WRInfo_TypeDef wr_info;  //I2C������д��Ϣ
	uint16_t register_adr;  //I2C�ӻ��Ĵ�����ַ
	uint8_t reg_adr_twobyte_flag;  //I2C�ӻ�2�ֽڼĴ�����ַ��־λ
	Delay_ReadReg_TypeDef readreg_delay_flag;  //I2C���Ĵ������Ͷ��ź�ǰ����ʱ��־
	uint32_t readreg_delay_nms;  //I2C���Ĵ������Ͷ��ź�ǰ����ʱʱ�� ms
}I2C_Master_ReadReg_Info_TypeDef;  //I2C�������Ĵ�����Ϣ

typedef I2C_Master_ReadReg_Info_TypeDef* pI2C_Master_ReadReg_Info_TypeDef;  //I2C�������Ĵ�����Ϣָ��

void I2C_Simulation_Master_Init(void);  //ģ��I2C��ʼ��

uint8_t I2C_Simulation_Master_Write(pI2C_Master_WRInfo_TypeDef piic);  //д�豸���ɹ�����0��ʧ�ܷ���1
uint8_t I2C_Simulation_Master_ReadDirect(pI2C_Master_WRInfo_TypeDef piic);  //���豸���ɹ�����0��ʧ�ܷ���1

uint8_t I2C_Simulation_Master_ReadRegister(pI2C_Master_ReadReg_Info_TypeDef piic);  //���豸�Ĵ������ɹ�����0��ʧ�ܷ���1

#ifdef __cplusplus
}
#endif

#endif