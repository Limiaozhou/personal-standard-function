#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

//#include "stm32f7xx_hal.h"
//#include "iostm8s103F3.h"
//#include "iostm8s207c8.h"
//#include "iostm8l151k4.h"
#include "stm8s.h"  //使用库函数不能包�?"iostm8s103F3.h"�?
#include "stm8s_clk.h"


//#include "typedef.h"  //数据类型声明

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int�?16,32位机int�?32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

// #define STM8S  //定义后为使用寄存器配�?
//#define STM8L

#ifdef STM8S
    #define HSI_VALUE ((uint32_t)16000000)  //内部高�?�时钟，16MHz
    #define LSI_VALUE ((uint32_t)128000)    //内部低�?�时钟，128KHz
    #define HSE_VALUE ((uint32_t)24000000)  //外部高�?�时钟，24MHz(1-24)
    
    typedef enum
    {
        CLK_SOURCE_HSI = (uint8_t)0xE1, /*!< Clock Source HSI. */
        CLK_SOURCE_LSI = (uint8_t)0xD2, /*!< Clock Source LSI. */
        CLK_SOURCE_HSE = (uint8_t)0xB4 /*!< Clock Source HSE. */
    }CLK_Source_TypeDef;  //STM8 时钟�?
    
    typedef enum
    {
        CLK_Source_Select_HSI = 0,  //内部高�?�时�?
        CLK_Source_Select_LSI,      //内部低�?�时�?
        CLK_Source_Select_HSE       //外部高�?�时�?
    }CLK_Source_Select_TypeDef;  //主时钟�?�择枚举�?
    
#elif defined STM8L
    #define HSI_VALUE ((uint32_t)16000000)  //内部高�?�时钟，16MHz
    #define LSI_VALUE ((uint32_t)38000)     //内部低�?�时钟，38KHz
    #define HSE_VALUE ((uint32_t)16000000)  //外部高�?�时钟，16MHz(1-16)
    #define LSE_VALUE ((uint32_t)32768)     //外部低�?�时钟，32.768Hz
    
    typedef enum
    {
        CLK_SOURCE_HSI = (uint8_t)0x01, /*!< Clock Source HSI. */
        CLK_SOURCE_LSI = (uint8_t)0x02, /*!< Clock Source LSI. */
        CLK_SOURCE_HSE = (uint8_t)0x04, /*!< Clock Source HSE. */
        CLK_SOURCE_LSE = (uint8_t)0x08  /*!< Clock Source LSE. */
    }CLK_Source_TypeDef;  //STM8 时钟�?
    
    typedef enum
    {
        CLK_Source_Select_HSI = 0,  //内部高�?�时�?
        CLK_Source_Select_LSI,      //内部低�?�时�?
        CLK_Source_Select_HSE,      //外部高�?�时�?
        CLK_Source_Select_LSE       //外部低�?�时�?
    }CLK_Source_Select_TypeDef;  //主时钟�?�择枚举�?
    
#endif

//void MPU_Config(void);  //STM32
//void CPU_CACHE_Enable(void);  //STM32

#if defined (STM8S) || defined (STM8L)
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, uint8_t hsidiv, uint8_t cpudiv);  //System Clock Configuration，输入时钟�?�择，HSI分频系数，CPU分频系数
uint32_t CLK_GetMasterClock_Freq(void);  //主时钟频率获取，输出主频�?
#else
void CLK_SYSCLK_Config(void);
#endif

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line);
#endif

#endif
