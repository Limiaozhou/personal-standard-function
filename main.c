/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void time_task1(void);
void time_task2(void);

/* Private functions ---------------------------------------------------------*/
void time_task1(void)
{
    Led_GPIO_Write(LED3, LED_TOGGLE);
}

void time_task2(void)
{
    timer_task_stop(time_task1);
}

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
    
	Delay_Init(72);  //��ʱ������׼����
    Led_GPIO_Init();
    TIM3_Init(719, 99);  //720 * 100 / 72000000 = 0.001s = 1ms
    timer_task_start(1000, 1000, time_task1);
    timer_task_start(10000, 0, time_task2);
    
//	/* Infinite loop */
	while(1)
	{
        timers_adjust();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
