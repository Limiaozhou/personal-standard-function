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
	number_of_uart
}Uart_Port;  //uart�˿�

typedef struct
{
	USART_TypeDef* USARTx;
    void (*uart_priority_task)(uint8_t * pdata, uint32_t len);  //���������жϵ���ִ��
    uint8_t * pbuffer;  //�������ݻ���ָ��
    uint32_t Buffer_Size;  //���ݻ����С
    uint32_t buffer_len;  //�������ݳ���
}Uart_PortInfo;  //uart�˿���Ϣ

#if defined STM32_STANDARD
#define UART_CONFIG_LIST {\
    {\
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
}Uart_GPIOType;  //uart��������

typedef enum
{
	RCC_APB1Periph_Sel = 0,
	RCC_APB2Periph_Sel
}Uart_RCC_APBPeriph;  //����ʱ��ѡ��

typedef struct
{
	USART_TypeDef* USARTx;
    uint32_t RCC_APBPeriph;
    uint32_t USART_BaudRate;
	Uart_RCC_APBPeriph RCC_APBPeriph_Sel;
}Uart_UartType;  //uart�˿�����

typedef struct
{
	uint8_t NVIC_IRQChannel;
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
}Uart_NVICType;  //uart�ж�����

typedef struct
{
	Uart_GPIOType Uart_GPIO;
    Uart_UartType Uart_Uart;
    Uart_NVICType Uart_NVIC;
}Uart_ConfigType;  //uart����
#endif

void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t buf_size);

void Uart_PriorityTask_Regist(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));  //����������ע��

void uart_write(Uart_Port uartx, uint8_t * pdata, uint32_t len);
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len));

#endif
