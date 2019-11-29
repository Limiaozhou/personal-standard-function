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
void ledblue_twinkle(void);
void led3_twinkle(void);
void key0_control_led3(void);
void key1_control_ledgreen(void);
void uart1_write_with_ledred(void);
void uart1_send(void);
void uart1_read(void);
void uart1_read_deal(uint8_t * pdata, uint32_t len);
void printf_test(void);
void uart2_write(void);
void uart2_send(void);
void uart2_read(void);
void uart2_read_deal(uint8_t * pdata, uint32_t len);
void uart3_write(void);
void uart3_send(void);
void uart3_read(void);
void uart3_read_deal(uint8_t * pdata, uint32_t len);
void uart4_write(void);
void uart4_send(void);
void uart4_read(void);
void uart4_read_deal(uint8_t * pdata, uint32_t len);
void uart5_write(void);
void uart5_send(void);
void uart5_read(void);
void uart5_read_deal(uint8_t * pdata, uint32_t len);

/* Private functions ---------------------------------------------------------*/
void ledblue_twinkle(void)
{
    Led_GPIO_Write(LED_BLUE, LED_TOGGLE);
}

void led3_twinkle(void)
{
    Led_GPIO_Write(LED3, LED_TOGGLE);
}

void key0_control_led3(void)
{
    if(!Key_GPIO_Read(KEY0))
        Led_GPIO_Write(LED3, LED_ON);
    else
        Led_GPIO_Write(LED3, LED_OFF);
}

void key1_control_ledgreen(void)
{
    if(!Key_GPIO_Read(KEY1))
        Led_GPIO_Write(LED_GREEN, LED_ON);
    else
        Led_GPIO_Write(LED_GREEN, LED_OFF);
}

void uart1_write_with_ledred(void)
{
    uint8_t buf[] = "01234567890";
    uart_write( Uart1, buf, sizeof(buf) );
    Led_GPIO_Write(LED_RED, LED_TOGGLE);
}

void uart1_send(void)
{
    uart_send_loop(Uart1);
}

void uart1_read(void)
{
    uart_read(Uart1, uart1_read_deal);
}

void uart1_read_deal(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart1, pdata, len);
}

void printf_test(void)
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

void uart2_write(void)
{
    uint8_t buf[] = "01234567890";
    uart_write( Uart2, buf, sizeof(buf) );
}

void uart2_send(void)
{
    uart_send_loop(Uart2);
}

void uart2_read(void)
{
    uart_read(Uart2, uart2_read_deal);
}

void uart2_read_deal(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart2, pdata, len);
}

void uart3_write(void)
{
    uint8_t buf[] = "01234567890";
    uart_write( Uart3, buf, sizeof(buf) );
}

void uart3_send(void)
{
    uart_send_loop(Uart3);
}

void uart3_read(void)
{
    uart_read(Uart3, uart3_read_deal);
}

void uart3_read_deal(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart3, pdata, len);
}

void uart4_write(void)
{
    uint8_t buf[] = "01234567890";
    uart_write( Uart4, buf, sizeof(buf) );
}

void uart4_send(void)
{
    uart_send_loop(Uart4);
}

void uart4_read(void)
{
    uart_read(Uart4, uart4_read_deal);
}

void uart4_read_deal(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart4, pdata, len);
}

void uart5_write(void)
{
    uint8_t buf[] = "01234567890";
    uart_write( Uart5, buf, sizeof(buf) );
}

void uart5_send(void)
{
    uart_send_loop(Uart5);
}

void uart5_read(void)
{
    uart_read(Uart5, uart5_read_deal);
}

void uart5_read_deal(uint8_t * pdata, uint32_t len)
{
    uart_write(Uart5, pdata, len);
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
    Uart_Init(Uart1, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_PriorityTask_Regist(Uart1, uart1_read_deal);
    Uart_Init(Uart2, 115200, 50, 50, UartTx_Interrupt_Sel);
    Uart_Init(Uart3, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart4, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart5, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_PriorityTask_Regist(Uart5, uart5_read_deal);
    IWDG_Init(IWDG_Prescaler_64, 1000);  //1.6s溢出
    
    timer_task_start(100, 0, 0, printf_test);
    timer_task_start(1000, 1000, 0, IWDG_Feed);
    timer_task_start(1000, 1000, 0, ledblue_twinkle);
    timer_task_start(1000, 1000, 0, led3_twinkle);
//    timer_task_start(5, 5, 0, key0_control_led3);
    timer_task_start(5, 5, 0, key1_control_ledgreen);
    timer_task_start(2000, 2000, 0, uart1_write_with_ledred);
    timer_task_start(2000, 2000, 0, uart1_send);
    timer_task_start(2000, 2000, 0, uart1_read);
    timer_task_start(2000, 2000, 0, uart2_write);
    timer_task_start(2000, 2000, 0, uart2_send);
    timer_task_start(2000, 2000, 0, uart2_read);
    timer_task_start(1000, 1000, 0, uart3_send);
    timer_task_start(2000, 2000, 0, uart3_write);
    timer_task_start(1000, 1000, 0, uart3_read);
//    timer_task_start(2000, 2000, 0, uart4_write);
//    timer_task_start(2000, 2000, 0, uart4_send);
//    timer_task_start(2000, 2000, 0, uart4_read);
//    timer_task_start(2000, 2000, 0, uart5_write);
//    timer_task_start(2000, 2000, 0, uart5_send);
//    timer_task_start(2000, 2000, 0, uart5_read);
    
	/* Infinite loop */
	while(1)
	{
//        time_ticks = get_tim3_ticks();
//        timers_adjust(&time_ticks);
        timeout_task_loop();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
