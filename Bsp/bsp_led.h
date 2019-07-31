#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f7xx_hal.h"

//#define STM32F7  //开启stm32_hal_legacy.h--GPIO_SPEED_HIGH

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint32_t Pin;
	uint8_t Pull;
}LED_GPIOType;  //LED引脚结构体

typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_TOGGLE
}LED_State;  //LED输出状态

#define LED_NUM 2  //LED最大数量
#define Led(x) x  //led号

#define LED0_PIN               GPIO_PIN_1  //LED0引脚相关宏定义
#define LED0_GPIO              GPIOB
#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED1_PIN               GPIO_PIN_0
#define LED1_GPIO              GPIOB
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED_NOPULL 0  //GPIO_NOPULL=0
#define LED_UP     1  //GPIO_PULLUP=1
#define LED_DOWN   2  //GPIO_PULLUDOWN=2

void Led_GPIO_Init(void);  //LED端口配置
void Led_Write(uint8_t ledx, LED_State state);  //写LED端口
uint8_t Led_Read(uint8_t ledx);  //读LED端口

#endif
