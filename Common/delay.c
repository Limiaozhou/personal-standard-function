#include "delay.h"

static uint8_t fac_us = 0;  //us延时倍乘数，静态全局变量，单独此.c用

/* 延时函数初始化，初始化延时基准 */
void Delay_Init(uint8_t sysclk)
{
#if defined STM8
	if(sysclk >= 16)
		fac_us = (16 - 4) / 4;
	else if(sysclk > 4)
		fac_us = (sysclk - 4) / 4;
	else
		fac_us = 1;
	
#elif defined STM32_HAL
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //配置SysTick频率为HCLK
    fac_us = sysclk;
    
#elif defined STM32_STANDARD
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    fac_us = sysclk;
#endif
}

/* 延时nus，nus为要延时的us数，STM32注意:nus的值不要大于1000（1ms），时钟摘取法 */
void delay_us(uint16_t nus)
{
#if defined STM8
    //延时时间=(fac_us*4+4)*nus*(T)，16M对应（3*4+4）*nus/16=nus us
	//其中,T为CPU运行频率(Mhz)的倒数,单位为us
	__asm(
	"PUSH A \n"  //1T，压栈
	"DELAY_XUS: \n"  //代码段名
	"LD A,fac_us \n"  //1T，fac_us加载到累加器A
	"DELAY_US_1: \n"  //代码段名
	"NOP \n"  //1T,nop延时
	"DEC A \n"  //1T,A--
	"JRNE DELAY_US_1 \n"  //A不等于0,则跳转(2T)到DELAY_US_1继续执行,若等于0,则不跳转(1T)
	"NOP \n"  //1T,nop延时
	"DECW X \n"   //1T,x--，对应nus
	"JRNE DELAY_XUS \n"  //X不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T)
	"POP A \n"  //1T,出栈
	);
	
#elif defined (STM32_HAL) || defined (STM32_STANDARD)
	uint32_t ticks;  //目标计数值
	uint32_t told;  //初始捕获值
	uint32_t tnow;  //当前捕获值
	uint32_t tcnt=0;  //当前计数值
	uint32_t reload = SysTick->LOAD;  //LOAD的值
	ticks = nus * fac_us;  //需要的节拍数
	told = SysTick->VAL;  //刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told)
				tcnt += told - tnow;  //SYSTICK是一个递减的24位计数器
			else
				tcnt += reload - tnow + told;  //这样累加可跨SysTick的周期
			
			told = tnow;
			
			if(tcnt >= ticks)
				break;  //时间超过/等于要延迟的时间,则退出
		}
	}
#endif
}

/* 延时nms，nms:要延时的ms数 */
void delay_ms(uint32_t nms)
{
#if defined STM8
	//减少while判断与跳转使用，增加准确性
	uint8_t t = 0;
	
	if(nms > 65)
	{
		t = nms / 65;
		while(t--)
			delay_us(65000);
		nms = nms % 65;
	}
	delay_us(nms * 1000);
	
#elif defined (STM32_HAL) || defined (STM32_STANDARD)
    while(nms--)
        delay_us(1000);
#endif
}
