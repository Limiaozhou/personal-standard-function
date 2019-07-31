#ifndef __BSP_TIME_H
#define __BSP_TIME_H

//#include "stm32f7xx_hal.h"
#include "iostm8s103F3.h"

#define TIME_NUM 20  //计时任务最大数量

typedef enum
{
	TIM1_Select = 1,
	TIM2_Select,
    TIM3_Select,
    TIM4_Select,
    TIM5_Select,
    TIM6_Select
}TIMx_Select_TypeDef;  //定时器选择枚举体

//STM32
//#define TIM_COMMON               TIM3
//#define TIM_COMMON_CLK_ENABLE()  __TIM3_CLK_ENABLE()
//
//#define TIM_COMMON_IRQn          TIM3_IRQn
//#define TIM_COMMON_IRQHandler    TIM3_IRQHandler

typedef struct
{
	unsigned char t_start;  //计时开关
//	unsigned long int t_old;  //之前时间
//	unsigned long int t_now;  //目前时间
	unsigned long int * t_cnt;  //计时时间
}TIME_CountType;  //定时器计时结构体

void TIMx_Init(TIMx_Select_TypeDef timex, unsigned long int prescaler, unsigned long int period);

unsigned char time_register(unsigned long int * data);
void time_start(unsigned char numx);
void time_clean(unsigned char numx);

#endif
