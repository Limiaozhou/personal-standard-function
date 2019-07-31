#include "bsp_time.h"

//static TIM_HandleTypeDef TIM_CommonStruct;  //STM32 ͨ�ö�ʱ���ṹ��

TIME_CountType Time_Table[TIME_NUM];  //��ʱ����ʱ��

static unsigned char num = 0;  //��ʱ����ע�����

static void time_count(void);

/*
 *period���Զ���װֵ
 *prescaler��ʱ��Ԥ��Ƶ��
 *STM32 ��ʱ�����ʱ����㷽��:Tout=((period+1)*(prescaler+1))/Ft us
 *Ft=��ʱ������Ƶ��,��λ:Mhz��ʱ��ΪHCLK/2
 *STM8 f = fmaster / (2^prescaler)��T = (period+1)/f us
 */
void TIMx_Init(TIMx_Select_TypeDef timex, unsigned long int prescaler, unsigned long int period)
{
    //STM8
    switch(timex)
    {
        case TIM1_Select :
        break;
        
        case TIM2_Select :  //���¼�����period��0
            TIM2_PSCR = prescaler & 0x0F;  //Ԥ��Ƶ2^prescaler����ʱ��2
            TIM2_ARRH = period >> 8;  //�Զ���װֵ��8λ
            TIM2_ARRL = period & 0xFF;  //�Զ���װֵ��8λ
            TIM2_SR1 &= ~0x01;  //��0�����ж�
            TIM2_IER |= 0x01;  //��������ж�
            TIM2_CR1 |= 0x81;  //������װ��ʹ�ܼ���
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
//	TIM_CommonStruct.Instance = TIM_COMMON;  //ͨ�ö�ʱ��
//    TIM_CommonStruct.Init.Prescaler = prescaler;  //��Ƶ
//    TIM_CommonStruct.Init.CounterMode = TIM_COUNTERMODE_UP;  //���ϼ�����
//    TIM_CommonStruct.Init.Period = period;  //�Զ�װ��ֵ
//    TIM_CommonStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //ʱ�ӷ�Ƶ����
//
//	HAL_TIM_Base_Init(&TIM_CommonStruct);  //��ʼ����ʱ��TIM
//
//	HAL_TIM_Base_Start_IT(&TIM_CommonStruct);	//������ʱ�������ж�
}

/* STM32 Init the low level hardware : GPIO, CLOCK, NVIC */
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance == TIM_COMMON)
//	{
//		TIM_COMMON_CLK_ENABLE();  //����TIM_CLK
//		
//		HAL_NVIC_SetPriority(TIM_COMMON_IRQn, 0, 3);  //�����ж����ȼ�����ռ���ȼ�0�������ȼ�3
//		HAL_NVIC_EnableIRQ(TIM_COMMON_IRQn);  //����ITM�ж�
//	}
//}

/* ��ʱ����ʱע�ᣬdata:�����ʱ��ʱ�䣬���ص�ǰע��ĸ�������������������0 */
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

/* ������ʱ����ʱ��־ */
void time_start(unsigned char numx)
{
	unsigned char temp = numx - 1;
	
	if((temp < num) && Time_Table[temp].t_cnt && (!Time_Table[temp].t_start))
	{
		Time_Table[temp].t_start = 1;
	}
}

/* ���ʱ��־��ʱ�� */
void time_clean(unsigned char numx)
{
	unsigned char temp = numx - 1;
	
	if((temp < num) && Time_Table[temp].t_cnt && Time_Table[temp].t_start)
	{
		Time_Table[temp].t_start = 0;
		*Time_Table[temp].t_cnt = 0;
	}
}

/* ��ʱ����ʱ */
void time_count(void)
{
	unsigned char i;
	for(i = 0; i < num; i++)
	{
		if(Time_Table[i].t_start)
			(*Time_Table[i].t_cnt)++;
	}
}

//STM8 ��ʱ���жϷ�������1ms�ж�һ��
#pragma vector=TIM2_OVR_UIF_vector  //15
__interrupt void TIM2_OVR_UIF_IRQHandler(void)
{
    TIM2_SR1 &= ~0x01;  //������жϱ�־
    time_count();
}

/* STM32 ��ʱ���жϷ�������1ms�ж�һ�� */
//void TIM_COMMON_IRQHandler(void)
//{
//	HAL_TIM_IRQHandler(&TIM_CommonStruct);
//}

/* TM32 ��ʱ���жϻص����� */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM_CommonStruct))
//    {
//		time_count();
////		time_common++;
//    }
//}
