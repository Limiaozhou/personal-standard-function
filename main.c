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

	/* Configure the system clock to 16 MHz */
//	CLK_SYSCLK_Config();  //�ڲ�����ʱ�ӣ�HSI��CPUʱ��0��Ƶ��fcpu = 16MHz
	
	Delay_Init(72);  //��ʱ������׼����
//    IIC_Simulation_Master_Init();  //��ʼ����ģ��IIC��
//    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_SLOW);
//	
//	Uart_Init(UART1_Select, 9600, uart1_read_deal);  //UART1������9600
//    Uart_Init(UART3_Select, 9600, pm_read_deal);  //UART3������9600
//    
//    init_Co2();
//    Init_Timer4();
//	IWDG_init();
//    
//    asm("rim");//��ȫ���жϣ�simΪ���ж�
//    
//	/* Infinite loop */
	while(1)
	{
		delay_ms(2000);
//        if(time_data_deal >= 1000)  //2048ms
//        {
//            data_deal();
//            GPIO_WriteReverse(GPIOE, GPIO_PIN_0);
//            time_data_deal = 0;
//        }
//        if(RST_GetFlagStatus(RST_FLAG_IWDGF))
//        {
//            uart_write(UART1_Select, "IWDGF_RST", 10);
//            RST_ClearFlag(RST_FLAG_IWDGF);
//        }
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
