#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f7xx_hal.h"

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint32_t Pin;
	uint8_t Pull;
}KEY_InitType;  //KEY初始化结构体

typedef struct
{
	uint8_t valid;  //按下效果
	
	uint8_t start_on;  //按下计时标志
	uint32_t time_on;  //按下计时
	uint8_t start_off;  //松开计时标志
	uint32_t time_off;  //松开计时
	
	uint32_t shake_on_time;  //按下消抖时间
	uint32_t shake_off_time;  //松开消抖时间
	uint32_t long_on_time;  //长按时间
	uint32_t over_off_time;  //松开间隔时间
}KEY_TimeType;  //按键时间结构体

#define KEY_NUM 4  //KEY最大数量

typedef struct
{
	uint8_t key[KEY_NUM];  //按键成功标志
	uint8_t mode[KEY_NUM];  //长短按模式
	uint8_t count[KEY_NUM];  //短按计数
	uint8_t press[KEY_NUM];  //点动模式，按下一直有效，松开立即失效
	
	KEY_TimeType t_table[KEY_NUM];
}KEY_Type;  //按键检测结构体

typedef void (*Key_CallBack)(uint8_t* key, uint8_t* mode, uint8_t* cnt, uint8_t* press);  //按键任务回调函数

#define Key(x) (x)  //按键号

#define Press_Key(x) (0x0C & (1 << (x)))  //选择哪个按键为点动模式，按位选择，1为点动，如C-12-1100，则按键3、4为点动
#define Key_Judge(x, y, z) ((((*((x) + (y))) == (z)) && ((y) < KEY_NUM)) && (!Press_Key(y)))  //KEY输出非点动信息判断，x基地址，y偏移地址，z判断值
#define Press_Judge(x, y, z) ((((*((x) + (y))) == (z)) && ((y) < KEY_NUM)) && Press_Key(y))  //KEY点动判断，x基地址，y偏移地址，z判断值

#define KEY0_PIN               GPIO_PIN_3  //KEY0引脚相关宏定义
#define KEY0_GPIO              GPIOH
#define KEY0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

#define KEY1_PIN               GPIO_PIN_2
#define KEY1_GPIO              GPIOH
#define KEY1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

#define KEY2_PIN               GPIO_PIN_13
#define KEY2_GPIO              GPIOC
#define KEY2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

#define KEY3_PIN               GPIO_PIN_0
#define KEY3_GPIO              GPIOA
#define KEY3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY_NOPULL 0  //GPIO_NOPULL=0
#define KEY_UP     1  //GPIO_PULLUP=1
#define KEY_DOWN   2  //GPIO_PULLUDOWN=2

#define KEY_SUCCESS 1  //按下成功

#define SHORT_MODE 1 //短按模式
#define LONG_MODE  2 //长按模式

#define PRESS_OFF 0  //点动模式松开状态
#define PRESS_ON  1  //点动模式按下状态

#define ON_INVALID  0  //短按无效
#define ON_VALID    1  //短按有效
#define OFF_INVALID 2  //松开无效
#define OFF_VALID   3  //松开有效

void Key_GPIO_Init(void);

void keyx_read(uint8_t keyx);
void key_scan(Key_CallBack task);

#endif
