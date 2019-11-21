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

typedef enum
{
	Uart1 = 0,
	number_of_uart
}Uart_Port;  //uart端口

typedef struct
{
	USART_TypeDef* USARTx;
    void (*uart_priority_task)(uint8_t * pdata, uint32_t len);  //优先任务，中断调度执行
    pQueue_TypeDef pbuffer_queue;  //串口接收数据缓存队列
//    uint8_t * pbuffer;  //串口数据缓存指针
//    uint32_t Buffer_Size;  //数据缓存大小
//    uint32_t buffer_len;  //缓存数据长度
}Uart_PortInfo;  //uart端口信息

#if defined STM32_STANDARD
#define UART_CONFIG_LIST {\
    {\
        {.DMAy_Channelx_Tx = DMA1_Channel4, .DMA_PeripheralBaseAddr_Tx = (u32)&USART1->DR, .DMA_MemoryBaseAddr_Tx = NULL,\
         .DMA_BufferSize_Tx = 50, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = DMA1_Channel5, .DMA_PeripheralBaseAddr_Rx = (u32)&USART1->DR, .DMA_MemoryBaseAddr_Rx = NULL,\
         .DMA_BufferSize_Rx = 50, .DMA_Priority_Rx = DMA_Priority_Medium},\
        {.GPIO_Tx = GPIOA, .Pin_Tx = GPIO_Pin_9, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOA,\
         .GPIO_Rx = GPIOA, .Pin_Rx = GPIO_Pin_10, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOA},\
        {.USARTx = USART1, .RCC_APBPeriph = RCC_APB2Periph_USART1, .USART_BaudRate = 9600, .RCC_APBPeriph_Sel = RCC_APB2Periph_Sel},\
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
}Uart_RCC_APBPeriph;  //外设时钟选择

typedef struct
{
	USART_TypeDef* USARTx;
    uint32_t RCC_APBPeriph;
    uint32_t USART_BaudRate;
	Uart_RCC_APBPeriph RCC_APBPeriph_Sel;
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

void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t buf_size);

void Uart_PriorityTask_Regist(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));  //优先任务函数注册

void uart_write(Uart_Port uartx, uint8_t * pdata, uint32_t len);
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));

#endif
