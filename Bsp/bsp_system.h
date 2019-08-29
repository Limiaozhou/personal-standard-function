#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

//#include "stm32f7xx_hal.h"
//#include "iostm8s103F3.h"
//#include "iostm8s207c8.h"
//#include "iostm8l151k4.h"
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

// #define STM8S  //�����Ϊʹ�üĴ������Z
//#define STM8L

#ifdef STM8S
    #define HSI_VALUE ((uint32_t)16000000)  //�ڲ���?ʱ�ӣ�16MHz
    #define LSI_VALUE ((uint32_t)128000)    //�ڲ���?ʱ�ӣ�128KHz
    #define HSE_VALUE ((uint32_t)24000000)  //�ⲿ��?ʱ�ӣ�24MHz(1-24)
    
    typedef enum
    {
        CLK_SOURCE_HSI = (uint8_t)0xE1, /*!< Clock Source HSI. */
        CLK_SOURCE_LSI = (uint8_t)0xD2, /*!< Clock Source LSI. */
        CLK_SOURCE_HSE = (uint8_t)0xB4 /*!< Clock Source HSE. */
    }CLK_Source_TypeDef;  //STM8 ʱ�Ӝ�
    
    typedef enum
    {
        CLK_Source_Select_HSI = 0,  //�ڲ���?ʱ��
        CLK_Source_Select_LSI,      //�ڲ���?ʱ��
        CLK_Source_Select_HSE       //�ⲿ��?ʱ��
    }CLK_Source_Select_TypeDef;  //��ʱ��?��ö��ʹ
    
#elif defined STM8L
    #define HSI_VALUE ((uint32_t)16000000)  //�ڲ���?ʱ�ӣ�16MHz
    #define LSI_VALUE ((uint32_t)38000)     //�ڲ���?ʱ�ӣ�38KHz
    #define HSE_VALUE ((uint32_t)16000000)  //�ⲿ��?ʱ�ӣ�16MHz(1-16)
    #define LSE_VALUE ((uint32_t)32768)     //�ⲿ��?ʱ�ӣ�32.768Hz
    
    typedef enum
    {
        CLK_SOURCE_HSI = (uint8_t)0x01, /*!< Clock Source HSI. */
        CLK_SOURCE_LSI = (uint8_t)0x02, /*!< Clock Source LSI. */
        CLK_SOURCE_HSE = (uint8_t)0x04, /*!< Clock Source HSE. */
        CLK_SOURCE_LSE = (uint8_t)0x08  /*!< Clock Source LSE. */
    }CLK_Source_TypeDef;  //STM8 ʱ�Ӝ�
    
    typedef enum
    {
        CLK_Source_Select_HSI = 0,  //�ڲ���?ʱ��
        CLK_Source_Select_LSI,      //�ڲ���?ʱ��
        CLK_Source_Select_HSE,      //�ⲿ��?ʱ��
        CLK_Source_Select_LSE       //�ⲿ��?ʱ��
    }CLK_Source_Select_TypeDef;  //��ʱ��?��ö��ʹ
    
#endif

//void MPU_Config(void);  //STM32
//void CPU_CACHE_Enable(void);  //STM32

#if defined (STM8S) || defined (STM8L)
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, uint8_t hsidiv, uint8_t cpudiv);  //System Clock Configuration������ʱ��?��HSI��Ƶϵ����CPU��Ƶϵ��
uint32_t CLK_GetMasterClock_Freq(void);  //��ʱ��Ƶ�ʻ�ȡ�������Ƶ�x
#else
void CLK_SYSCLK_Config(void);
#endif

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line);
#endif

#endif
