#ifndef __BSP_LED_H
#define __BSP_LED_H

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

#define LED_NUM 2  //LED最大数量
#define Led(x) x  //led号

typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_TOGGLE  //翻转
}LED_GPIO_OutState;  //LED输出状态

typedef enum
{
	LED0 = 0,
	LED1,
    LED2,
	number_of_led
}LED_GPIO_Port;  //LED端口

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint8_t active_level;  //有效电平
}LED_GPIOType;  //LED引脚结构体

void Led_GPIO_Init(void);
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state);

#endif
