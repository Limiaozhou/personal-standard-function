#include "app_key_task.h"

/* ���������ж�ÿ�������ɹ����key����ģʽ��mode�����̰�������cnt����
   ������ϼ������㶯��press���޸�Press_Key(x)������ʹ���ļ���Ϊ�㶯����
   ѡ��㶯ģʽ�İ�������ͨ�������ܣ��ж�ΪPress_Judge()������ͨΪKey_Judge() */
void key_task(uint8_t* key, uint8_t* mode, uint8_t* cnt, uint8_t* press)
{
	//����0���³ɹ����Ұ�������Ч����
	if(Key_Judge(key, 0, KEY_SUCCESS))
	{
		if(Key_Judge(mode, 0, LONG_MODE))  //����0����ģʽ���Ұ�������Ч����
		{
			Led_Write(Led(0), LED_ON);
			Led_Write(Led(1), LED_OFF);
		}
		else
		{
			if(Key_Judge(cnt, 0, 1))  //����0�̰�����Ϊ1���Ұ�������Ч����
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
	
	//����ͬʱ�̰����ģʽ
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
	
	//�����㶯ģʽ
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
