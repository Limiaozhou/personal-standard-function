#include "delay.h"

static uint8_t fac_us = 0;  //us��ʱ����������̬ȫ�ֱ�����������.c��

/* ��ʱ������ʼ������ʼ����ʱ��׼ */
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
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //����SysTickƵ��ΪHCLK
    fac_us = sysclk;
    
#elif defined STM32_STANDARD
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    fac_us = sysclk;
#endif
}

/* ��ʱnus��nusΪҪ��ʱ��us����STM32ע��:nus��ֵ��Ҫ����1000��1ms����ʱ��ժȡ�� */
void delay_us(uint16_t nus)
{
#if defined STM8
    //��ʱʱ��=(fac_us*4+4)*nus*(T)��16M��Ӧ��3*4+4��*nus/16=nus us
	//����,TΪCPU����Ƶ��(Mhz)�ĵ���,��λΪus
	__asm(
	"PUSH A \n"  //1T��ѹջ
	"DELAY_XUS: \n"  //�������
	"LD A,fac_us \n"  //1T��fac_us���ص��ۼ���A
	"DELAY_US_1: \n"  //�������
	"NOP \n"  //1T,nop��ʱ
	"DEC A \n"  //1T,A--
	"JRNE DELAY_US_1 \n"  //A������0,����ת(2T)��DELAY_US_1����ִ��,������0,����ת(1T)
	"NOP \n"  //1T,nop��ʱ
	"DECW X \n"   //1T,x--����Ӧnus
	"JRNE DELAY_XUS \n"  //X������0,����ת(2T)��DELAY_XUS����ִ��,������0,����ת(1T)
	"POP A \n"  //1T,��ջ
	);
	
#elif defined (STM32_HAL) || defined (STM32_STANDARD)
	uint32_t ticks;  //Ŀ�����ֵ
	uint32_t told;  //��ʼ����ֵ
	uint32_t tnow;  //��ǰ����ֵ
	uint32_t tcnt=0;  //��ǰ����ֵ
	uint32_t reload = SysTick->LOAD;  //LOAD��ֵ
	ticks = nus * fac_us;  //��Ҫ�Ľ�����
	told = SysTick->VAL;  //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told)
				tcnt += told - tnow;  //SYSTICK��һ���ݼ���24λ������
			else
				tcnt += reload - tnow + told;  //�����ۼӿɿ�SysTick������
			
			told = tnow;
			
			if(tcnt >= ticks)
				break;  //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�
		}
	}
#endif
}

/* ��ʱnms��nms:Ҫ��ʱ��ms�� */
void delay_ms(uint32_t nms)
{
#if defined STM8
	//����while�ж�����תʹ�ã�����׼ȷ��
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
