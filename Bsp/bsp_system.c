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

/* System Clock Configuration��clk_sel: ����ʱ��ѡ��hsidiv: HSI��Ƶϵ����cpudiv: CPU��Ƶϵ�� */
void CLK_SYSCLK_Config(CLK_Source_Select_TypeDef clk_sel, uint8_t hsidiv, uint8_t cpudiv)
{
    //STM8
    switch(clk_sel)
	{
		case CLK_Source_Select_HSE :  //�ⲿ����ʱ�ӣ�1-24MHz
            CLK_ECKR |= 0x01;  //HSE�����ⲿ��������ʹ�ܣ�1-24MHz
			while(!(CLK_ECKR & 0x02));  //�ȴ������ⲿ����׼������
			CLK_SWCR &= ~((1<<2)|(1<<3));  //ʱ���л��жϽ�ֹ������жϱ�־
			CLK_SWR = CLK_SOURCE_HSE;  //HSEΪ��ʱ��Դ
			while(CLK_SWCR & 0x01);  //�ȴ�ʱ���л�æ����
			CLK_SWCR |= 0x02;  //ʹ��ʱ���л�
			while(CLK_CMSR != CLK_SOURCE_HSE);  //�ȴ�ϵͳʱ���л���0xB4��HSE
			CLK_CKDIVR_HSIDIV = hsidiv;  //HSI��Ƶϵ����fmaster = fHSI / 2^hsidiv���˴���Ч
            CLK_CKDIVR_CPUDIV = cpudiv;  //CPU��Ƶϵ����fcpu = fmaster /2^cpudiv
			CLK_CCOR &= ~(1<<0);  //��ֹCCO���
            CLK_CSSR = 0x01;  //ʱ�Ӱ�ȫ���ʹ��
			CLK_ICKR &= ~0x01;  //HSI�����ڲ���������ʧ��
		break;
		
		case CLK_Source_Select_LSI :  //�ڲ�����ʱ�ӣ�128KHz��Ŀǰ���ã����ڲ�����ʱ�Ӵ���
//		break;
		
		case CLK_Source_Select_HSI :  //�ڲ�����ʱ�ӣ�16MHz
//        break;
        
		default :  //����ѡ��Χ�ڣ����ó��ڲ�����ʱ��
        
            CLK_ICKR |= 0x01;  //HSI�����ڲ�RC����ʹ�ܣ�16MHz
			while(!(CLK_ICKR & 0x02));  //�ȴ������ڲ�����׼������
			CLK_SWCR &= ~((1<<2)|(1<<3));  //ʱ���л��жϽ�ֹ������жϱ�־
			CLK_SWR = CLK_SOURCE_HSI;  //HSIΪ��ʱ��Դ(��λֵ)
			while(CLK_SWCR & 0x01);  //�ȴ�ʱ���л�æ����
			CLK_SWCR |= 0x02;  //ʹ��ʱ���л�
			while(CLK_CMSR != CLK_SOURCE_HSI);  //�ȴ�ϵͳʱ���л���0xE1��HSI
            CLK_CKDIVR_HSIDIV = hsidiv;  //HSI��Ƶϵ����fmaster = fHSI / 2^hsidiv
            CLK_CKDIVR_CPUDIV = cpudiv;  //CPU��Ƶϵ����fcpu = fmaster /2^cpudiv
			CLK_CCOR &= ~(1<<0);  //��ֹCCO���
            CLK_CSSR = 0x01;  //ʱ�Ӱ�ȫ���ʹ��
			CLK_ECKR &= ~0x01;  //HSE�����ⲿ��������ʧ��
            
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
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  //ʱ��ԴΪHSE
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;  //��HSE
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;  //��PLL
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  //PLLʱ��Դѡ��HSE
//   RCC_OscInitStruct.PLL.PLLM = 25;  //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ)
//   RCC_OscInitStruct.PLL.PLLN = 432;  //��PLL��Ƶϵ��(PLL��Ƶ)
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
//   RCC_OscInitStruct.PLL.PLLQ = 9;  //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
//   RCC_OscInitStruct.PLL.PLLR = 7;
//  
//   ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);  //��ʼ��
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
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  //����ϵͳʱ��ʱ��ԴΪPLL
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;  //AHB��Ƶϵ��Ϊ1
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  //APB1��Ƶϵ��Ϊ4
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  //APB2��Ƶϵ��Ϊ2
//  
//   ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);  //ͬʱ����FLASH��ʱ����Ϊ7WS��Ҳ����8��CPU����
//   if(ret != HAL_OK)
//   {
//     Error_Handler();
//   }
}

//��ʱ��Ƶ�ʻ�ȡ�������Ƶ�ʣ�����CPUƵ�ʣ�CPUƵ�� = ��Ƶ�� / 2^CPU��Ƶϵ��
uint32_t CLK_GetMasterClock_Freq(void)
{
    //STM8
    CLK_Source_TypeDef clock_source = CLK_SOURCE_HSI;  //ʱ��Դ����
    uint32_t clock_frequency = 0;  //��ʱ��Ƶ�ʻ���
    
    clock_source = (CLK_Source_TypeDef)CLK_CMSR;  //��ȡʱ��Դ�Ĵ�������ȡʱ��Դ
    
    switch(clock_source)
    {
        case CLK_SOURCE_HSI :  //�ڲ�����ʱ�ӣ�16MHz
            clock_frequency = HSI_VALUE / (0x01 << CLK_CKDIVR_HSIDIV);  //��2�ķ�Ƶϵ������
        break;
        
        case CLK_SOURCE_LSI :  //�ڲ�����ʱ�ӣ�128KHz
            clock_frequency = LSI_VALUE;
        break;
        
        case CLK_SOURCE_HSE :  //�ⲿ����ʱ�ӣ�24MHz
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
