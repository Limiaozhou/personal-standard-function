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
	CLK_SYSCLK_Config();  //�ڲ�����ʱ�ӣ�HSI��CPUʱ��0��Ƶ��fcpu = 16MHz
	
	Delay_Init(16);  //��ʱ������׼����
//	Led_GPIO_Init();  //led��������
//	Key_GPIO_Init();  //������������
	IIC_Master_Init(1);
	// TIMx_Init(TIM2_Select, 4, 1000-1);  //����ͨ�ö�ʱ��������1000us��1ms
	
	// Uart_Init(UART1_Select, 9600);  //UART1������9600
    
    // uart1_send_ontime_int();  //UART1��ʱ���ͳ�ʼ����ע�ᶨʱ��
	
    asm("rim");//��ȫ���жϣ�simΪ���ж�
    
	/* Infinite loop */
	while (1)
	{
		delay_ms(1000);
//		key_scan(key_task);
        // uart1_send_ontime();  //UART1��ʱ����
		// uart_read(UART1_Select, uart1_read_deal);
		light_data = light_read();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
