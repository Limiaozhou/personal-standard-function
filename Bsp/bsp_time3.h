#ifndef __BSP_TIME3_H
#define __BSP_TIME3_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_tim.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

void TIM3_Init(uint16_t prescaler, uint16_t period, void (*time3_task)(void *));
uint32_t get_tim3_ticks(void);

#endif
