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
#include "stdio.h"  //����fputc�������������

//#include "typedef.h"

//������������
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned int uint16_t;
//typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
//typedef unsigned long int uint32_t;
//typedef signed long int int32_t;

typedef enum
{
	Uart1 = 0,
    Uart2,
    Uart3,
    Uart4,
    Uart5,
	number_of_uart
}Uart_Port;  //uart�˿�

typedef enum
{
    UartTx_ImmediatelyBlock_Sel = 0,  //�����������ͣ���ѭ��ֱ��������
	UartTx_LoopBlock_Sel,  //ѭ���������ͣ����㷢����������ѭ��ֱ��������
	UartTx_Loop_Sel,  //ѭ�����ͣ��жϷ������ٷ�����һ��
    UartTx_Interrupt_Sel  //�жϷ��ͣ����÷�������жϷ�����һ��
}UartTx_Mode_SelType;  //uart���ͷ�ʽѡ��

typedef struct
{
	USART_TypeDef* USARTx;
    DMA_Channel_TypeDef* DMAy_Channelx_Tx;
    DMA_Channel_TypeDef* DMAy_Channelx_Rx;
    void (*uart_priority_task)(uint8_t * pdata, uint32_t len);  //���������жϵ���ִ��
    pQueue_TypeDef pbuffer_queue_Rx;  //���ڽ������ݻ��滷�ζ���
    pQueue_TypeDef pframe_queue_Rx;  //���ڽ�������֡���ζ��У�����֡����ÿ֡������Ϣ
    pQueue_TypeDef pbuffer_queue_Tx;
    pQueue_TypeDef pframe_queue_Tx;
    uint32_t current_frame_len_Tx;  //��ǰ֡����
    uint32_t frame_interval_times_Tx;  //����֮֡����ѭ������
    UartTx_Mode_SelType UartTx_Mode_Sel;
}Uart_PortInfo;  //uart�˿���Ϣ

