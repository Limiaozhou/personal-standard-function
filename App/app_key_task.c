#include "app_key_task.h"

/* 按键任务，判断每个按键成功与否（key）、模式（mode）、短按次数（cnt）、
   按键组合及按键点动（press，修改Press_Key(x)来决定使用哪几个为点动），
   选择点动模式的按键无普通按键功能，判断为Press_Judge()，而普通为Key_Judge() */
void key_task(uint8_t* key, uint8_t* mode, uint8_t* cnt, uint8_t* press)
{
	//按键0按下成功，且按键是有效按键
	if(Key_Judge(key, 0, KEY_SUCCESS))
	{
		if(Key_Judge(mode, 0, LONG_MODE))  //按键0长按模式，且按键是有效按键
		{
			Led_Write(Led(0), LED_ON);
			Led_Write(Led(1), LED_OFF);
		}
		else
		{
			if(Key_Judge(cnt, 0, 1))  //按键0短按次数为1，且按键是有效按键
				Led_Write(Led(0), LED_ON);
			else if(Key_Judge(cnt, 0, 2))
				Led_Write(Led(0), LED_OFF);
		}
	}
	
	if(Key_Judge(key, 1, KEY_SUCCESS))
	{
		if(Key_Judge(mode, 1, LONG_MODE))
		{
			Led_Write(Led(0), LED_OFF);
			Led_Write(Led(1), LED_ON);
		}
		else
		{
			if(Key_Judge(cnt, 1, 1))
				Led_Write(Led(1), LED_ON);
			else if(Key_Judge(cnt, 1, 2))
				Led_Write(Led(1), LED_OFF);
		}
	}
	
	if(Key_Judge(key, 2, KEY_SUCCESS))
	{
		if(Key_Judge(mode, 2, LONG_MODE))
		{
			Led_Write(Led(0), LED_ON);
			Led_Write(Led(1), LED_OFF);
		}
		else
		{
			if(Key_Judge(cnt, 2, 1))
				Led_Write(Led(0), LED_ON);
			else if(Key_Judge(cnt, 2, 2))
				Led_Write(Led(0), LED_OFF);
		}
	}
	
	if(Key_Judge(key, 3, KEY_SUCCESS))
	{
		if(Key_Judge(mode, 3, LONG_MODE))
		{
			Led_Write(Led(0), LED_OFF);
			Led_Write(Led(1), LED_ON);
		}
		else
		{
			if(Key_Judge(cnt, 3, 1))
				Led_Write(Led(1), LED_ON);
			else if(Key_Judge(cnt, 3, 2))
				Led_Write(Led(1), LED_OFF);
		}
	}
	
	//按键同时短按组合模式
	if(Key_Judge(mode, 0, SHORT_MODE) && Key_Judge(mode, 2, SHORT_MODE))
	{
		Led_Write(Led(0), LED_ON);
		Led_Write(Led(1), LED_ON);
	}
	else if(Key_Judge(mode, 1, SHORT_MODE) && Key_Judge(mode, 3, SHORT_MODE))
	{
		Led_Write(Led(0), LED_OFF);
		Led_Write(Led(1), LED_OFF);
	}
	
	//按键点动模式
	if(Press_Judge(press, 0, PRESS_ON))
		Led_Write(Led(0), LED_ON);
	else if(Press_Judge(press, 0, PRESS_OFF))
		Led_Write(Led(0), LED_OFF);
	
	if(Press_Judge(press, 1, PRESS_ON))
		Led_Write(Led(1), LED_ON);
	else if(Press_Judge(press, 1, PRESS_OFF))
		Led_Write(Led(1), LED_OFF);
	
	if(Press_Judge(press, 2, PRESS_ON))
		Led_Write(Led(0), LED_ON);
//	else if(Press_Judge(press, 2, PRESS_OFF))
//		Led_Write(Led(0), LED_OFF);
	
	if(Press_Judge(press, 3, PRESS_ON))
		Led_Write(Led(1), LED_ON);
//	else if(Press_Judge(press, 3, PRESS_OFF))
//		Led_Write(Led(1), LED_OFF);
}
