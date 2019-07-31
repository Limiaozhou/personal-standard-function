#include "bsp_key.h"
#include "bsp_tim_common.h"

/* KEY初始化表，增减只需修改表内初始化和LED最大数量KEY_NUM */
static KEY_InitType Key_InitTable[KEY_NUM] =
{
	{.GPIOx = KEY0_GPIO, .Pin = KEY0_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY1_GPIO, .Pin = KEY1_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY2_GPIO, .Pin = KEY2_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY3_GPIO, .Pin = KEY3_PIN, .Pull = KEY_DOWN}
};

/* 按键检测初始化 */
static KEY_Type Key =
{
	.t_table[0] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[1] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[2] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[3] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
};

/* 按键引脚初始化 */
void Key_GPIO_Init(void)
{
	uint8_t i;  //key初始化计数
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	KEY0_GPIO_CLK_ENABLE();  //开启KEY相关的GPIO外设时钟
	KEY1_GPIO_CLK_ENABLE();
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //输入
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;  //高速
	
	for(i = 0; i < KEY_NUM; i++)  //引脚遍历
	{
		if(Key_InitTable[i].GPIOx)  //引脚有效
		{
			GPIO_InitStruct.Pull = Key_InitTable[i].Pull;
			GPIO_InitStruct.Pin = Key_InitTable[i].Pin;
			HAL_GPIO_Init(Key_InitTable[i].GPIOx, &GPIO_InitStruct);
		}
	}
}

/* 读取KEYx引脚 */
void keyx_read(uint8_t keyx)
{
	uint8_t state;  //按键引脚实时状态
	
	if((keyx < KEY_NUM) && Key_InitTable[keyx].GPIOx)  //按键输入端口有效
	{
		if(!Key.t_table[keyx].start_on)  //没有注册定时器过
			Key.t_table[keyx].start_on = time_register(&Key.t_table[keyx].time_on);  //注册按下计时任务，获得定时器任务号
		
		if(!Key.t_table[keyx].start_off)  //没有注册定时器过
			Key.t_table[keyx].start_off = time_register(&Key.t_table[keyx].time_off);  //注册松开计时任务，获得定时器任务号
		
		//检测状态，上拉时为0有效，下拉时为1有效，+1后对应Pull上下垃1,2
		state = HAL_GPIO_ReadPin(Key_InitTable[keyx].GPIOx, Key_InitTable[keyx].Pin) + 1;
		
		if(state == Key_InitTable[keyx].Pull)  //按下状态
		{
			time_start(Key.t_table[keyx].start_on);  //按下计时开始
			
			if(Key.t_table[keyx].valid == OFF_INVALID)  //由松开无效到按下
				time_clean(Key.t_table[keyx].start_off);  //清松开计时标志及时间
			
			//大于消抖时间
			if(Key.t_table[keyx].time_on >= Key.t_table[keyx].shake_on_time)
			{
				Key.press[keyx] = PRESS_ON;  //点动模式按下状态
				
				if(Key.t_table[keyx].valid == OFF_VALID)  //由松开有效到按下，只清一次
					time_clean(Key.t_table[keyx].start_off);  //清松开计时标志及时间
				
				if(Key.t_table[keyx].time_on >= Key.t_table[keyx].long_on_time)  //大于长按时间
				{
					time_clean(Key.t_table[keyx].start_on);  //清按下计时标志及时间
					Key.t_table[keyx].valid = ON_INVALID;  //清0按键效果
					
					Key.key[keyx] = KEY_SUCCESS;  //按键成功
					Key.mode[keyx] = LONG_MODE;  //长按模式
					
					return;
				}
				else
				{
					Key.t_table[keyx].valid = ON_VALID;  //短按有效
					
					Key.mode[keyx] = SHORT_MODE;  //短按模式
				}
			}
		}
		else
		{
			if(Key.t_table[keyx].valid != ON_INVALID)  //短按有效后松开，开始松开消抖
			{
				time_start(Key.t_table[keyx].start_off);  //松开计时开始
				
				//大于消抖时间
				if(Key.t_table[keyx].time_off >= Key.t_table[keyx].shake_off_time)
				{
					Key.press[keyx] = PRESS_OFF;  //清0点动模式，点动模式松开状态
					
					if(Key.t_table[keyx].time_off >= Key.t_table[keyx].over_off_time)  //大于间隔时间
					{
						time_clean(Key.t_table[keyx].start_off);  //清松开计时标志及时间
						time_clean(Key.t_table[keyx].start_on);  //清按下计时标志及时间
						Key.t_table[keyx].valid = ON_INVALID;  //清0按键效果
						
						Key.key[keyx] = KEY_SUCCESS;  //按键成功
						
						return;
					}
					else if(Key.t_table[keyx].valid != OFF_VALID)  //短按一次执行一次
					{
						time_clean(Key.t_table[keyx].start_on);  //清按下计时标志及时间
						
						Key.t_table[keyx].valid = OFF_VALID;  //松开有效
						
						Key.count[keyx]++;  //记录次数
					}
				}
				else
					Key.t_table[keyx].valid = OFF_INVALID;  //小于消抖时间，松开无效
			}
			else  //按下无效后松开清0
			{
				time_clean(Key.t_table[keyx].start_on);  //清按下计时标志及时间
				
				Key.press[keyx] = PRESS_OFF;  //清0点动模式，点动模式松开状态
			}
		}
	}
}

/* 按键扫描，并执行用户按键功能程序 */
void key_scan(Key_CallBack task)
{
	uint8_t i;  //按键检测计数
	uint8_t cnt = 0;  //记录按键同时按下的个数
	
	static uint8_t flag[KEY_NUM] = {0};  //按键同时按下后，记录是哪几个按下的标志位
	
	for(i = 0; i < KEY_NUM; i++)  //按键检测遍历，最后一起输出所有按键的信息
		keyx_read(i);
	
	for(i = 0; i < KEY_NUM; i++)
	{
		if(Key.mode[i] == SHORT_MODE)
			cnt++;  //记录按键同时按下的个数
	}
	
	if(cnt > 1)  //有1个以上按键同时按下后，记录是哪几个按下
	{
		for(i = 0; i < KEY_NUM; i++)
		{
			if(Key.mode[i] == SHORT_MODE)
				flag[i] = 1;
		}
	}
	
	for(i = 0; i < KEY_NUM; i++)
	{
		if(Key.key[i] && flag[i])  //清0组合按键成功，以完成组合按键功能，防止松开按键后，又执行各自单独按键功能
		{
			flag[i] = 0;
			Key.key[i] = 0;
			Key.count[i] = 0;
		}
	}
	
	task(Key.key, Key.mode, Key.count, Key.press);  //执行按键功能
	
	for(i = 0; i < KEY_NUM; i++)  //执行完按键功能后，清0所有按键输出的信息
	{
		if(Key.key[i])  //按成功后，清0对应按键信息
		{
			Key.key[i] = 0;
			Key.count[i] = 0;
		}
		Key.mode[i] = 0;
	}
}
