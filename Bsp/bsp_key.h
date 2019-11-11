#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "typedef.h"  //数据类型声明
//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int丿16,32位机int丿32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

#define KEY_GPIO_LIST {\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA, .Mode = GPIO_Mode_IPU}\
}

typedef enum
{
	KEY0 = 0,
	number_of_key
}KEY_GPIO_Port;  //端口号

typedef struct
{
	GPIO_TypeDef* GPIOx;
    uint32_t RCC_APB2Periph;  //外设时钟使能
	uint16_t Pin;
    GPIOMode_TypeDef Mode;  //引脚工作模式
}KEY_GPIOType;  //引脚结构体

void Key_GPIO_Init(void);
uint8_t Key_GPIO_Read(KEY_GPIO_Port keyx);

#endif
