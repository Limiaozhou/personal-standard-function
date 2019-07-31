#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f7xx_hal.h"

//#define STM32F7  //����stm32_hal_legacy.h--GPIO_SPEED_HIGH

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint32_t Pin;
	uint8_t Pull;
}LED_GPIOType;  //LED���Žṹ��

typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_TOGGLE
}LED_State;  //LED���״̬

#define LED_NUM 2  //LED�������
#define Led(x) x  //led��

#define LED0_PIN               GPIO_PIN_1  //LED0������غ궨��
#define LED0_GPIO              GPIOB
#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED1_PIN               GPIO_PIN_0
#define LED1_GPIO              GPIOB
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED_NOPULL 0  //GPIO_NOPULL=0
#define LED_UP     1  //GPIO_PULLUP=1
#define LED_DOWN   2  //GPIO_PULLUDOWN=2

void Led_GPIO_Init(void);  //LED�˿�����
void Led_Write(uint8_t ledx, LED_State state);  //дLED�˿�
uint8_t Led_Read(uint8_t ledx);  //��LED�˿�

#endif
