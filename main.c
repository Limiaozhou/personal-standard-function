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
void time_task6(void);
void time_task7(void);
void time_task8(void);

void uart_task1(uint8_t * pdata, uint32_t len);
void uart_task2(uint8_t * pdata, uint32_t len);

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
    timer_task_start(5, 5, 1, time_task6);
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

void time_task6(void)
{
    if(!Key_GPIO_Read(KEY1))
        Led_GPIO_Write(LED_BLUE, LED_ON);
    else
        Led_GPIO_Write(LED_BLUE, LED_OFF);
}

void time_task7(void)
{
    uart_read(Uart1, uart_task2);
}

void time_task8(void)
{
    static uint8_t buf[15] = "0123456789aBcD";
    uart_write_dma(Uart1, buf, 14);
}

void uart_task1(uint8_t * pdata, uint32_t len)
{
    uart_write_dma(Uart1, pdata, len);
//    uart_write(Uart1, pdata, len);
}

void uart_task2(uint8_t * pdata, uint32_t len)
{
    uint32_t i;
    uint8_t * pbuf= (uint8_t*)malloc( len * sizeof(uint8_t) );
    
    if(!pbuf)
        return;
    
    for(i = 0; i < len; ++i)
    {
        pbuf[i] = pdata[len - 1 -i];
    }
    uart_write_dma(Uart1, pbuf, len);
//    uart_write(Uart1, pbuf, len);
    
    free(pbuf);
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
    Key_GPIO_Init();
    TIM3_Init(719, 99, Timer_Update);  //720 * 100 / 72000000 = 0.001s = 1ms
    Uart_Init(Uart1, 115200, 20, 20);
    Uart_PriorityTask_Regist(Uart1, uart_task1);
    
    timer_task_start(2000, 2000, 0, time_task1);
    timer_task_start(1000, 1000, 1, time_task2);
    timer_task_start(20000, 0, 1, time_task3);
//    timer_task_start(100, 100, 0, time_task7);
//    timer_task_start(5, 5, 1, time_task5);
//    timer_task_start(1000, 1000, 1, time_task8);
    
	/* Infinite loop */
	while(1)
	{
//        time_ticks = get_tim3_ticks();
//        timers_adjust(&time_ticks);
        timeout_task_loop();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
