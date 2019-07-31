#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

//#include "stm32f7xx_hal.h"
#include "iostm8s103F3.h"

#define HSI_VALUE ((unsigned long int)16000000)  //内部高速时钟，16MHz
#define LSI_VALUE ((unsigned long int)128000)    //内部低速时钟，128KHz
#define HSE_VALUE ((unsigned long int)24000000)  //外部高速时钟，24MHz

typedef enum
{
    CLK_SOURCE_HSI = (unsigned char)0xE1, /*!< Clock Source HSI. */
    CLK_SOURCE_LSI = (unsigned char)0xD2, /*!< Clock Source LSI. */
    CLK_SOURCE_HSE = (unsigned char)0xB4 /*!< Clock Source HSE. */
}CLK_Source_TypeDef;  //STM8 时钟源

typedef enum
{
	CLK_Source_Select_HSI = 0,  //内部高速时钟，16MHz
	CLK_Source_Select_LSI,      //内部低速时钟，128KHz
    CLK_Source_Select_HSE       //外部高速时钟，1-24MHz
}CLK_Source_Select_TypeDef;  //主时钟选择枚举体

//void MPU_Config(void);  //STM32
//void CPU_CACHE_Enable(void);  //STM32
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, unsigned char hsidiv, unsigned char cpudiv);  //System Clock Configuration，输入时钟选择，HSI分频系数，CPU分频系数
unsigned long int CLK_GetMasterClock_Freq(void);  //主时钟频率获取，输出主频率

#ifdef  USE_FULL_ASSERT
void assert_failed(unsigned char * file, unsigned long int line);
#endif

#endif
