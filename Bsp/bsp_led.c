#include "bsp_led.h"

/* LED�� */
static LED_GPIOType Led_GPIO_List[number_of_led] = LED_GPIO_LIST;

/* LED���ų�ʼ�� */
void Led_GPIO_Init(void)
{
	uint8_t i;
	
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	for(i = 0; i < number_of_led; i++)  //���ű���
	{
		if(Led_GPIO_List[i].GPIOx)  //������Ч
		{
			GPIO_InitStruct.GPIO_Pin = Led_GPIO_List[i].Pin;
			GPIO_Init(Led_GPIO_List[i].GPIOx, &GPIO_InitStruct);
            
            Led_GPIO_Write((LED_GPIO_Port)i, LED_OFF);  //��ʼ�ر�led
		}
	}
}

/* дLED�ڣ�ledx:LED�ţ�state:LED״̬ */
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state)
{
    uint8_t bitstatus_read = 0;
    
	if((ledx < number_of_led) && Led_GPIO_List[ledx].GPIOx)  //�˿���Ч
	{
		if(state != LED_TOGGLE)  //���ǵ�ƽ��ת
		{
			if(Led_GPIO_List[ledx].active_level == 0)
				GPIO_WriteBit(Led_GPIO_List[ledx].GPIOx, Led_GPIO_List[ledx].Pin, (BitAction)!state);
			else
				GPIO_WriteBit(Led_GPIO_List[ledx].GPIOx, Led_GPIO_List[ledx].Pin, (BitAction)state);
		}
        else
        {
            bitstatus_read = Led_GPIO_Read(ledx);
            GPIO_WriteBit(Led_GPIO_List[ledx].GPIOx, Led_GPIO_List[ledx].Pin, (BitAction)!bitstatus_read);
        }
            
	}
}

uint8_t Led_GPIO_Read(LED_GPIO_Port ledx)
{
    uint8_t bitstatus = 0;
    
    if((ledx < number_of_led) && Led_GPIO_List[ledx].GPIOx)
        bitstatus = GPIO_ReadOutputDataBit(Led_GPIO_List[ledx].GPIOx, Led_GPIO_List[ledx].Pin);
    
    return bitstatus;
}
