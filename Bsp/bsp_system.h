#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

//#include "stm32f7xx_hal.h"
#include "iostm8s103F3.h"

//数据类型声明
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int uint16_t;
typedef int int16_t;  //8位机int为16,32位机int为32
typedef unsigned long int uint32_t;
typedef long int int32_t;

#define HSI_VALUE ((uint32_t)16000000)  //内部高速时钟，16MHz
#define LSI_VALUE ((uint32_t)128000)    //内部低速时钟，128KHz
#define HSE_VALUE ((uint32_t)24000000)  //外部高速时钟，24MHz

typedef enum
{
    CLK_SOURCE_HSI = (uint8_t)0xE1, /*!< Clock Source HSI. */
    CLK_SOURCE_LSI = (uint8_t)0xD2, /*!< Clock Source LSI. */
    CLK_SOURCE_HSE = (uint8_t)0xB4 /*!< Clock Source HSE. */
}CLK_Source_TypeDef;  //STM8 时钟源

typedef enum
{
	CLK_Source_Select_HSI = 0,  //内部高速时钟，16MHz
	CLK_Source_Select_LSI,      //内部低速时钟，128KHz
    CLK_Source_Select_HSE       //外部高速时钟，1-24MHz
}CLK_Source_Select_TypeDef;  //主时钟选择枚举体

//void MPU_Config(void);  //STM32
//void CPU_CACHE_Enable(void);  //STM32
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, uint8_t hsidiv, uint8_t cpudiv);  //System Clock Configuration，输入时钟选择，HSI分频系数，CPU分频系数
uint32_t CLK_GetMasterClock_Freq(void);  //主时钟频率获取，输出主频率

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line);
#endif

#endif
