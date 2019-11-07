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

#define LED_GPIO_LIST {\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_4, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_5, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_6, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOD, .Pin = GPIO_Pin_2, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOD}\
}

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
    LED3,
	number_of_led
}LED_GPIO_Port;  //LED�˿�

typedef struct
{
	GPIO_TypeDef* GPIOx;
    uint32_t RCC_APB2Periph;  //����ʱ��ʹ��
	uint16_t Pin;
	uint8_t active_level;  //��Ч��ƽ
}LED_GPIOType;  //LED���Žṹ��

void Led_GPIO_Init(void);
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state);
uint8_t Led_GPIO_Read(LED_GPIO_Port ledx);

#endif
