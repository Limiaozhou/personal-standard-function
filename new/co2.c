#include "co2.h"
u8 CO2_flage=0;
u16 CO2_cnt=0,CO2_cnt_save=0;
u32 CO2temp_s=0;



void init_Co2()
{
    GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_IN_FL_IT);//���ô��ڸ�������ʹ���ⲿ�ж�
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);//�˿�D�������ش����ж�
}


void Init_Timer4(void) 
{
    TIM4_Cmd(ENABLE);//ʹ�ܼ�����
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0xff);//���÷�Ƶϵ����������װֵ
    
    
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//�����ж�ʹ��
    TIM4_GenerateEvent(TIM4_EVENTSOURCE_UPDATE);//���������³�ʼ������������
    
    TIM4_SetCounter(0xff);//���ü�������ֵ
    TIM4_SetAutoreload(0xff);//�����Զ���װ��ֵ
    
}
void Get_CO2(float *tempfs)
{
  float data;
  if(CO2_flage==0)
  {
    
      data=CO2_cnt_save*2.04*3.992;//��λms  CO2 ��λPPM
      CO2_cnt_save=0;
      
      if(data<4200 &&data>380)
        *tempfs=data;
  }

}

//#pragma vector= EXTIC_vector
// __interrupt void  EXTI_PORTC_IRQHandler(void)
//{
//
//}

