#include "bsp_usart.h"
#include "stdlib.h"  //包含NULL和内存分配函数
#include "string.h"  //包含NULL和内存分配函数

static Uart_PortInfo Uart_PortInfoList[number_of_uart] = {NULL};

static void Uart_GPIOInit(Uart_GPIOType * pUart_GPIO);  //串口引脚配置
static void Uart_UartInit(Uart_UartType * pUart_Uart);  //串口参数配置
static void Uart_NVICInit(Uart_NVICType * pUart_NVIC);  //串口中断配置
static void Uart_DMAInit(Uart_DMAType * pUart_DMA);  //串口DMA配置
static void DMA_ReEnable(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_BufferSize);  //DMA失能，配置数量寄存器，再重新使能
static void Uart_IRQHandler_Deal(Uart_Port uartx);  //串口中断处理

//串口初始化
void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t rxbuf_size, uint32_t dma_txbuf_size)
{
    Uart_ConfigType Uart_ConfigList[number_of_uart] = UART_CONFIG_LIST;
    
    if((uartx < number_of_uart) && Uart_ConfigList[uartx].Uart_Uart.USARTx)
    {
        Uart_ConfigList[uartx].Uart_Uart.USART_BaudRate = baudrate;
        Uart_PortInfoList[uartx].USARTx = Uart_ConfigList[uartx].Uart_Uart.USARTx;
        Uart_PortInfoList[uartx].DMAy_Channelx_Tx = Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Tx;
        Uart_PortInfoList[uartx].DMAy_Channelx_Rx = Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Rx;
        
        if(rxbuf_size > 0)
        {
//            Uart_PortInfoList[uartx].pbuffer_queue = queue_init(rxbuf_size, 1);
            Uart_PortInfoList[uartx].Buffer_Size_Rx = rxbuf_size;
            Uart_PortInfoList[uartx].pbuffer_Rx = (uint8_t*)malloc( Uart_PortInfoList[uartx].Buffer_Size_Rx * sizeof(uint8_t) );
            if(Uart_PortInfoList[uartx].pbuffer_Rx)  //分配内存成功
            {  //设置寄存器数据量要大于等于可能接收的数，否则数据会错位，寄存器减到0后一个数据，会在下个帧成第一个数据
                Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Rx = (uint32_t)Uart_PortInfoList[uartx].pbuffer_Rx;
                Uart_ConfigList[uartx].Uart_DMA.DMA_BufferSize_Rx = Uart_PortInfoList[uartx].Buffer_Size_Rx;
            }
        }
        
        if(dma_txbuf_size > 0)
        {
            Uart_PortInfoList[uartx].Buffer_Size_DmaTx = dma_txbuf_size;
            Uart_PortInfoList[uartx].pbuffer_dmaTx = (uint8_t*)malloc( Uart_PortInfoList[uartx].Buffer_Size_DmaTx * sizeof(uint8_t) );
            if(Uart_PortInfoList[uartx].pbuffer_dmaTx)
                Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Tx = (uint32_t)Uart_PortInfoList[uartx].pbuffer_dmaTx;
        }
        
        Uart_DMAInit(&Uart_ConfigList[uartx].Uart_DMA);
        Uart_GPIOInit(&Uart_ConfigList[uartx].Uart_GPIO);
        Uart_UartInit(&Uart_ConfigList[uartx].Uart_Uart);
        Uart_NVICInit(&Uart_ConfigList[uartx].Uart_NVIC);
    }
}

//串口引脚配置
static void Uart_GPIOInit(Uart_GPIOType * pUart_GPIO)
{
    if( (!pUart_GPIO->GPIO_Tx) && (!pUart_GPIO->GPIO_Rx) )
        return;  //Tx、Rx通道都没有就返回
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    if(pUart_GPIO->GPIO_Tx)
    {
        RCC_APB2PeriphClockCmd(pUart_GPIO->RCC_APB2Periph_Tx, ENABLE);
        GPIO_InitStruct.GPIO_Pin = pUart_GPIO->Pin_Tx;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(pUart_GPIO->GPIO_Tx, &GPIO_InitStruct);
    }
    
    if(pUart_GPIO->GPIO_Rx)
    {
        RCC_APB2PeriphClockCmd(pUart_GPIO->RCC_APB2Periph_Rx, ENABLE);
        GPIO_InitStruct.GPIO_Pin = pUart_GPIO->Pin_Rx;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(pUart_GPIO->GPIO_Rx, &GPIO_InitStruct);
    }
}

