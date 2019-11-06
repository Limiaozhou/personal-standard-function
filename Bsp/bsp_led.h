#ifndef __BSP_LED_H
#define __BSP_LED_H

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

#define LED_NUM 2  //LED�������
#define Led(x) x  //led��

typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_TOGGLE  //��ת
}LED_GPIO_OutState;  //LED���״̬

typedef enum
{
	LED0 = 0,
	LED1,
    LED2,
	number_of_led
}LED_GPIO_Port;  //LED�˿�

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint8_t active_level;  //��Ч��ƽ
}LED_GPIOType;  //LED���Žṹ��

void Led_GPIO_Init(void);
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state);

#endif
