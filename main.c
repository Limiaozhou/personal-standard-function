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
void time_task9(void);
void time_task10(void);
void time_task11(void);
void time_task12(void);
void time_task13(void);
void time_task14(void);
void time_task15(void);

void uart_task1(uint8_t * pdata, uint32_t len);
void uart_task2(uint8_t * pdata, uint32_t len);
void uart_task3(uint8_t * pdata, uint32_t len);

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
        Led_GPIO_Write(LED_GREEN, LED_ON);
    else
        Led_GPIO_Write(LED_GREEN, LED_OFF);
}

void time_task7(void)
{
    Led_GPIO_Write(LED3, LED_TOGGLE);
}

void time_task8(void)
{
    uart_read(Uart1, uart_task1);
}

void time_task9(void)
{
    uart_send_loop(Uart1);
}

void time_task10(void)
{
    uint8_t buf[] = "01234567890";
    uart_write(Uart1, buf, 11);
    Led_GPIO_Write(LED_BLUE, LED_TOGGLE);
}

void time_task11(void)
{
    IWDG_Feed();
}

void time_task12(void)
{
    printf("Uart_ConfigType size : %d byte\r\n", sizeof(Uart_ConfigType));
    printf("Uart_DMAType size : %d byte\r\n", sizeof(Uart_DMAType));
    printf("Uart_GPIOType size : %d byte\r\n", sizeof(Uart_GPIOType));
    printf("Uart_UartType size : %d byte\r\n", sizeof(Uart_UartType));
    printf("Uart_NVICType size : %d byte\r\n", sizeof(Uart_NVICType));
    printf("Uart_PortInfo size : %d byte\r\n", sizeof(Uart_PortInfo));
    printf("UartTx_Mode_SelType size : %d byte\r\n", sizeof(UartTx_Mode_SelType));
    printf("Current parameters value: file %s on line %d\r\n", (uint8_t *)__FILE__, __LINE__);
}

void time_task13(void)
{
    uart_read(Uart2, uart_task3);
}

void time_task14(void)
{
    uart_send_loop(Uart2);
}

void time_task15(void)
{
    uint8_t buf[] = "abcdefghijk";
    uart_write(Uart2, buf, 11);
}

void uart_task1(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart1, pdata, len);
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
    uart_write(Uart1, pbuf, len);
    
    free(pbuf);
}

void uart_task3(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart2, pdata, len);
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
    Uart_Init(Uart1, 115200, 20, 20, UartTx_Interrupt_Sel);
//    Uart_PriorityTask_Regist(Uart1, uart_task1);
    Uart_Init(Uart2, 115200, 20, 20, UartTx_Interrupt_Sel);
    IWDG_Init(IWDG_Prescaler_64, 1000);  //1.6s溢出
    
//    timer_task_start(2000, 2000, 0, time_task1);  //闪蓝灯
    timer_task_start(1000, 1000, 1, time_task2);  //闪绿灯
    timer_task_start(20000, 0, 1, time_task3);  //关蓝绿灯闪烁任务，闪红灯，检测按键开关蓝灯
//    timer_task_start(5, 5, 1, time_task5);  //按键检测开关LED3
    timer_task_start(1000, 1000, 0, time_task7);  //闪LED3
    timer_task_start(1000, 1000, 1, time_task8);  //读串口1缓存并执行
    timer_task_start(2000, 2000, 0, time_task10);  //串口1写循环，闪蓝灯
    timer_task_start(1000, 1000, 1, time_task9);  //串口1发送循环
    timer_task_start(1000, 1000, 0, time_task11);  //独立看门狗复位
//    timer_task_start(2000, 2000, 0, time_task12);  //printf
    timer_task_start(1000, 1000, 1, time_task13);  //读串口2缓存并执行
    timer_task_start(2000, 2000, 0, time_task14);  //串口2写循环
    timer_task_start(1000, 1000, 1, time_task15);  //串口2发送循环
    
	/* Infinite loop */
	while(1)
	{
//        time_ticks = get_tim3_ticks();
//        timers_adjust(&time_ticks);
        timeout_task_loop();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
