#include "bsp_usart.h"
#include "stdlib.h"  //包含NULL

Uart_PortInfo Uart_PortInfoList[number_of_uart] = {NULL};

static void Uart_GPIOInit(Uart_GPIOType * pUart_GPIO);  //串口引脚配置
static void Uart_UartInit(Uart_UartType * pUart_Uart);  //串口参数配置
static void Uart_NVICInit(Uart_NVICType * pUart_NVIC);  //串口中断配置
static void Uart_IRQHandler_Deal(Uart_Port uartx);  //串口中断处理

//串口初始化
void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t buf_size)
{
    Uart_ConfigType Uart_ConfigList[number_of_uart] = UART_CONFIG_LIST;
    
    if((uartx < number_of_uart) && Uart_ConfigList[uartx].Uart_Uart.USARTx)
    {
        Uart_ConfigList[uartx].Uart_Uart.USART_BaudRate = baudrate;
        Uart_PortInfoList[uartx].USARTx = Uart_ConfigList[uartx].Uart_Uart.USARTx;
        
        if(buf_size > 0)
        {
            Uart_PortInfoList[uartx].Buffer_Size = buf_size;
            Uart_PortInfoList[uartx].pbuffer = (uint8_t*)malloc(Uart_PortInfoList[uartx].Buffer_Size * sizeof(uint8_t));
        }
        
        Uart_GPIOInit(&Uart_ConfigList[uartx].Uart_GPIO);
        Uart_UartInit(&Uart_ConfigList[uartx].Uart_Uart);
        Uart_NVICInit(&Uart_ConfigList[uartx].Uart_NVIC);
    }
}

//串口引脚配置
static void Uart_GPIOInit(Uart_GPIOType * pUart_GPIO)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(pUart_GPIO->RCC_APB2Periph_Tx, ENABLE);
    RCC_APB2PeriphClockCmd(pUart_GPIO->RCC_APB2Periph_Rx, ENABLE);
    
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStruct.GPIO_Pin = pUart_GPIO->Pin_Tx;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(pUart_GPIO->GPIO_Tx, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = pUart_GPIO->Pin_Rx;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(pUart_GPIO->GPIO_Rx, &GPIO_InitStruct);
}

//串口参数配置
static void Uart_UartInit(Uart_UartType * pUart_Uart)
{
    USART_InitTypeDef USART_InitStructure;
    
    if(pUart_Uart->RCC_APBPeriph_Sel == RCC_APB2Periph_Sel)
        RCC_APB2PeriphClockCmd(pUart_Uart->RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(pUart_Uart->RCC_APBPeriph, ENABLE);

    USART_DeInit(pUart_Uart->USARTx);

    USART_InitStructure.USART_BaudRate = pUart_Uart->USART_BaudRate;  //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;  //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 //收发模式
    USART_Init(pUart_Uart->USARTx, &USART_InitStructure);  //初始化串口
    
    USART_ITConfig(pUart_Uart->USARTx, USART_IT_RXNE, ENABLE);  //开启接收字节中断
    USART_ITConfig(pUart_Uart->USARTx, USART_IT_IDLE, ENABLE);  //开启空闲中断
    USART_Cmd(pUart_Uart->USARTx, ENABLE);  //使能串口
}

//串口中断配置
static void Uart_NVICInit(Uart_NVICType * pUart_NVIC)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = pUart_NVIC->NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pUart_NVIC->NVIC_IRQChannelPreemptionPriority;  //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = pUart_NVIC->NVIC_IRQChannelSubPriority;  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化VIC寄存器
}

//优先任务函数注册
void Uart_PriorityTask_Regist(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len))
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx)
        Uart_PortInfoList[uartx].uart_priority_task = uart_task;
}

//串口写数据
void uart_write(Uart_Port uartx, uint8_t * pdata, uint32_t len)
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx)
    {
        while(len--)
        {
            USART_SendData(Uart_PortInfoList[uartx].USARTx, *(pdata++));
            while(!USART_GetFlagStatus(Uart_PortInfoList[uartx].USARTx, USART_FLAG_TXE));  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
        }
    }
}

//串口读数据处理
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len))
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx && Uart_PortInfoList[uartx].buffer_len)
    {
        uart_task(Uart_PortInfoList[uartx].pbuffer, Uart_PortInfoList[uartx].buffer_len);
        Uart_PortInfoList[uartx].buffer_len = 0;
    }
}

//串口中断处理
static void Uart_IRQHandler_Deal(Uart_Port uartx)
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx)
    {
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_RXNE) != RESET)
            Uart_PortInfoList[uartx].pbuffer[Uart_PortInfoList[uartx].buffer_len++] = USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
        
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_IDLE) != RESET)
        {
            USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
            if(Uart_PortInfoList[uartx].uart_priority_task != NULL)
            {
                Uart_PortInfoList[uartx].uart_priority_task(Uart_PortInfoList[uartx].pbuffer, Uart_PortInfoList[uartx].buffer_len);
                Uart_PortInfoList[uartx].buffer_len = 0;
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    if(Uart_PortInfoList[Uart1].USARTx == USART1)
        Uart_IRQHandler_Deal(Uart1);
}
