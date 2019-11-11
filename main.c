/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_BLUE  LED0
#define LED_GREEN LED1
#define LED_RED   LED2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint32_t time_ticks;

/* Private function prototypes -----------------------------------------------*/
void time_task1(void);
void time_task2(void);
void time_task3(void);
void time_task4(void);
void time_task5(void);

/* Private functions ---------------------------------------------------------*/
void time_task1(void)
{
    Led_GPIO_Write(LED_BLUE, LED_TOGGLE);
    delay_ms(1500);
}

void time_task2(void)
{
    Led_GPIO_Write(LED_GREEN, LED_TOGGLE);
}

void time_task3(void)
{
    timer_task_stop(time_task1);
    timer_task_stop(time_task2);
    timer_task_start(1000, 1000, 0, time_task4);
}

void time_task4(void)
{
    Led_GPIO_Write(LED_RED, LED_TOGGLE);
}

void time_task5(void)
{
    if(!Key_GPIO_Read(KEY0))
        Led_GPIO_Write(LED3, LED_ON);
    else
        Led_GPIO_Write(LED3, LED_OFF);
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
    
	Delay_Init(72);  //延时函数基准配置
    Led_GPIO_Init();
    TIM3_Init(719, 99, Timer_Update);  //720 * 100 / 72000000 = 0.001s = 1ms
//    timer_task_start(2000, 2000, 0, time_task1);
//    timer_task_start(1000, 1000, 1, time_task2);
//    timer_task_start(20000, 0, 1, time_task3);
    timer_task_start(5, 5, 0, time_task5);
    
//	/* Infinite loop */
	while(1)
	{
//        time_ticks = get_tim3_ticks();
//        timers_adjust(&time_ticks);
        timeout_task_loop();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
