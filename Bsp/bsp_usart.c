#include "bsp_usart.h"

//STM32
//UART_HandleTypeDef Uart1_Handle;
//DMA_HandleTypeDef DMA_Uart1Tx_Handle;
//DMA_HandleTypeDef DMA_Uart1Rx_Handle;

Queue_TypeDef Queue_Uart1_Rx;  //uart1接收数据缓存队列
Queue_TypeDef Queue_Uart1_Rx_Num;  //uart1接收帧长队列，保存每帧长度

uart_data_deal_cb uart1_data_deal;

static void Uart_Queue_Init(UARTx_Select_TypeDef uartx);  //uart收发缓存数据队列初始化
static void Uart_Port_Init(UARTx_Select_TypeDef uartx, uint32_t bound);  //uart端口初始化
//static void Uart_DMA_Init(UARTx_Select_TypeDef uartx);  //uart DMA初始化
static void uart_deal_register(UARTx_Select_TypeDef uartx, uart_data_deal_cb uartx_data_deal);  //uart数据处理函数注册

/* 串口初始化，包涵端口、（DMA-stm32）和数据队列初始化、数据处理函数注册，输入串口号、波特率、数据处理回调函数 */
void Uart_Init(UARTx_Select_TypeDef uartx, uint32_t bound, uart_data_deal_cb uartx_data_deal)
{
	Uart_Queue_Init(uartx);  //初始化uart收发缓存数据队列
    Uart_Port_Init(uartx, bound);  //初始化uart端口
//	Uart_DMA_Init(uartx);
    uart_deal_register(uartx, uartx_data_deal);  //注册
}

/* 串口队列初始化 */
static void Uart_Queue_Init(UARTx_Select_TypeDef uartx)
{
	switch(uartx)
	{
		case UART1_Select :
			queue_init(&Queue_Uart1_Rx, 500, Uint8_Type);  //初始化接收uart1数据队列，500字节长度，注意分配堆heap要足够大
			queue_init(&Queue_Uart1_Rx_Num, 10, Uint32_Type);  //初始化接收uart1帧数队列，10字节长度
		break;
		
		default :
		break;
	}
}

/* 串口端口初始化
 *STM8如波特率9600：16M / 9600 = 1667 = 0683h，BRR1：68h，BRR2：03h
 */
static void Uart_Port_Init(UARTx_Select_TypeDef uartx, uint32_t bound)
{
    //STM8
    // uint32_t clock_uart = 0;  //串口时钟频率
    // uint32_t uart_div = 0;  //分频倍数
    // uint8_t brr1 = 0, brr2 = 0;  //波特率寄存器值缓存
    
    // clock_uart = CLK_GetMasterClock_Freq();  //获取串口时钟频率
    // uart_div = (uint32_t)((float)clock_uart / bound + 0.5);  //计算分频倍数，四舍五入
    // brr1 = (uart_div & 0x0FF0) >> 4;  //波特率寄存器BRR1值缓存
    // brr2 = ((uart_div & 0xF000) >> 8) | (uart_div & 0x0F);  //波特率寄存器BRR2值缓存
    
	switch(uartx)
	{
		case UART1_Select :
            //STM8
			UART1_DeInit();  //复位uart1串口默认配置
            //配置波特率、8位数据位、1位停止位、无校验、失能同步、使能收发
			UART1_Init(bound, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
                       UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
            UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);  //使能接收中断
            UART1_ITConfig(UART1_IT_IDLE, ENABLE);  //使能空闲中断
            // UART1_CR1 &= ~0x10;  //M，一个起始位，8个数据位，无校验
            // UART1_CR3 |= 0x00;  //STOP，1个停止位
            // UART1_BRR2 = brr2;
            // UART1_BRR1 = brr1;
            // UART1_CR2 |= 0x04;  //REN，接收使能，SR:RXNE
            // UART1_CR2 |= 0x20;  //RIEN，接收中断使能，SR:RXNE
            // UART1_CR2 |= 0x10;  //ILIEN，接收空闲中断使能，SR:IDLE
            // UART1_CR2 |= 0x08;  //TEN，发送使能，SR:TXE,TC
            
            //STM32
//			Uart1_Handle.Instance = USART1;  //UART号
//			Uart1_Handle.Init.BaudRate = bound;  //波特率
//			Uart1_Handle.Init.WordLength = UART_WORDLENGTH_8B;  //字长为8位数据格式
//			Uart1_Handle.Init.StopBits = UART_STOPBITS_1;  //一个停止位
//			Uart1_Handle.Init.Parity = UART_PARITY_NONE;  //无奇偶校验位
//			Uart1_Handle.Init.Mode = UART_MODE_TX_RX;  //收发模式
//			Uart1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;  //无硬件流控
//			Uart1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
//			Uart1_Handle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
//			Uart1_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//			
//			HAL_UART_Init(&Uart1_Handle);  //初始化和使能UART
//			
////			__HAL_UART_ENABLE_IT(&Uart1_Handle, UART_IT_RXNE);  //使能串口接收完成中断，接收完一个数据
//			__HAL_UART_ENABLE_IT(&Uart1_Handle, UART_IT_IDLE);  //使能串口空闲中断，接收完一帧数据
//			
////			HAL_UART_Receive_DMA(&Uart1_Handle, Queue_Uart1_Rx.data, Queue_Uart1_Rx.size);  //打开串口DMA接收中断，并将数据存在Queue_Uart1_Rx.data
//			HAL_UART_Receive_DMA(&Uart1_Handle, DMA_DATA_RXaa, 160);
        break;
        
		default :
		break;
	}
}

