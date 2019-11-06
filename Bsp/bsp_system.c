#include "bsp_system.h"

#if defined STM32_HAL
static void Error_Handler(void);  //STM32
#endif

#if defined STM32_HAL
/**
  * @brief  STM32 Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;
    
    /* Disable the MPU */
    HAL_MPU_Disable();
    
    /* Configure the MPU attributes as WT for SRAM */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x20010000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    
    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  STM32 CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();
    
    /* Enable D-Cache */
    SCB_EnableDCache();
    
    SCB->CACR |= 1<<2;
}

/**
  * @brief  STM32 This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* User may add here some code to deal with this error */
    while(1)
    {
    }
}
#endif

void CLK_SYSCLK_Config(void)
{
#if defined STM32_HAL
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;
    
    //    Enable Power Control clock
    __HAL_RCC_PWR_CLK_ENABLE();
    
    //    The voltage scaling allows optimizing the power consumption when the device is 
    //      clocked below the maximum system frequency, to update the voltage scaling value 
    //      regarding system frequency refer to product datasheet. 
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  
    
    //    Enable HSE Oscillator and activate PLL with HSE as source 
    //     PLLCLK = HSE / M * N / P = 25M / 25 *432 / 2 = 216M
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  //时钟源为HSE
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;  //打开HSE
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;  //打开PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  //PLL时钟源选择HSE
    RCC_OscInitStruct.PLL.PLLM = 25;  //主PLL和音频PLL分频系数(PLL之前的分频)
    RCC_OscInitStruct.PLL.PLLN = 432;  //主PLL倍频系数(PLL倍频)
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  //系统时钟的主PLL分频系数(PLL之后的分频)
    RCC_OscInitStruct.PLL.PLLQ = 9;  //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
    RCC_OscInitStruct.PLL.PLLR = 7;
    
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);  //初始化
    if(ret != HAL_OK)
    {
        Error_Handler();
    }
    
    //    Activate the OverDrive to reach the 216 MHz Frequency  
    ret = HAL_PWREx_EnableOverDrive();
    if(ret != HAL_OK)
    {
        Error_Handler();
    }
    
    //    Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  //设置系统时钟时钟源为PLL
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;  //AHB分频系数为1
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  //APB1分频系数为4
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  //APB2分频系数为2
    
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);  //同时设置FLASH延时周期为7WS，也就是8个CPU周期
    if(ret != HAL_OK)
    {
        Error_Handler();
    }
    
#elif defined STM32_STANDARD
    //系统时钟已初始化为外部高速-72MHz，其他外设时钟没打开
    
#elif defined STM8
	CLK_DeInit();  //复位时钟配置，HSI为8分频，2MHz
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  //改HSI为1分频，16MHz
#endif
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
    while (1)
    {
    }
}
#endif
