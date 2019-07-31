#include "bsp_time.h"

//static TIM_HandleTypeDef TIM_CommonStruct;  //STM32 通用定时器结构体

TIME_CountType Time_Table[TIME_NUM];  //定时器计时表

static unsigned char num = 0;  //定时任务注册计数

static void time_count(void);

/*
 *period：自动重装值
 *prescaler：时钟预分频数
 *STM32 定时器溢出时间计算方法:Tout=((period+1)*(prescaler+1))/Ft us
 *Ft=定时器工作频率,单位:Mhz，时钟为HCLK/2
 *STM8 f = fmaster / (2^prescaler)，T = (period+1)/f us
 */
void TIMx_Init(TIMx_Select_TypeDef timex, unsigned long int prescaler, unsigned long int period)
{
    //STM8
    switch(timex)
    {
        case TIM1_Select :
        break;
        
        case TIM2_Select :  //向下计数，period到0
            TIM2_PSCR = prescaler & 0x0F;  //预分频2^prescaler，定时器2
            TIM2_ARRH = period >> 8;  //自动重装值高8位
            TIM2_ARRL = period & 0xFF;  //自动重装值低8位
            TIM2_SR1 &= ~0x01;  //清0更新中断
            TIM2_IER |= 0x01;  //允许更新中断
            TIM2_CR1 |= 0x81;  //允许重装，使能计数
        break;
        
        case TIM3_Select :
        break;
        
        case TIM4_Select :
        break;
        
        case TIM5_Select :
        break;
        
        case TIM6_Select :
        break;
        
        default :
        break;
    }
    
    //STM32
//	TIM_CommonStruct.Instance = TIM_COMMON;  //通用定时器
//    TIM_CommonStruct.Init.Prescaler = prescaler;  //分频
//    TIM_CommonStruct.Init.CounterMode = TIM_COUNTERMODE_UP;  //向上计数器
//    TIM_CommonStruct.Init.Period = period;  //自动装载值
//    TIM_CommonStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //时钟分频因子
//
//	HAL_TIM_Base_Init(&TIM_CommonStruct);  //初始化定时器TIM
//
//	HAL_TIM_Base_Start_IT(&TIM_CommonStruct);	//开启定时器更新中断
}

/* STM32 Init the low level hardware : GPIO, CLOCK, NVIC */
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance == TIM_COMMON)
//	{
//		TIM_COMMON_CLK_ENABLE();  //开启TIM_CLK
//		
//		HAL_NVIC_SetPriority(TIM_COMMON_IRQn, 0, 3);  //设置中断优先级，抢占优先级0，子优先级3
//		HAL_NVIC_EnableIRQ(TIM_COMMON_IRQn);  //开启ITM中断
//	}
//}

/* 定时器计时注册，data:传入计时的时间，返回当前注册的个数，超过最大个数返回0 */
unsigned char time_register(unsigned long int * data)
{
	if(num < TIME_NUM)
	{
		Time_Table[num].t_cnt = data;
		num++;
		return num;
	}
	else
		return 0;
}

/* 开启定时器计时标志 */
void time_start(unsigned char numx)
{
	unsigned char temp = numx - 1;
	
	if((temp < num) && Time_Table[temp].t_cnt && (!Time_Table[temp].t_start))
	{
		Time_Table[temp].t_start = 1;
	}
}

/* 清计时标志及时间 */
void time_clean(unsigned char numx)
{
	unsigned char temp = numx - 1;
	
	if((temp < num) && Time_Table[temp].t_cnt && Time_Table[temp].t_start)
	{
		Time_Table[temp].t_start = 0;
		*Time_Table[temp].t_cnt = 0;
	}
}

/* 定时器计时 */
void time_count(void)
{
	unsigned char i;
	for(i = 0; i < num; i++)
	{
		if(Time_Table[i].t_start)
			(*Time_Table[i].t_cnt)++;
	}
}

//STM8 定时器中断服务函数，1ms中断一次
#pragma vector=TIM2_OVR_UIF_vector  //15
__interrupt void TIM2_OVR_UIF_IRQHandler(void)
{
    TIM2_SR1 &= ~0x01;  //清更新中断标志
    time_count();
}

/* STM32 定时器中断服务函数，1ms中断一次 */
//void TIM_COMMON_IRQHandler(void)
//{
//	HAL_TIM_IRQHandler(&TIM_CommonStruct);
//}

/* TM32 定时器中断回调函数 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM_CommonStruct))
//    {
//		time_count();
////		time_common++;
//    }
//}
