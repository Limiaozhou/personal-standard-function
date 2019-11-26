#include "bsp_iwdg.h"

//初始化独立看门狗，时钟源LSI-40KHz
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低12位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(uint8_t IWDG_Prescaler, uint16_t Reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	
	IWDG_SetPrescaler(IWDG_Prescaler);  //设置IWDG预分频值
	
	IWDG_SetReload(Reload);  //设置IWDG重装载值
	
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
	
	IWDG_Enable();  //使能IWDG
}

//喂独立看门狗
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();  //reload
}