//串口参数配置
static void Uart_UartInit(Uart_UartType * pUart_Uart)
{
    if(!pUart_Uart->USARTx)
        return;  //没有通道就返回
    
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
    
    USART_ITConfig(pUart_Uart->USARTx, USART_IT_IDLE, ENABLE);  //开启空闲中断
    
    if(pUart_Uart->DMA_UseTx_Sel == DMA_Use_Sel)
        USART_DMACmd(pUart_Uart->USARTx, USART_DMAReq_Tx, ENABLE);  //使能串口DMA发送
    
    if(pUart_Uart->DMA_UseRx_Sel == DMA_Use_Sel)
        USART_DMACmd(pUart_Uart->USARTx, USART_DMAReq_Rx, ENABLE);
    else
        USART_ITConfig(pUart_Uart->USARTx, USART_IT_RXNE, ENABLE);  //开启接收字节中断
    
    USART_Cmd(pUart_Uart->USARTx, ENABLE);  //使能串口
}

//串口中断配置
static void Uart_NVICInit(Uart_NVICType * pUart_NVIC)
{
    if(!pUart_NVIC->NVIC_IRQChannel)
        return;  //没有通道就返回
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = pUart_NVIC->NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pUart_NVIC->NVIC_IRQChannelPreemptionPriority;  //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = pUart_NVIC->NVIC_IRQChannelSubPriority;  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化VIC寄存器
}

//串口DMA配置
static void Uart_DMAInit(Uart_DMAType * pUart_DMA)
{
    if( (!pUart_DMA->DMAy_Channelx_Tx) && (!pUart_DMA->DMAy_Channelx_Rx) )
        return;  //Tx、Rx通道都没有就返回
    
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    
	if(pUart_DMA->DMAy_Channelx_Tx)  //有Tx通道
    {
        DMA_DeInit(pUart_DMA->DMAy_Channelx_Tx);  //重设为缺省值
        DMA_InitStructure.DMA_PeripheralBaseAddr = pUart_DMA->DMA_PeripheralBaseAddr_Tx;  //外设基地址
        DMA_InitStructure.DMA_MemoryBaseAddr = pUart_DMA->DMA_MemoryBaseAddr_Tx;  //DMA内存基地址
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
        DMA_InitStructure.DMA_BufferSize = pUart_DMA->DMA_BufferSize_Tx;  //DMA通道的DMA缓存的大小
        DMA_InitStructure.DMA_Priority = pUart_DMA->DMA_Priority_Tx;  //DMA通道优先级
        DMA_Init(pUart_DMA->DMAy_Channelx_Tx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
    }
    
    if(pUart_DMA->DMAy_Channelx_Rx)
    {
        DMA_DeInit(pUart_DMA->DMAy_Channelx_Rx);
        DMA_InitStructure.DMA_PeripheralBaseAddr = pUart_DMA->DMA_PeripheralBaseAddr_Rx;
        DMA_InitStructure.DMA_MemoryBaseAddr = pUart_DMA->DMA_MemoryBaseAddr_Rx;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize = pUart_DMA->DMA_BufferSize_Rx;
        DMA_InitStructure.DMA_Priority = pUart_DMA->DMA_Priority_Rx;
        DMA_Init(pUart_DMA->DMAy_Channelx_Rx, &DMA_InitStructure);
        
        DMA_Cmd(pUart_DMA->DMAy_Channelx_Rx, ENABLE);  //使能通道
    }
}

//DMA失能，配置数量寄存器，再重新使能
static void DMA_ReEnable(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_BufferSize)
{
    DMA_Cmd( DMAy_Channelx, DISABLE );  //关闭通道，以重新设置数据量大小
    DMA_SetCurrDataCounter( DMAy_Channelx, DMA_BufferSize );  //设置缓存的大小
    DMA_Cmd( DMAy_Channelx, ENABLE );  //使能通道
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

//串口DMA写数据
void uart_write_dma(Uart_Port uartx, uint8_t * pdata, uint32_t len)
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].DMAy_Channelx_Tx)
    {
        uint32_t size = Uart_PortInfoList[uartx].Buffer_Size_DmaTx;
        
        memcpy( Uart_PortInfoList[uartx].pbuffer_dmaTx, pdata, (len > size ? size : len) );
        
        DMA_ReEnable( Uart_PortInfoList[uartx].DMAy_Channelx_Tx, len );
//        DMA_Cmd( Uart_PortInfoList[uartx].DMAy_Channelx_Tx, DISABLE );  //关闭通道，以重新设置数据量大小
//        DMA_SetCurrDataCounter( Uart_PortInfoList[uartx].DMAy_Channelx_Tx, len );  //设置缓存的大小
//        DMA_Cmd( Uart_PortInfoList[uartx].DMAy_Channelx_Tx, ENABLE );  //使能通道
    }
}

