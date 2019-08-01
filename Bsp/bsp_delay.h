#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

//#include "stm32f7xx_hal.h"

void Delay_Init(unsigned char sysclk);  //延时基准初始化
void delay_us(unsigned int nus);  //延时n us
void delay_ms(unsigned int nms);  //延时n ms

#endif