#if defined STM32_STANDARD
#define UART_CONFIG_LIST {\
    {\
        {.DMAy_Channelx_Tx = /*NULL*/DMA1_Channel4, .DMA_PeripheralBaseAddr_Tx = (u32)&USART1->DR, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = /*NULL*/DMA1_Channel5, .DMA_PeripheralBaseAddr_Rx = (u32)&USART1->DR, .DMA_Priority_Rx = DMA_Priority_Medium,\
         .RCC_AHBPeriph = RCC_AHBPeriph_DMA1},\
        {.GPIO_Tx = GPIOA, .Pin_Tx = GPIO_Pin_9, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOA,\
         .GPIO_Rx = GPIOA, .Pin_Rx = GPIO_Pin_10, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOA},\
        {.USARTx = USART1, .RCC_APBPeriph = RCC_APB2Periph_USART1, .RCC_APBPeriph_Sel = RCC_APB2Periph_Sel},\
        {.NVIC_IRQChannel = USART1_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
    },\
    {\
        {.DMAy_Channelx_Tx = /*NULL*/DMA1_Channel7, .DMA_PeripheralBaseAddr_Tx = (u32)&USART2->DR, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = /*NULL*/DMA1_Channel6, .DMA_PeripheralBaseAddr_Rx = (u32)&USART2->DR, .DMA_Priority_Rx = DMA_Priority_Medium,\
         .RCC_AHBPeriph = RCC_AHBPeriph_DMA1},\
        {.GPIO_Tx = GPIOA, .Pin_Tx = GPIO_Pin_2, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOA,\
         .GPIO_Rx = GPIOA, .Pin_Rx = GPIO_Pin_3, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOA},\
        {.USARTx = USART2, .RCC_APBPeriph = RCC_APB1Periph_USART2, .RCC_APBPeriph_Sel = RCC_APB1Periph_Sel},\
        {.NVIC_IRQChannel = USART2_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
    },\
    {\
        {.DMAy_Channelx_Tx = /*NULL*/DMA1_Channel2, .DMA_PeripheralBaseAddr_Tx = (u32)&USART3->DR, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = /*NULL*/DMA1_Channel3, .DMA_PeripheralBaseAddr_Rx = (u32)&USART3->DR, .DMA_Priority_Rx = DMA_Priority_Medium,\
         .RCC_AHBPeriph = RCC_AHBPeriph_DMA1},\
        {.GPIO_Tx = GPIOB, .Pin_Tx = GPIO_Pin_10, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOB,\
         .GPIO_Rx = GPIOB, .Pin_Rx = GPIO_Pin_11, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOB},\
        {.USARTx = USART3, .RCC_APBPeriph = RCC_APB1Periph_USART3, .RCC_APBPeriph_Sel = RCC_APB1Periph_Sel},\
        {.NVIC_IRQChannel = USART3_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
    },\
    {\
        {.DMAy_Channelx_Tx = /*NULL*/DMA2_Channel5, .DMA_PeripheralBaseAddr_Tx = (u32)&UART4->DR, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = /*NULL*/DMA2_Channel3, .DMA_PeripheralBaseAddr_Rx = (u32)&UART4->DR, .DMA_Priority_Rx = DMA_Priority_Medium,\
         .RCC_AHBPeriph = RCC_AHBPeriph_DMA2},\
        {.GPIO_Tx = GPIOC, .Pin_Tx = GPIO_Pin_10, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOC,\
         .GPIO_Rx = GPIOC, .Pin_Rx = GPIO_Pin_11, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOC},\
        {.USARTx = UART4, .RCC_APBPeriph = RCC_APB1Periph_UART4, .RCC_APBPeriph_Sel = RCC_APB1Periph_Sel},\
        {.NVIC_IRQChannel = /*NULL*/UART4_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
    },\
    {\
        {.DMAy_Channelx_Tx = NULL, .DMA_PeripheralBaseAddr_Tx = (u32)NULL, .DMA_Priority_Tx = DMA_Priority_Medium,\
         .DMAy_Channelx_Rx = NULL, .DMA_PeripheralBaseAddr_Rx = (u32)NULL, .DMA_Priority_Rx = DMA_Priority_Medium,\
         .RCC_AHBPeriph = RCC_AHBPeriph_DMA2},\
        {.GPIO_Tx = GPIOC, .Pin_Tx = GPIO_Pin_12, .RCC_APB2Periph_Tx = RCC_APB2Periph_GPIOC,\
         .GPIO_Rx = GPIOD, .Pin_Rx = GPIO_Pin_2, .RCC_APB2Periph_Rx = RCC_APB2Periph_GPIOD},\
        {.USARTx = UART5, .RCC_APBPeriph = RCC_APB1Periph_UART5, .RCC_APBPeriph_Sel = RCC_APB1Periph_Sel},\
        {.NVIC_IRQChannel = /*NULL*/UART5_IRQn, .NVIC_IRQChannelPreemptionPriority = 3, .NVIC_IRQChannelSubPriority = 3}\
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
}Uart_GPIOType;  //uart��������

typedef enum
{
	RCC_APB1Periph_Sel = 0,
	RCC_APB2Periph_Sel
}Uart_RCC_APBPeriph_Sel;  //����ʱ��ѡ��

typedef enum
{
	DMA_NoUsed_Sel = 0,
	DMA_Used_Sel
}Uart_DMA_UseSel;  //DMAʹ�����ѡ��

typedef struct
{
	USART_TypeDef* USARTx;
    uint32_t RCC_APBPeriph;
    uint32_t USART_BaudRate;
	Uart_RCC_APBPeriph_Sel RCC_APBPeriph_Sel;
    Uart_DMA_UseSel DMA_UseTx_Sel;  //Txʹ��DMAѡ��
    Uart_DMA_UseSel DMA_UseRx_Sel;
}Uart_UartType;  //uart�˿�����

typedef struct
{
	uint8_t NVIC_IRQChannel;
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
}Uart_NVICType;  //uart�ж�����

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
    uint32_t RCC_AHBPeriph;
}Uart_DMAType;  //uart dma����

typedef struct
{
    Uart_DMAType  Uart_DMA;
	Uart_GPIOType Uart_GPIO;
    Uart_UartType Uart_Uart;
    Uart_NVICType Uart_NVIC;
}Uart_ConfigType;  //uart����
#endif

//���ڳ�ʼ����uartx�����ںţ�baudrate�������ʣ�rxbuf_size�����ջ����С��txbuf_size�����ͻ����С��mode������ģʽ���������˷���dmaͨ�������ģʽѡ����Ч��
void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t rxbuf_size, uint32_t txbuf_size, UartTx_Mode_SelType mode);

void Uart_PriorityTask_Regist(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));  //����������ע��

void uart_write(Uart_Port uartx, uint8_t * pdata, uint32_t len);
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));

void uart_send_loop(Uart_Port uartx);

#endif
