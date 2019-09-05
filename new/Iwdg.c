#include "Iwdg.h"

void IWDG_init(void)
{
	IWDG->KR = 0xCC; //启动IWDG    
	IWDG->KR = 0x55; //解除 PR 及 RLR 的写保护    
	IWDG->RLR = 0xff; //看门狗计数器重装载数值，Tmax = 255 * 4ms = 1.02s
	IWDG->PR = 0x06; //分频系数为256,128K / 2 /256 = 0.25KHz
	IWDG->KR = 0xAA; //刷新IDDG，避免产生看门狗复位，同时恢复 PR 及 RLR 的写保护状态 
}

void IWDG_Feed()
{
	IWDG->KR = 0xAA;  //刷新IDDG，避免产生看门狗复位
}
