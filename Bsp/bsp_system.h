#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

//#include "stm32f7xx_hal.h"
#include "stm8s.h"  //ʹ�ÿ⺯�����ܰ���"iostm8s103F3.h"
#include "stm8s_clk.h"

//#include "typedef.h"  //������������

//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intد16,32λ��intد32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

//void MPU_Config(void);  //STM32
//void CPU_CACHE_Enable(void);  //STM32

void CLK_SYSCLK_Config(void);

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line);
#endif

#endif
