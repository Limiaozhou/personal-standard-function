#include "bsp_system.h"

//static void Error_Handler(void);  //STM32

/**
  * @brief  STM32 Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
//void MPU_Config(void)
//{
//  MPU_Region_InitTypeDef MPU_InitStruct;
//  
//  /* Disable the MPU */
//  HAL_MPU_Disable();
//
//  /* Configure the MPU attributes as WT for SRAM */
//  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//  MPU_InitStruct.BaseAddress = 0x20010000;
//  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
//  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
//  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
//  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
//  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//  MPU_InitStruct.SubRegionDisable = 0x00;
//  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//
//  HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
//  /* Enable the MPU */
//  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
//}

/**
  * @brief  STM32 CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
//void CPU_CACHE_Enable(void)
//{
//	/* Enable I-Cache */
//	SCB_EnableICache();
//
//	/* Enable D-Cache */
//	SCB_EnableDCache();
//	
//	SCB->CACR |= 1<<2;
//}

/* System Clock Configuration，clk_sel: 输入时钟选择，hsidiv: HSI分频系数，cpudiv: CPU分频系数 */
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, uint8_t hsidiv, uint8_t cpudiv)
{
    //STM8
    switch(clk_sel)
	{
		case CLK_Source_Select_HSE :  //外部高速时钟，1-24MHz
            CLK_ECKR |= 0x01;  //HSE高速外部晶体振荡器使能，1-24MHz
			while(!(CLK_ECKR & 0x02));  //等待高速外部振荡器准备就绪
			CLK_SWCR &= ~((1<<2)|(1<<3));  //时钟切换中断禁止并清除中断标志
			CLK_SWR = CLK_SOURCE_HSE;  //HSE为主时钟源
			while(CLK_SWCR & 0x01);  //等待时钟切换忙结束
			CLK_SWCR |= 0x02;  //使能时钟切换
			while(CLK_CMSR != CLK_SOURCE_HSE);  //等待系统时钟切换到0xB4：HSE
			CLK_CKDIVR_HSIDIV = hsidiv;  //HSI分频系数，fmaster = fHSI / 2^hsidiv，此处无效
            CLK_CKDIVR_CPUDIV = cpudiv;  //CPU分频系数，fcpu = fmaster /2^cpudiv
			CLK_CCOR &= ~(1<<0);  //禁止CCO输出
            CLK_CSSR = 0x01;  //时钟安全监测使能
			CLK_ICKR &= ~0x01;  //HSI高速内部晶体振荡器失能
		break;
		
		case CLK_Source_Select_LSI :  //内部低速时钟，128KHz，目前不用，当内部高速时钟处理
//		break;
		
		case CLK_Source_Select_HSI :  //内部高速时钟，16MHz
//        break;
        
		default :  //不在选择范围内，配置成内部高速时钟
        
            CLK_ICKR |= 0x01;  //HSI高速内部RC振荡器使能，16MHz
			while(!(CLK_ICKR & 0x02));  //等待高速内部振荡器准备就绪
			CLK_SWCR &= ~((1<<2)|(1<<3));  //时钟切换中断禁止并清除中断标志
			CLK_SWR = CLK_SOURCE_HSI;  //HSI为主时钟源(复位值)
			while(CLK_SWCR & 0x01);  //等待时钟切换忙结束
			CLK_SWCR |= 0x02;  //使能时钟切换
			while(CLK_CMSR != CLK_SOURCE_HSI);  //等待系统时钟切换到0xE1：HSI
            CLK_CKDIVR_HSIDIV = hsidiv;  //HSI分频系数，fmaster = fHSI / 2^hsidiv
            CLK_CKDIVR_CPUDIV = cpudiv;  //CPU分频系数，fcpu = fmaster /2^cpudiv
			CLK_CCOR &= ~(1<<0);  //禁止CCO输出
            CLK_CSSR = 0x01;  //时钟安全监测使能
			CLK_ECKR &= ~0x01;  //HSE高速外部晶体振荡器失能
            
		break;
	}
	
	//STM32
//   RCC_ClkInitTypeDef RCC_ClkInitStruct;
//   RCC_OscInitTypeDef RCC_OscInitStruct;
//   HAL_StatusTypeDef ret = HAL_OK;
//  
//   Enable Power Control clock
//   __HAL_RCC_PWR_CLK_ENABLE();
//  
//   The voltage scaling allows optimizing the power consumption when the device is 
//      clocked below the maximum system frequency, to update the voltage scaling value 
//      regarding system frequency refer to product datasheet. 
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  
//
//   Enable HSE Oscillator and activate PLL with HSE as source 
//	 PLLCLK = HSE / M * N / P = 25M / 25 *432 / 2 = 216M
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  //时钟源为HSE
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;  //打开HSE
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;  //打开PLL
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  //PLL时钟源选择HSE
//   RCC_OscInitStruct.PLL.PLLM = 25;  //主PLL和音频PLL分频系数(PLL之前的分频)
//   RCC_OscInitStruct.PLL.PLLN = 432;  //主PLL倍频系数(PLL倍频)
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  //系统时钟的主PLL分频系数(PLL之后的分频)
//   RCC_OscInitStruct.PLL.PLLQ = 9;  //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
//   RCC_OscInitStruct.PLL.PLLR = 7;
//  
//   ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);  //初始化
//   if(ret != HAL_OK)
//   {
//     Error_Handler();
//   }
//  
//   Activate the OverDrive to reach the 216 MHz Frequency  
//   ret = HAL_PWREx_EnableOverDrive();
//   if(ret != HAL_OK)
//   {
//     Error_Handler();
//   }
//  
//   Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
//   RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  //设置系统时钟时钟源为PLL
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;  //AHB分频系数为1
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  //APB1分频系数为4
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  //APB2分频系数为2
//  
//   ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);  //同时设置FLASH延时周期为7WS，也就是8个CPU周期
//   if(ret != HAL_OK)
//   {
//     Error_Handler();
//   }
}

//主时钟频率获取，输出主频率，不是CPU频率，CPU频率 = 主频率 / 2^CPU分频系数
uint32_t CLK_GetMasterClock_Freq(void)
{
    //STM8
    CLK_Source_TypeDef clock_source = CLK_SOURCE_HSI;  //时钟源缓存
    uint32_t clock_frequency = 0;  //主时钟频率缓存
    
    clock_source = (CLK_Source_TypeDef)CLK_CMSR;  //读取时钟源寄存器，获取时钟源
    
    switch(clock_source)
    {
        case CLK_SOURCE_HSI :  //内部高速时钟，16MHz
            clock_frequency = HSI_VALUE / (0x01 << CLK_CKDIVR_HSIDIV);  //除2的分频系数次幂
        break;
        
        case CLK_SOURCE_LSI :  //内部低速时钟，128KHz
            clock_frequency = LSI_VALUE;
        break;
        
        case CLK_SOURCE_HSE :  //外部高速时钟，24MHz
            clock_frequency = HSE_VALUE;
        break;
        
        default :
            clock_frequency = HSI_VALUE / (0x01 << CLK_CKDIVR_HSIDIV);
        break;
    }
    
    return clock_frequency;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
//static void Error_Handler(void)
//{
//  /* User may add here some code to deal with this error */
//  while(1)
//  {
//  }
//}

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
