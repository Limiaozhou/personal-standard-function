#include "bsp_delay.h"

//static unsigned char fac_us=0;  //STM32 us延时倍乘数

/* STM32 延时函数初始化 */
//void Delay_Init(unsigned char sysclk)
//{
//	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //STM32 SysTick频率为HCLK
//	fac_us = sysclk;
//}

/* 延时nus，nus为要延时的us数，注意:nus的值不要大于1000（1ms），STM32时钟摘取法 */
void delay_us(unsigned int nus)
{
    while(nus--)
    {
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
    }
    
    //STM32
//	unsigned long int ticks;  //目标计数值
//	unsigned long int told;  //初始捕获值
//	unsigned long int tnow;  //当前捕获值
//	unsigned long int tcnt=0;  //当前计数值
//	unsigned long int reload = SysTick->LOAD;  //LOAD的值
//	ticks = nus * fac_us;  //需要的节拍数
//	told = SysTick->VAL;  //刚进入时的计数器值
//	while(1)
//	{
//		tnow = SysTick->VAL;
//		if(tnow != told)
//		{
//			if(tnow < told)
//				tcnt += told - tnow;  //SYSTICK是一个递减的24位计数器
//			else
//				tcnt += reload - tnow + told;  //这样累加可跨SysTick的周期
//			
//			told = tnow;
//			
//			if(tcnt >= ticks)
//				break;  //时间超过/等于要延迟的时间,则退出
//		}
//	}
}

/* 延时nms，nms:要延时的ms数 */
void delay_ms(unsigned int nms)
{
    while(nms--)
        delay_us(1000);
}
