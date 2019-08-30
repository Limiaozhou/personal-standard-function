#ifndef __BSP_USART_H
#define __BSP_USART_H

//#include "stm32f7xx_hal.h"
// #include "iostm8s103F3.h"
// #include "iostm8s207c8.h"

// #include "bsp_system.h"
#include "queue.h"

// #include "typedef.h"

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef enum
{
	UART1_Select = 0,
	UART2_Select,
    UART3_Select
}UARTx_Select_TypeDef;  //串口选择枚举体

//STM32
//#define USART1_RX_GPIO GPIOA
//#define USART1_RX_PIN  GPIO_PIN_10
//#define USART1_RX_AF   GPIO_AF7_USART1
//#define USART1_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
//
//#define USART1_TX_GPIO GPIOA
//#define USART1_TX_PIN  GPIO_PIN_9
//#define USART1_TX_AF   GPIO_AF7_USART1
//#define USART1_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
//
//#define USART1_DMA_CLK_ENABLE() __HAL_RCC_DMA2_CLK_ENABLE()
//
//#define USART1_TX_DMA_STREAM  DMA2_Stream7
//#define USART1_TX_DMA_CHANNEL DMA_CHANNEL_4
//
//#define USART1_RX_DMA_STREAM  DMA2_Stream5
//#define USART1_RX_DMA_CHANNEL DMA_CHANNEL_4

typedef void (*uart_data_deal_cb)(uint8_t * pdata, uint32_t len);  //数据解析回调函数

void Uart_Init(UARTx_Select_TypeDef uartx, uint32_t bound);

void uart_write(UARTx_Select_TypeDef uartx, uint8_t * pdata, uint32_t len);
void uart_read(UARTx_Select_TypeDef uartx, uart_data_deal_cb uart_data_deal);

#endif
