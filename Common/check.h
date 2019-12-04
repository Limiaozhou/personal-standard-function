#ifndef __CHECK_H
#define __CHECK_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

//#include "typedef.h"  //数据类型重定义包含头文件

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;
// typedef unsigned long long int uint64_t;
// typedef signed long long int int64_t;

uint16_t check_sum16_calculate(void * pdata, uint16_t len);

uint16_t check_crc16_modbus_calc(void * pdata, uint16_t len);

#endif
