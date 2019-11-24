#include "bsp_usart.h"
#include "stdlib.h"  //包含NULL和内存分配函数
#include "string.h"  //包含memcpy等字符操作函数

static Uart_PortInfo Uart_PortInfoList[number_of_uart] = {NULL};

//帧间隔循环次数
#define FRAME_INTERVAL_TIMES 1000

static void Uart_GPIOInit(Uart_GPIOType * pUart_GPIO);  //串口引脚配置
static void Uart_UartInit(Uart_UartType * pUart_Uart);  //串口参数配置
static void Uart_NVICInit(Uart_NVICType * pUart_NVIC);  //串口中断配置
static void Uart_DMAInit(Uart_DMAType * pUart_DMA);  //串口DMA配置
static void DMA_ReEnable(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_BufferSize);  //DMA失能，配置数量寄存器，再重新使能
static void Uart_IRQHandler_Deal(Uart_Port uartx);  //串口中断处理

//串口初始化
void Uart_Init(Uart_Port uartx, uint32_t baudrate, uint32_t rxbuf_size, uint32_t txbuf_size, UartTx_Mode_SelType mode)
{
    Uart_ConfigType Uart_ConfigList[number_of_uart] = UART_CONFIG_LIST;
    
    if((uartx < number_of_uart) && Uart_ConfigList[uartx].Uart_Uart.USARTx)
    {
        Uart_ConfigList[uartx].Uart_Uart.USART_BaudRate = baudrate;
        Uart_ConfigList[uartx].Uart_Uart.DMA_UseTx_Sel = DMA_NoUsed_Sel;
        Uart_ConfigList[uartx].Uart_Uart.DMA_UseRx_Sel = DMA_NoUsed_Sel;
        Uart_ConfigList[uartx].Uart_Uart.UartTx_Mode_Sel = mode;
        Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Tx = NULL;
        Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Rx = NULL;
        Uart_ConfigList[uartx].Uart_DMA.DMA_BufferSize_Tx = 0;
        Uart_ConfigList[uartx].Uart_DMA.DMA_BufferSize_Rx = 0;
        
        if(Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Tx)
            Uart_ConfigList[uartx].Uart_Uart.DMA_UseTx_Sel = DMA_Used_Sel;
        
        if(Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Rx)
            Uart_ConfigList[uartx].Uart_Uart.DMA_UseRx_Sel = DMA_Used_Sel;
        
        Uart_PortInfoList[uartx].USARTx = Uart_ConfigList[uartx].Uart_Uart.USARTx;
        Uart_PortInfoList[uartx].DMAy_Channelx_Tx = Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Tx;
        Uart_PortInfoList[uartx].DMAy_Channelx_Rx = Uart_ConfigList[uartx].Uart_DMA.DMAy_Channelx_Rx;
        Uart_PortInfoList[uartx].UartTx_Mode_Sel = Uart_ConfigList[uartx].Uart_Uart.UartTx_Mode_Sel;
        
        if(rxbuf_size > 0)
        {
//            if(Uart_PortInfoList[uartx].DMAy_Channelx_Rx)
//            {
//                Uart_PortInfoList[uartx].pbuffer_Rx = (uint8_t*)malloc( rxbuf_size * sizeof(uint8_t) );
//                if(Uart_PortInfoList[uartx].pbuffer_Rx)  //分配内存成功
//                {  //设置寄存器数据量要大于等于可能接收的数，否则数据会错位，寄存器减到0后一个数据，会在下个帧成第一个数据
//                    Uart_PortInfoList[uartx].Buffer_Size_Rx = rxbuf_size;  //分配成功才能置dma数量寄存器非0
//                    Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Rx = (uint32_t)Uart_PortInfoList[uartx].pbuffer_Rx;
//                    Uart_ConfigList[uartx].Uart_DMA.DMA_BufferSize_Rx = Uart_PortInfoList[uartx].Buffer_Size_Rx;
//                }
//            }
//            else
//            {
//#if QUEUE_USED == 1
                Uart_PortInfoList[uartx].pbuffer_queue_Rx = queue_init(rxbuf_size, 1);
                Uart_PortInfoList[uartx].pframe_queue_Rx = queue_init(10, 4);
                
//#else
//                Uart_PortInfoList[uartx].pbuffer_Rx = (uint8_t*)malloc( rxbuf_size * sizeof(uint8_t) );
//                if(Uart_PortInfoList[uartx].pbuffer_Rx)  //分配内存成功
//                {  //设置寄存器数据量要大于等于可能接收的数，否则数据会错位，寄存器减到0后一个数据，会在下个帧成第一个数据
//                    Uart_PortInfoList[uartx].Buffer_Size_Rx = rxbuf_size;  //分配成功才能置dma数量寄存器非0
//                    Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Rx = (uint32_t)Uart_PortInfoList[uartx].pbuffer_Rx;
//                    Uart_ConfigList[uartx].Uart_DMA.DMA_BufferSize_Rx = Uart_PortInfoList[uartx].Buffer_Size_Rx;
//                }
//#endif
//            }
        }
        
        if(txbuf_size > 0)
        {
//            if(Uart_PortInfoList[uartx].DMAy_Channelx_Tx)
//            {
//                Uart_PortInfoList[uartx].pbuffer_Tx = (uint8_t*)malloc( txbuf_size * sizeof(uint8_t) );
//                if(Uart_PortInfoList[uartx].pbuffer_Tx)
//                {
//                    Uart_PortInfoList[uartx].Buffer_Size_Tx = txbuf_size;
//                    Uart_ConfigList[uartx].Uart_DMA.DMA_MemoryBaseAddr_Tx = (uint32_t)Uart_PortInfoList[uartx].pbuffer_Tx;
//                }
//            }
//            else
//            {
                Uart_PortInfoList[uartx].pbuffer_queue_Tx = queue_init(txbuf_size, 1);
                Uart_PortInfoList[uartx].pframe_queue_Tx = queue_init(10, 4);
//            }
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
    
    if(pUart_Uart->DMA_UseTx_Sel == DMA_Used_Sel)
        USART_DMACmd(pUart_Uart->USARTx, USART_DMAReq_Tx, ENABLE);  //使能串口DMA发送
    else if(pUart_Uart->UartTx_Mode_Sel == UartTx_Interrupt_Sel)
        USART_ITConfig(pUart_Uart->USARTx, USART_IT_TXE, ENABLE);  //开启发送字节中断
    
    if(pUart_Uart->DMA_UseRx_Sel == DMA_Used_Sel)
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
    if(!DMAy_Channelx)
        return;
    
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
        if(Uart_PortInfoList[uartx].DMAy_Channelx_Tx)
        {
//            uint32_t size = Uart_PortInfoList[uartx].Buffer_Size_Tx;
//            
//            memcpy( Uart_PortInfoList[uartx].pbuffer_Tx, pdata, (len > size ? size : len) );
//            
//            DMA_ReEnable( Uart_PortInfoList[uartx].DMAy_Channelx_Tx, len );
        }
        else
        {
            if(Uart_PortInfoList[uartx].UartTx_Mode_Sel == UartTx_Block_Sel)
            {  //阻塞发送
                while(len--)
                {
                    USART_SendData(Uart_PortInfoList[uartx].USARTx, *(pdata++));
                    while(!USART_GetFlagStatus(Uart_PortInfoList[uartx].USARTx, USART_FLAG_TXE));  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
                }
            }
            else if(Uart_PortInfoList[uartx].UartTx_Mode_Sel == UartTx_Timing_Sel)
            {  //定时发送
                //写数据到缓存，等待发送
                if( queue_write(Uart_PortInfoList[uartx].pbuffer_queue_Tx, pdata, len) )
                    return;  //写失败，数据长度大于剩余长度，就返回
                
                //保存此帧长度到帧记录队列
                queue_write(Uart_PortInfoList[uartx].pframe_queue_Tx, &len, 1);
                
//                uint32_t size = Uart_PortInfoList[uartx].Buffer_Size_Tx;
//                Uart_PortInfoList[uartx].buffer_len_Tx = len;
//                memcpy( Uart_PortInfoList[uartx].pbuffer_Tx, pdata, (len > size ? size : len) );
            }
            else
            {  //中断发送
                
            }
        }
    }
}

//串口定时发送数据，运行一次发送队列中一个数据，100
void uart_send_timing(Uart_Port uartx)
{
    if( (uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx && (Uart_PortInfoList[uartx].UartTx_Mode_Sel == UartTx_Timing_Sel) )
    {
        static uint32_t frame_len = 0;  //记录帧长度，发送完一帧的数据，再读下一帧长
        static uint16_t tiems = 0;  //发完一帧，间隔一段循环时间，取下帧
        
        if(!frame_len)
        {
            if(++tiems >= FRAME_INTERVAL_TIMES)
            {
                queue_read( Uart_PortInfoList[uartx].pframe_queue_Tx, &frame_len, 1 );
                tiems = 0;
            }
        }
        
        if( frame_len && USART_GetFlagStatus(Uart_PortInfoList[uartx].USARTx, USART_FLAG_TXE) )
        {
            uint8_t data = 0;
            
            queue_read( Uart_PortInfoList[uartx].pbuffer_queue_Tx, &data, 1 );
            USART_SendData( Uart_PortInfoList[uartx].USARTx, data );
            --frame_len;
        }
    }
}

//串口读数据处理
void uart_read(Uart_Port uartx, void (*uart_task)(uint8_t * pdata, uint32_t len))
{
//    if( (uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx )
//    {
//        if(Uart_PortInfoList[uartx].DMAy_Channelx_Rx)
//        {
//            uint32_t buf_len = Uart_PortInfoList[uartx].Buffer_Size_Rx - DMA_GetCurrDataCounter(Uart_PortInfoList[uartx].DMAy_Channelx_Rx);
//            
//            if(buf_len)
//            {
//                uart_task(Uart_PortInfoList[uartx].pbuffer_Rx, buf_len);
//                DMA_ReEnable( Uart_PortInfoList[uartx].DMAy_Channelx_Rx, Uart_PortInfoList[uartx].Buffer_Size_Rx );
//            }
//        }
//        else
//        {
//#if QUEUE_USED == 1
//            uint32_t buf_len = get_queue_len(Uart_PortInfoList[uartx].pbuffer_queue_Rx);  //保存数据长度
//            
//            if( buf_len )
//            {
//                uint8_t * pdata = (uint8_t*)malloc( buf_len * sizeof(uint8_t) );  //分配数据空间
//                
//                if(!pdata)
//                    return;
//                
//                queue_read( Uart_PortInfoList[uartx].pbuffer_queue_Rx, pdata, buf_len );
//                uart_task(pdata, buf_len);
//                
//                free(pdata);  //释放数据空间
//            }
//            
//#else
//            if(Uart_PortInfoList[uartx].buffer_len_Rx)
//            {
//                uart_task(Uart_PortInfoList[uartx].pbuffer_Rx, Uart_PortInfoList[uartx].buffer_len_Rx);
//                Uart_PortInfoList[uartx].buffer_len_Rx = 0;
//            }
//#endif
//        }
//    }
}

//串口中断处理
static void Uart_IRQHandler_Deal(Uart_Port uartx)
{
    if((uartx < number_of_uart) && Uart_PortInfoList[uartx].USARTx)
    {
        uint8_t data = 0;
        
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_RXNE) != RESET)
        {
            data = USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
            
            if(!Uart_PortInfoList[uartx].DMAy_Channelx_Rx)
            {
//#if QUEUE_USED == 1
                queue_write(Uart_PortInfoList[uartx].pbuffer_queue_Rx, &data, 1);
                
//#else
//                if( Uart_PortInfoList[uartx].buffer_len_Rx < Uart_PortInfoList[uartx].Buffer_Size_Rx )
//                    Uart_PortInfoList[uartx].pbuffer_Rx[Uart_PortInfoList[uartx].buffer_len_Rx++] = data;
//#endif
            }
        }
        
        if(USART_GetITStatus(Uart_PortInfoList[uartx].USARTx, USART_IT_IDLE) != RESET)
        {
            uint32_t frame_len = 0;
            
            USART_ReceiveData(Uart_PortInfoList[uartx].USARTx);
            
            frame_len = Uart_PortInfoList[uartx].pbuffer_queue_Rx->len - queue_data_sum(Uart_PortInfoList[uartx].pframe_queue_Rx);
            queue_write(Uart_PortInfoList[uartx].pframe_queue_Rx, &frame_len, 1);
            
            if(Uart_PortInfoList[uartx].uart_priority_task != NULL)
            {
                if(Uart_PortInfoList[uartx].DMAy_Channelx_Rx)
                {
//                    uint32_t buf_len = Uart_PortInfoList[uartx].Buffer_Size_Rx - DMA_GetCurrDataCounter(Uart_PortInfoList[uartx].DMAy_Channelx_Rx);
//                    
//                    if(buf_len)
//                    {
//                        Uart_PortInfoList[uartx].uart_priority_task(Uart_PortInfoList[uartx].pbuffer_Rx, buf_len);
//                        DMA_ReEnable( Uart_PortInfoList[uartx].DMAy_Channelx_Rx, Uart_PortInfoList[uartx].Buffer_Size_Rx );
//                    }
                }
                else
                {
//#if QUEUE_USED == 1
                    uint32_t buf_len = get_queue_len(Uart_PortInfoList[uartx].pbuffer_queue_Rx);
                    
                    if( buf_len )
                    {
                        uint8_t * pdata = (uint8_t*)malloc( buf_len * sizeof(uint8_t) );
                        
                        if(!pdata)
                            return;
                        
                        queue_read( Uart_PortInfoList[uartx].pbuffer_queue_Rx, pdata, buf_len );
                        Uart_PortInfoList[uartx].uart_priority_task(pdata, buf_len);
                        
                        free(pdata);
                    }
                    
//#else
//                    if(Uart_PortInfoList[uartx].buffer_len_Rx)
//                    {
//                        Uart_PortInfoList[uartx].uart_priority_task(Uart_PortInfoList[uartx].pbuffer_Rx, Uart_PortInfoList[uartx].buffer_len_Rx);
//                        Uart_PortInfoList[uartx].buffer_len_Rx = 0;
//                    }
//#endif
                }
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    if(Uart_PortInfoList[Uart1].USARTx == USART1)
        Uart_IRQHandler_Deal(Uart1);
}
