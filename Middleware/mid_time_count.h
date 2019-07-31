#ifndef __MID_TIME_COUNT_H
#define __MID_TIME_COUNT_H

#include "stm32f7xx_hal.h"
#include "bsp_tim_common.h"

typedef struct
{
	uint32_t start;  //开始时间
	uint32_t now;  //现在时间
	uint64_t set;  //设置时间
	uint64_t count;  //计数时间
	
	uint8_t key_start;  //开始时间更新锁
	uint8_t key;  //定时器锁
}Time_GetTypeDef;  //定时器时间获取结构体

typedef Time_GetTypeDef* pTime_GetTypeDef;  //定时器时间获取结构体指针

#define TIME_READY   1
#define TIME_UNREADY 0

uint8_t Time_Count(pTime_GetTypeDef p);  //定时器计时函数

#endif
