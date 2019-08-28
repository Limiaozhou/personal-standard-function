#ifndef __DELAY_H
#define __DELAY_H

//#include "stm32f7xx_hal.h"
#include "stm8s.h"

//#include "typedef.h"

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

void Delay_Init(uint8_t sysclk);  //延时基准初始化
void delay_us(uint16_t nus);  //延时n us
void delay_ms(uint16_t nms);  //延时n ms

#endif
