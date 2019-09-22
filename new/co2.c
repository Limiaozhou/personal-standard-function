#include "co2.h"
u8 CO2_flage=0;
u16 CO2_cnt=0,CO2_cnt_save=0;
u32 CO2temp_s=0;


int mlml;
void init_Co2()
{
    GPIO_Init(GPIOD, GPIO_PIN_7,GPIO_MODE_IN_FL_NO_IT);//设置串口浮空输入使能外部中断
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_RISE_ONLY);
    //EXTI_GetTLISensitivity();
    //EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
    GPIO_Init(GPIOD, GPIO_PIN_7,GPIO_MODE_IN_FL_IT);
     
    //EXTI_GetTLISensitivity();
     
//端口D仅上升沿触发中断
}


void Init_Timer4(void) 
{
    TIM4_Cmd(ENABLE);//使能计数器
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0xff);//设置分频系数，设置重装值，T=2.048ms
    
    
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//更新中断使能
    TIM4_GenerateEvent(TIM4_EVENTSOURCE_UPDATE);//计数器重新初始化并产生更新
    
    TIM4_SetCounter(0xff);//设置计数器的值
    TIM4_SetAutoreload(0xff);//设置自动重装的值
    
}
void Get_CO2(float *tempfs)
{
  float data;
  if(CO2_flage==0)
  {
    
      data=CO2_cnt_save*2.04*3.992;//单位ms  CO2 单位PPM
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

