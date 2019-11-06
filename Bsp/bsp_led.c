#include "bsp_led.h"

/* LED表 */
static LED_GPIOType Led_GPIO_Table[number_of_led] =
{
	{.GPIOx = GPIOA, .Pin = GPIO_Pin_4, .active_level = 0},
	{.GPIOx = GPIOA, .Pin = GPIO_Pin_5, .active_level = 0},
    {.GPIOx = GPIOA, .Pin = GPIO_Pin_6, .active_level = 0}
};

/* LED引脚初始化 */
void Led_GPIO_Init(void)
{
	uint8_t i;
	
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	for(i = 0; i < number_of_led; i++)  //引脚遍历
	{
		if(Led_GPIO_Table[i].GPIOx)  //引脚有效
		{
			GPIO_InitStruct.GPIO_Pin = Led_GPIO_Table[i].Pin;
			GPIO_Init(Led_GPIO_Table[i].GPIOx, &GPIO_InitStruct);
		}
	}
	
	Led_GPIO_Write(LED0, LED_OFF);  //初始关闭led
	Led_GPIO_Write(LED1, LED_OFF);
    Led_GPIO_Write(LED2, LED_OFF);
}

/* 写LED口，ledx:LED号，state:LED状态 */
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state)
{
	if((ledx < number_of_led) && Led_GPIO_Table[ledx].GPIOx)  //端口有效
	{
		if(state != LED_TOGGLE)  //不是电平翻转
		{
			if(Led_GPIO_Table[ledx].active_level == 0)
				GPIO_WriteBit(Led_GPIO_Table[ledx].GPIOx, Led_GPIO_Table[ledx].Pin, (BitAction)!state);
			else
				GPIO_WriteBit(Led_GPIO_Table[ledx].GPIOx, Led_GPIO_Table[ledx].Pin, (BitAction)state);
		}
	}
}
