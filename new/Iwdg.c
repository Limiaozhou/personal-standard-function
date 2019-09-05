#include "Iwdg.h"

void IWDG_init(void)
{
	IWDG->KR = 0xCC; //����IWDG    
	IWDG->KR = 0x55; //��� PR �� RLR ��д����    
	IWDG->RLR = 0xff; //���Ź���������װ����ֵ��Tmax = 255 * 4ms = 1.02s
	IWDG->PR = 0x06; //��Ƶϵ��Ϊ256,128K / 2 /256 = 0.25KHz
	IWDG->KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬ 
}

void IWDG_Feed()
{
	IWDG->KR = 0xAA;  //ˢ��IDDG������������Ź���λ
}
