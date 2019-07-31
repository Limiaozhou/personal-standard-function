#include "bsp_led.h"

/* LED������ֻ���޸ı��ڳ�ʼ����LED�������LED_NUM */
static LED_GPIOType Led_Table[LED_NUM] =
{
	{.GPIOx = LED0_GPIO, .Pin = LED0_PIN, .Pull = LED_UP},
	{.GPIOx = LED1_GPIO, .Pin = LED1_PIN, .Pull = LED_UP}
};

/* LED���ų�ʼ�� */
void Led_GPIO_Init(void)
{
	uint8_t i;  //led��ʼ������
	
    GPIO_InitTypeDef GPIO_InitStruct;
	
	LED0_GPIO_CLK_ENABLE();  //����LED��ص�GPIO����ʱ��
	LED1_GPIO_CLK_ENABLE();
	
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_InitStruct.Pull = GPIO_PULLUP;          //����
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;     //����
	
	for(i = 0; i < LED_NUM; i++)  //���ű���
	{
		if(Led_Table[i].GPIOx)  //������Ч
		{
			GPIO_InitStruct.Pull = Led_Table[i].Pull;
			GPIO_InitStruct.Pin = Led_Table[i].Pin;
			HAL_GPIO_Init(Led_Table[i].GPIOx, &GPIO_InitStruct);
		}
	}
	
	Led_Write(Led(0), LED_OFF);  //��ʼ�ر�led
	Led_Write(Led(1), LED_OFF);
}

/* дLED�ڣ�ledx:LED��ţ�state:LED״̬ */
void Led_Write(uint8_t ledx, LED_State state)
{
	if((ledx < LED_NUM) && Led_Table[ledx].GPIOx)  //�˿���Ч
	{
		if(state != LED_TOGGLE)  //���ǵ�ƽ��ת
		{
			if(Led_Table[ledx].Pull == LED_DOWN)  //��������1��Ч��ON-1��OFF-0
				HAL_GPIO_WritePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin, (GPIO_PinState)state);
			else  //������0��Ч
				HAL_GPIO_WritePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin, (GPIO_PinState)!state);
		}
		else
			HAL_GPIO_TogglePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin); //�����ƽ��ת
	}
}

/* ��LED�ڣ����ض˿�״̬ */
uint8_t Led_Read(uint8_t ledx)
{
	uint8_t state = 2;  //��Ч�˿ڷ���2
	if((ledx < LED_NUM) && Led_Table[ledx].GPIOx)  //�˿���Ч
		state = HAL_GPIO_ReadPin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin);
	return state;
}