/* STM32 串口MCU(MSP)初始化回调函数，包含于HAL_UART_Init() */
//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	if(huart->Instance == USART1)  //如果是串口1，进行串口1 MSP初始化
//	{
//		__HAL_RCC_USART1_CLK_ENABLE();  //使能USART1时钟
//		
//		USART1_TX_GPIO_CLK_ENABLE();
//		USART1_RX_GPIO_CLK_ENABLE();
//		
//		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  //复用推挽输出
//		GPIO_InitStruct.Pull = GPIO_PULLUP;  //上拉
//		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  //高速
//		
//		GPIO_InitStruct.Pin = USART1_TX_PIN;  //PA9 --> USART1_TX
//		GPIO_InitStruct.Alternate = USART1_TX_AF;  //复用为USART1
//		HAL_GPIO_Init(USART1_TX_GPIO, &GPIO_InitStruct);  //初始化PA9
//
//		GPIO_InitStruct.Pin = USART1_RX_PIN;  //PA10 --> USART1_RX
//		GPIO_InitStruct.Alternate = USART1_RX_AF;
//		HAL_GPIO_Init(USART1_RX_GPIO, &GPIO_InitStruct);
//		
//		HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);  //抢占优先级3，子优先级3
//		HAL_NVIC_EnableIRQ(USART1_IRQn);  //使能USART1中断通道
//	}
//}

/* 串口DMA初始化 */
//static void Uart_DMA_Init(UARTx_Select_TypeDef uartx)
//{
//	switch(uartx)
//	{
//		case UART1_Select :
//            
//            //STM32
//			USART1_DMA_CLK_ENABLE();  //UART1-DMA时钟使能
//			
//			DMA_Uart1Tx_Handle.Instance = USART1_TX_DMA_STREAM;  //数据流选择
//			DMA_Uart1Tx_Handle.Init.Channel = USART1_TX_DMA_CHANNEL;  //通道选择
//			DMA_Uart1Tx_Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;  //存储器到外设
//			DMA_Uart1Tx_Handle.Init.PeriphInc = DMA_PINC_DISABLE;  //外设非增量模式
//			DMA_Uart1Tx_Handle.Init.MemInc = DMA_MINC_ENABLE;  //存储器增量模式
//			DMA_Uart1Tx_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  //外设数据长度:8位
//			DMA_Uart1Tx_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;  //存储器数据长度:8位
//			DMA_Uart1Tx_Handle.Init.Mode = DMA_NORMAL;  //普通模式
//			DMA_Uart1Tx_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;  //中等优先级
//			DMA_Uart1Tx_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  //禁用FIFO
//			DMA_Uart1Tx_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//			DMA_Uart1Tx_Handle.Init.MemBurst = DMA_MBURST_SINGLE;  //存储器突发单次传输
//			DMA_Uart1Tx_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  //外设突发单次传输
//			
//			HAL_DMA_DeInit(&DMA_Uart1Tx_Handle);
//			HAL_DMA_Init(&DMA_Uart1Tx_Handle);
//			__HAL_LINKDMA(&Uart1_Handle, hdmatx, DMA_Uart1Tx_Handle);  //将发送DMA与USART联系起来
//			
//			DMA_Uart1Rx_Handle.Instance = USART1_RX_DMA_STREAM;
//			DMA_Uart1Rx_Handle.Init.Channel = USART1_RX_DMA_CHANNEL;
//			DMA_Uart1Rx_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
//			DMA_Uart1Rx_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
//			DMA_Uart1Rx_Handle.Init.MemInc = DMA_MINC_ENABLE;
//			DMA_Uart1Rx_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//			DMA_Uart1Rx_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//			DMA_Uart1Rx_Handle.Init.Mode = DMA_CIRCULAR;
//			DMA_Uart1Rx_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;
//			DMA_Uart1Rx_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//			DMA_Uart1Rx_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
//			DMA_Uart1Rx_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
//			DMA_Uart1Rx_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
//			
//			HAL_DMA_DeInit(&DMA_Uart1Rx_Handle);
//			HAL_DMA_Init(&DMA_Uart1Rx_Handle);
//			__HAL_LINKDMA(&Uart1_Handle, hdmarx, DMA_Uart1Rx_Handle);  //将接收DMA与USART联系起来
//		break;
//		
//		default :
//		break;
//	}
//}

