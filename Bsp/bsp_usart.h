#ifndef __BSP_USART_H
#define __BSP_USART_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_usart.h"

#elif defined STM8
#include "stm8s_uart1.h"
#include "stm8s_uart3.h"
#include "stm8s_it.h"
#endif

#include "queue.h"

//#include "typedef.h"

//数据类型声明
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8位机int为16,32位机int为32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

//是否使用环形队列，要包含"queue.h"
#define QUEUE_USED 0

typedef enum
{
	Uart1 = 0,
	number_of_uart
}Uart_Port;  //uart端口

typedef enum
{
	UartTx_Block_Sel = 0,  //阻塞发送，死循环直到发送完
	UartTx_Timing_Sel,  //定时发送，循环运行，判断发送完再发送下一个
    UartTx_Interrupt_Sel  //中断发送，利用发送完成中断发送下一个
}UartTx_Mode_SelType;  //uart发送方式选择

typedef struct
{
	USART_TypeDef* USARTx;
    DMA_Channel_TypeDef* DMAy_Channelx_Tx;
    DMA_Channel_TypeDef* DMAy_Channelx_Rx;
    void (*uart_priority_task)(uint8_t * pdata, uint32_t len);  //优先任务，中断调度执行
    uint8_t * pbuffer_Rx;  //数据缓存指针
    uint32_t Buffer_Size_Rx;  //数据缓存大小
    uint32_t buffer_len_Rx;  //缓存数据长度
    uint8_t * pbuffer_Tx;
    uint32_t Buffer_Size_Tx;
    uint32_t buffer_len_Tx;
#if QUEUE_USED == 1
    pQueue_TypeDef pbuffer_queue_Rx;  //串口接收数据缓存队列
#endif
    UartTx_Mode_SelType UartTx_Mode_Sel;
}Uart_PortInfo;  //uart端口信息

#if defined STM32_STANDARD
#define UART_CONFIG_LIST {\
    {\
        {.DMAy_Channelx_Tx = NULL/*DMA1_Channel4*/, .DMA_PeripheralBaseAddr_Tx = (u32)&USART1->DR, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = NULL/*DMA1_Channel5*/, .DMA_PeripheralBaseAddr_Rx = (u32)&USART1->DR, .DMA_Priority_Rx = DMA_Priority_Medium},\
        {.GPIO_Tx = GPIOA, .Pin_Tx = GPIO_Pin_9, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOA,\
         .GPIO_Rx = GPIOA, .Pin_Rx = GPIO_Pin_10, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOA},\
        {.USARTx = USART1, .RCC_APBPeriph = RCC_APB2Periph_USART1, .RCC_APBPeriph_Sel = RCC_APB2Periph_Sel},\
        {.NVIC_IRQChannel = USART1_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
    }\
};

typedef struct
{
	GPIO_TypeDef* GPIO_Tx;
    GPIO_TypeDef* GPIO_Rx;
    uint32_t RCC_APB2Periph_Tx;
    uint32_t RCC_APB2Periph_Rx;
	uint16_t Pin_Tx;
    uint16_t Pin_Rx;
}Uart_GPIOType;  //uart引脚配置

typedef enum
{
	RCC_APB1Periph_Sel = 0,
	RCC_APB2Periph_Sel
}Uart_RCC_APBPeriph_Sel;  //外设时钟选择

typedef enum
{
	DMA_NoUsed_Sel = 0,
	DMA_Used_Sel
}Uart_DMA_UseSel;  //DMA使用与否选择

typedef struct
{
	USART_TypeDef* USARTx;
    uint32_t RCC_APBPeriph;
    uint32_t USART_BaudRate;
	Uart_RCC_APBPeriph_Sel RCC_APBPeriph_Sel;
    Uart_DMA_UseSel DMA_UseTx_Sel;  //Tx使用DMA选择
    Uart_DMA_UseSel DMA_UseRx_Sel;
    UartTx_Mode_SelType UartTx_Mode_Sel;
}Uart_UartType;  //uart端口配置

typedef struct
{
	uint8_t NVIC_IRQChannel;
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
}Uart_NVICType;  //uart中断配置

typedef struct
{
	DMA_Channel_TypeDef* DMAy_Channelx_Tx;
    uint32_t DMA_PeripheralBaseAddr_Tx;
    uint32_t DMA_MemoryBaseAddr_Tx;
    uint32_t DMA_BufferSize_Tx;
    uint32_t DMA_Priority_Tx;
    DMA_Channel_TypeDef* DMAy_Channelx_Rx;
    uint32_t DMA_PeripheralBaseAddr_Rx;
    uint32_t DMA_MemoryBaseAddr_Rx;
    uint32_t DMA_BufferSize_Rx;
    uint32_t DMA_Priority_Rx;
}Uart_DMAType;  //uart dma配置

typedef struct
{
    Uart_DMAType  Uart_DMA;
	Uart_GPIOType Uart_GPIO;
    Uart_UartType Uart_Uart;
    Uart_NVICType Uart_NVIC;
}Uart_ConfigType;  //uart配置
#endif

void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t rxbuf_size, uint32_t txbuf_size, UartTx_Mode_SelType mode);

void Uart_PriorityTask_Regist(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));  //优先任务函数注册

void uart_write(Uart_Port uartx, uint8_t * pdata, uint32_t len);
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));

void uart_send_timing(Uart_Port uartx);

#endif
