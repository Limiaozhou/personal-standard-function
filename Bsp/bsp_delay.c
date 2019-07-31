#include "bsp_delay.h"

//static unsigned char fac_us=0;  //STM32 us��ʱ������

/* STM32 ��ʱ������ʼ�� */
//void Delay_Init(unsigned char sysclk)
//{
//	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //STM32 SysTickƵ��ΪHCLK
//	fac_us = sysclk;
//}

/* ��ʱnus��nusΪҪ��ʱ��us����ע��:nus��ֵ��Ҫ����1000��1ms����STM32ʱ��ժȡ�� */
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
//	unsigned long int ticks;  //Ŀ�����ֵ
//	unsigned long int told;  //��ʼ����ֵ
//	unsigned long int tnow;  //��ǰ����ֵ
//	unsigned long int tcnt=0;  //��ǰ����ֵ
//	unsigned long int reload = SysTick->LOAD;  //LOAD��ֵ
//	ticks = nus * fac_us;  //��Ҫ�Ľ�����
//	told = SysTick->VAL;  //�ս���ʱ�ļ�����ֵ
//	while(1)
//	{
//		tnow = SysTick->VAL;
//		if(tnow != told)
//		{
//			if(tnow < told)
//				tcnt += told - tnow;  //SYSTICK��һ���ݼ���24λ������
//			else
//				tcnt += reload - tnow + told;  //�����ۼӿɿ�SysTick������
//			
//			told = tnow;
//			
//			if(tcnt >= ticks)
//				break;  //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�
//		}
//	}
}

/* ��ʱnms��nms:Ҫ��ʱ��ms�� */
void delay_ms(unsigned int nms)
{
    while(nms--)
        delay_us(1000);
}