//串口读数据处理
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len))
{
//    uint32_t buf_len = get_queue_len(Uart_PortInfoList[uartx].pbuffer_queue);  //保存数据长度
//    
//    if( (uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx && buf_len )
//    {
//        uint8_t * pdata = (uint8_t*)malloc( buf_len * sizeof(uint8_t) );  //分配数据空间
//        
//        if(!pdata)
//            return;
//        
//        queue_read( Uart_PortInfoList[uartx].pbuffer_queue, pdata, buf_len );
//        uart_task(pdata, buf_len);
//        
//        free(pdata);  //释放数据空间
//    }
//    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx && Uart_PortInfoList[uartx].buffer_len)
//    {
//        uart_task(Uart_PortInfoList[uartx].pbuffer, Uart_PortInfoList[uartx].buffer_len);
//        Uart_PortInfoList[uartx].buffer_len = 0;
//    }
}

//串口中断处理
static void Uart_IRQHandler_Deal(Uart_Port uartx)
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx)
    {
//        uint8_t data = 0;
        
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_RXNE) != RESET)
        {
            /*data = */USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
//            queue_write(Uart_PortInfoList[uartx].pbuffer_queue, &data, 1);
//            if( Uart_PortInfoList[uartx].buffer_len < Uart_PortInfoList[uartx].Buffer_Size )
//                Uart_PortInfoList[uartx].pbuffer[Uart_PortInfoList[uartx].buffer_len++] = data;
        }
        
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_IDLE) != RESET)
        {
            USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
            if(Uart_PortInfoList[uartx].uart_priority_task != NULL)
            {
                uint32_t buf_len = Uart_PortInfoList[uartx].Buffer_Size_Rx - DMA_GetCurrDataCounter(Uart_PortInfoList[uartx].DMAy_Channelx_Rx);
                
                Uart_PortInfoList[uartx].uart_priority_task(Uart_PortInfoList[uartx].pbuffer_Rx, buf_len);
                
                DMA_ReEnable( Uart_PortInfoList[uartx].DMAy_Channelx_Rx, Uart_PortInfoList[uartx].Buffer_Size_Rx );
//                uint32_t buf_len = get_queue_len(Uart_PortInfoList[uartx].pbuffer_queue);
//                
//                if( buf_len )
//                {
//                    uint8_t * pdata = (uint8_t*)malloc( buf_len * sizeof(uint8_t) );
//                    
//                    if(!pdata)
//                        return;
//                    
//                    queue_read( Uart_PortInfoList[uartx].pbuffer_queue, pdata, buf_len );
//                    Uart_PortInfoList[uartx].uart_priority_task(pdata, buf_len);
//                    
//                    free(pdata);
//                }
//                Uart_PortInfoList[uartx].uart_priority_task(Uart_PortInfoList[uartx].pbuffer, Uart_PortInfoList[uartx].buffer_len);
//                Uart_PortInfoList[uartx].buffer_len = 0;
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    if(Uart_PortInfoList[Uart1].USARTx == USART1)
        Uart_IRQHandler_Deal(Uart1);
}
