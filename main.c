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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Main program */
int main(void)
{
	float light_data = 0;
	/* STM32 Configure the MPU attributes as Write Through */
//	MPU_Config();

	/* STM32 Enable the CPU Cache */
//	CPU_CACHE_Enable();
	
	/* STM32F7xx HAL library initialization */
//	HAL_Init();

	/* Configure the system clock to 16 MHz */
	CLK_SYSCLK_Config();  //内部高速时钟，HSI和CPU时钟0分频，fcpu = 16MHz
	
	Delay_Init(16);  //延时函数基准配置
//	Led_GPIO_Init();  //led引脚配置
//	Key_GPIO_Init();  //按键引脚配置
	IIC_Master_Init(1);
	// TIMx_Init(TIM2_Select, 4, 1000-1);  //配置通用定时器，周期1000us，1ms
	
	// Uart_Init(UART1_Select, 9600);  //UART1波特率9600
    
    // uart1_send_ontime_int();  //UART1定时发送初始化，注册定时器
	
    asm("rim");//开全局中断，sim为关中断
    
	/* Infinite loop */
	while (1)
	{
		delay_ms(1000);
//		key_scan(key_task);
        // uart1_send_ontime();  //UART1定时发送
		// uart_read(UART1_Select, uart1_read_deal);
		light_data = light_read();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
