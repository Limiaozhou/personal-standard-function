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
	/* STM32 Configure the MPU attributes as Write Through */
//	MPU_Config();

	/* STM32 Enable the CPU Cache */
//	CPU_CACHE_Enable();
	
	/* STM32F7xx HAL library initialization */
//	HAL_Init();

	/* Configure the system clock to 16 MHz */
	CLK_SYSCLK_Config();  //内部高速时钟，HSI和CPU时钟0分频，fcpu = 16MHz
	
	Delay_Init(16);  //延时函数基准配置
    IIC_Master_Init(1);
    IIC_Master_Init(2);
    IIC_Master_Init(3);
	
	Uart_Init(UART1_Select, 9600, uart1_read_deal);  //UART1波特率9600
    Uart_Init(UART3_Select, 9600, uart3_read_deal);  //UART3波特率9600
    
    IIC_Hard_Init(100000, 0, I2C_ADDMODE_7BIT, tvoc_data_deal);
    
    init_Co2();
    Init_Timer4();
//    I2C_MASTERMODE_Init(STANDARDSPEED);
	IWDG_init();
    
    asm("rim");//开全局中断，sim为关中断
    
	/* Infinite loop */
	while (1)
	{
		delay_ms(2000);
		data_deal();
        if(RST_GetFlagStatus(RST_FLAG_IWDGF))
        {
            uart_write(UART1_Select, "IWDGF_RST", 10);
            RST_ClearFlag(RST_FLAG_IWDGF);
        }
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
