#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "typedef.h"  //������������
//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intد16,32λ��intد32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

#define KEY_GPIO_LIST {\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA, .Mode = GPIO_Mode_IPU}\
}

typedef enum
{
	KEY0 = 0,
	number_of_key
}KEY_GPIO_Port;  //�˿ں�

typedef struct
{
	GPIO_TypeDef* GPIOx;
    uint32_t RCC_APB2Periph;  //����ʱ��ʹ��
	uint16_t Pin;
    GPIOMode_TypeDef Mode;  //���Ź���ģʽ
}KEY_GPIOType;  //���Žṹ��

void Key_GPIO_Init(void);
uint8_t Key_GPIO_Read(KEY_GPIO_Port keyx);

#endif
