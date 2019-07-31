#include "bsp_led.h"

/* LED表，增减只需修改表内初始化和LED最大数量LED_NUM */
static LED_GPIOType Led_Table[LED_NUM] =
{
	{.GPIOx = LED0_GPIO, .Pin = LED0_PIN, .Pull = LED_UP},
	{.GPIOx = LED1_GPIO, .Pin = LED1_PIN, .Pull = LED_UP}
};

/* LED引脚初始化 */
void Led_GPIO_Init(void)
{
	uint8_t i;  //led初始化计数
	
    GPIO_InitTypeDef GPIO_InitStruct;
	
	LED0_GPIO_CLK_ENABLE();  //开启LED相关的GPIO外设时钟
	LED1_GPIO_CLK_ENABLE();
	
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
//    GPIO_InitStruct.Pull = GPIO_PULLUP;          //上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;     //高速
	
	for(i = 0; i < LED_NUM; i++)  //引脚遍历
	{
		if(Led_Table[i].GPIOx)  //引脚有效
		{
			GPIO_InitStruct.Pull = Led_Table[i].Pull;
			GPIO_InitStruct.Pin = Led_Table[i].Pin;
			HAL_GPIO_Init(Led_Table[i].GPIOx, &GPIO_InitStruct);
		}
	}
	
	Led_Write(Led(0), LED_OFF);  //初始关闭led
	Led_Write(Led(1), LED_OFF);
}

/* 写LED口，ledx:LED表号，state:LED状态 */
void Led_Write(uint8_t ledx, LED_State state)
{
	if((ledx < LED_NUM) && Led_Table[ledx].GPIOx)  //端口有效
	{
		if(state != LED_TOGGLE)  //不是电平翻转
		{
			if(Led_Table[ledx].Pull == LED_DOWN)  //下拉，给1有效，ON-1，OFF-0
				HAL_GPIO_WritePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin, (GPIO_PinState)state);
			else  //上拉给0有效
				HAL_GPIO_WritePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin, (GPIO_PinState)!state);
		}
		else
			HAL_GPIO_TogglePin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin); //输出电平翻转
	}
}

/* 读LED口，返回端口状态 */
uint8_t Led_Read(uint8_t ledx)
{
	uint8_t state = 2;  //无效端口返回2
	if((ledx < LED_NUM) && Led_Table[ledx].GPIOx)  //端口有效
		state = HAL_GPIO_ReadPin(Led_Table[ledx].GPIOx, Led_Table[ledx].Pin);
	return state;
}