/* uart数据处理函数注册 */
static void uart_deal_register(UARTx_Select_TypeDef uartx, uart_data_deal_cb uartx_data_deal)
{
    switch(uartx)
	{
        case UART1_Select :
            uart1_data_deal = uartx_data_deal;
        break;
        
        default :
			break;
    }
}

/* 串口发送， uartx：串口号， pdata：数据指针， len：数据量 */
void uart_write(UARTx_Select_TypeDef uartx, uint8_t * pdata, uint32_t len)
{
	switch(uartx)
	{
		case UART1_Select :
		{
			while(len--)
			{
                //STM8
                UART1_SendData8(*(pdata++));  //发送数据
                while(!UART1_GetFlagStatus(UART1_FLAG_TXE));  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
//                UART1_DR = *(pdata++);  //发送数据
//                while(!UART1_SR_TXE);  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
                
                //STM32
//				USART1->TDR = *(pdata++);
//				while(__HAL_UART_GET_IT(&Uart1_Handle, UART_IT_TXE) == RESET);
			}
		}break;
		
		default :
			break;
	}
}

/* 串口数据读取处理， uartx：串口号 */
void uart_read(UARTx_Select_TypeDef uartx)
{
	switch(uartx)
	{
		case UART1_Select :
		{
            uint8_t rx1_data[500] = {0};  //队列数据缓存数组
            uint32_t rx1_frame_len = 0;  //每帧长
            uint8_t rx1_frame_num = 0;  //帧数
            
			rx1_frame_num = Queue_Uart1_Rx_Num.len;  //帧数
			
			while(rx1_frame_num--)  //完整取去每帧数据
			{
				if((!queue_read(&Queue_Uart1_Rx_Num, &rx1_frame_len, 1)) && rx1_frame_len)  //每帧长
				{
					queue_read(&Queue_Uart1_Rx, rx1_data, rx1_frame_len);  //取出一帧数据保存到rdata数组
                    if(uart1_data_deal != NULL)
                        uart1_data_deal(rx1_data, rx1_frame_len);  //将数据指针和帧长传给回调函数去解析
				}
			}
		}break;
		
		default :
			break;
	}
}

/* STM32 串口1接收中断服务程序 */
//void USART1_IRQHandler(void)
//{
////	unsigned char data = 0;  //数据
//	unsigned char len_now;  //当前帧长度
//	
////	if(__HAL_UART_GET_IT(&Uart1_Handle, UART_IT_RXNE) != RESET)  //接收单个数据
////	{
////		data = USART1->RDR;
////		queue_write_single(&Queue_Uart1_Rx, data);
////	}
//	
//	if(__HAL_UART_GET_IT(&Uart1_Handle, UART_IT_IDLE) != RESET)  //接收完一帧数据，空闲中断
//	{
//		USART1->ICR |= 0x10;  //清0空闲中断
////		Queue_Uart1_Rx.tail = Queue_Uart1_Rx.size - USART1_RX_DMA_STREAM->NDTR;  //队尾 = 总长 - 剩余长
////		
////		get_queue_len(&Queue_Uart1_Rx);  //计算队列中数据长度
////		
////		len_now = Queue_Uart1_Rx.len - queue_data_sum(&Queue_Uart1_Rx_Num);  //当前帧长度 = 当前数据队列长度 - 已保存的总长度
////		queue_write_single(&Queue_Uart1_Rx_Num, len_now);  //保存当前数据帧长度
//	}
//}

//STM8 串口接收中断函数
//#pragma vector=UART1_R_OR_vector  //20
//__interrupt void UART1_R_OR_IRQHandler(void)
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    uint8_t data = 0;  //数据缓存
    uint32_t frame_len_now = 0;  //当前帧长度
    
    if(UART1_GetFlagStatus(UART1_FLAG_RXNE))  //接收标志位
    {
        data = UART1_ReceiveData8();  //读DR同时会清0接收标志位
        queue_write(&Queue_Uart1_Rx, &data, 1);  //将读出的数据保存到接收队列
    }
    
    if(UART1_GetFlagStatus(UART1_FLAG_IDLE))  //空闲标志位
    {
        data = UART1->SR;
        data = UART1_ReceiveData8();  //顺序读完后，清0空闲IDLE标志位
        
        UART1_ITConfig(UART1_IT_IDLE, DISABLE);  //失能空闲中断，防止咬尾
        frame_len_now = Queue_Uart1_Rx.len - queue_data_sum(&Queue_Uart1_Rx_Num);  //当前帧长度 = 当前数据队列长度 - 已保存的总长度
        queue_write(&Queue_Uart1_Rx_Num, &frame_len_now, 1);  //保存当前数据帧长度
        uart_read(UART1_Select);
        UART1_ITConfig(UART1_IT_IDLE, ENABLE);  //使能空闲中断
    }
}
