#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

//#include "stm32f7xx_hal.h"

//void Delay_Init(unsigned char sysclk);  //STM32
void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);

#endif
