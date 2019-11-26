#ifndef __BSP_IWDG_H
#define __BSP_IWDG_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_iwdg.h"

#elif defined STM8
#endif

//#include "typedef.h"

//������������
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

void IWDG_Init(uint8_t IWDG_Prescaler, uint16_t Reload);
void IWDG_Feed(void);

#endif
