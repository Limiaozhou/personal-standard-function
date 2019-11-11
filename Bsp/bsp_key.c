#include "bsp_key.h"

/* KEY表 */
static KEY_GPIOType Key_GPIO_List[number_of_key] = KEY_GPIO_LIST;

/* KEY引脚初始化 */
void Key_GPIO_Init(void)
{
	uint8_t i;
	
    GPIO_InitTypeDef GPIO_InitStruct;
	
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	for(i = 0; i < number_of_key; i++)  //引脚遍历
	{
		if(Key_GPIO_List[i].GPIOx)  //引脚有效
		{
            RCC_APB2PeriphClockCmd(Key_GPIO_List[i].RCC_APB2Periph, ENABLE);
            
			GPIO_InitStruct.GPIO_Pin = Key_GPIO_List[i].Pin;
            GPIO_InitStruct.GPIO_Mode = Key_GPIO_List[i].Mode;
			GPIO_Init(Key_GPIO_List[i].GPIOx, &GPIO_InitStruct);
		}
	}
}

uint8_t Key_GPIO_Read(KEY_GPIO_Port keyx)
{
    uint8_t bitstatus = 0;
    
    if((keyx < number_of_key) && Key_GPIO_List[keyx].GPIOx)
        bitstatus = GPIO_ReadInputDataBit(Key_GPIO_List[keyx].GPIOx, Key_GPIO_List[keyx].Pin);
    
    return bitstatus;
}
