#include "bsp_iwdg.h"

//��ʼ���������Ź���ʱ��ԴLSI-40KHz
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��12λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(uint8_t IWDG_Prescaler, uint16_t Reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	
	IWDG_SetPrescaler(IWDG_Prescaler);  //����IWDGԤ��Ƶֵ
	
	IWDG_SetReload(Reload);  //����IWDG��װ��ֵ
	
	IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	
	IWDG_Enable();  //ʹ��IWDG
}

//ι�������Ź�
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();  //reload
}
