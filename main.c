/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//uint16_t time_data_deal = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Main program */
int main(void)
{
#if defined STM32_HAL
	/* STM32 Configure the MPU attributes as Write Through */
	MPU_Config();

	/* STM32 Enable the CPU Cache */
	CPU_CACHE_Enable();
	
	/* STM32F7xx HAL library initialization */
	HAL_Init();
#endif

	/* Configure the system clock */
	CLK_SYSCLK_Config();
	
#if defined STM32_STANDARD
    NVIC_PriorityGroupInit();
    SysTick_Init();
#endif
    
	Delay_Init(72);  //延时函数基准配置
    Led_GPIO_Init();
    TIM3_Init(7199, 9);  //7200 * 10 / 72000000 = 0.001s = 1ms
    
//	/* Infinite loop */
	while(1)
	{
        if((get_tim3_ticks() % 1000) == 0)
            Led_GPIO_Write(LED0, LED_TOGGLE);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
