#ifndef __BSP_USART_H
#define __BSP_USART_H

//#include "stm32f7xx_hal.h"
#include "iostm8s103F3.h"
#include "bsp_system.h"
#include "bsp_queue.h"

typedef enum
{
	UART1_Select = 1,
	UART2_Select,
    UART3_Select,
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

typedef void (*data_deal_cb)(unsigned char *pdata, unsigned char len);  //数据解析回调函数

void Uart_Init(UARTx_Select_TypeDef uartx, unsigned long int bound);

void uart_write(UARTx_Select_TypeDef uartx, unsigned char *pdata, unsigned char len);
void uart_read(UARTx_Select_TypeDef uartx, data_deal_cb callback);

#endif
