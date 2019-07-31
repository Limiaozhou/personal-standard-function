#include "bsp_key.h"
#include "bsp_tim_common.h"

/* KEY��ʼ��������ֻ���޸ı��ڳ�ʼ����LED�������KEY_NUM */
static KEY_InitType Key_InitTable[KEY_NUM] =
{
	{.GPIOx = KEY0_GPIO, .Pin = KEY0_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY1_GPIO, .Pin = KEY1_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY2_GPIO, .Pin = KEY2_PIN, .Pull = KEY_UP},
	{.GPIOx = KEY3_GPIO, .Pin = KEY3_PIN, .Pull = KEY_DOWN}
};

/* ��������ʼ�� */
static KEY_Type Key =
{
	.t_table[0] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[1] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[2] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
	.t_table[3] = {.shake_on_time = 10, .shake_off_time = 10, .long_on_time = 3000, .over_off_time = 200},
};

/* �������ų�ʼ�� */
void Key_GPIO_Init(void)
{
	uint8_t i;  //key��ʼ������
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	KEY0_GPIO_CLK_ENABLE();  //����KEY��ص�GPIO����ʱ��
	KEY1_GPIO_CLK_ENABLE();
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //����
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;  //����
	
	for(i = 0; i < KEY_NUM; i++)  //���ű���
	{
		if(Key_InitTable[i].GPIOx)  //������Ч
		{
			GPIO_InitStruct.Pull = Key_InitTable[i].Pull;
			GPIO_InitStruct.Pin = Key_InitTable[i].Pin;
			HAL_GPIO_Init(Key_InitTable[i].GPIOx, &GPIO_InitStruct);
		}
	}
}

/* ��ȡKEYx���� */
void keyx_read(uint8_t keyx)
{
	uint8_t state;  //��������ʵʱ״̬
	
	if((keyx < KEY_NUM) && Key_InitTable[keyx].GPIOx)  //��������˿���Ч
	{
		if(!Key.t_table[keyx].start_on)  //û��ע�ᶨʱ����
			Key.t_table[keyx].start_on = time_register(&Key.t_table[keyx].time_on);  //ע�ᰴ�¼�ʱ���񣬻�ö�ʱ�������
		
		if(!Key.t_table[keyx].start_off)  //û��ע�ᶨʱ����
			Key.t_table[keyx].start_off = time_register(&Key.t_table[keyx].time_off);  //ע���ɿ���ʱ���񣬻�ö�ʱ�������
		
		//���״̬������ʱΪ0��Ч������ʱΪ1��Ч��+1���ӦPull������1,2
		state = HAL_GPIO_ReadPin(Key_InitTable[keyx].GPIOx, Key_InitTable[keyx].Pin) + 1;
		
		if(state == Key_InitTable[keyx].Pull)  //����״̬
		{
			time_start(Key.t_table[keyx].start_on);  //���¼�ʱ��ʼ
			
			if(Key.t_table[keyx].valid == OFF_INVALID)  //���ɿ���Ч������
				time_clean(Key.t_table[keyx].start_off);  //���ɿ���ʱ��־��ʱ��
			
			//��������ʱ��
			if(Key.t_table[keyx].time_on >= Key.t_table[keyx].shake_on_time)
			{
				Key.press[keyx] = PRESS_ON;  //�㶯ģʽ����״̬
				
				if(Key.t_table[keyx].valid == OFF_VALID)  //���ɿ���Ч�����£�ֻ��һ��
					time_clean(Key.t_table[keyx].start_off);  //���ɿ���ʱ��־��ʱ��
				
				if(Key.t_table[keyx].time_on >= Key.t_table[keyx].long_on_time)  //���ڳ���ʱ��
				{
					time_clean(Key.t_table[keyx].start_on);  //�尴�¼�ʱ��־��ʱ��
					Key.t_table[keyx].valid = ON_INVALID;  //��0����Ч��
					
					Key.key[keyx] = KEY_SUCCESS;  //�����ɹ�
					Key.mode[keyx] = LONG_MODE;  //����ģʽ
					
					return;
				}
				else
				{
					Key.t_table[keyx].valid = ON_VALID;  //�̰���Ч
					
					Key.mode[keyx] = SHORT_MODE;  //�̰�ģʽ
				}
			}
		}
		else
		{
			if(Key.t_table[keyx].valid != ON_INVALID)  //�̰���Ч���ɿ�����ʼ�ɿ�����
			{
				time_start(Key.t_table[keyx].start_off);  //�ɿ���ʱ��ʼ
				
				//��������ʱ��
				if(Key.t_table[keyx].time_off >= Key.t_table[keyx].shake_off_time)
				{
					Key.press[keyx] = PRESS_OFF;  //��0�㶯ģʽ���㶯ģʽ�ɿ�״̬
					
					if(Key.t_table[keyx].time_off >= Key.t_table[keyx].over_off_time)  //���ڼ��ʱ��
					{
						time_clean(Key.t_table[keyx].start_off);  //���ɿ���ʱ��־��ʱ��
						time_clean(Key.t_table[keyx].start_on);  //�尴�¼�ʱ��־��ʱ��
						Key.t_table[keyx].valid = ON_INVALID;  //��0����Ч��
						
						Key.key[keyx] = KEY_SUCCESS;  //�����ɹ�
						
						return;
					}
					else if(Key.t_table[keyx].valid != OFF_VALID)  //�̰�һ��ִ��һ��
					{
						time_clean(Key.t_table[keyx].start_on);  //�尴�¼�ʱ��־��ʱ��
						
						Key.t_table[keyx].valid = OFF_VALID;  //�ɿ���Ч
						
						Key.count[keyx]++;  //��¼����
					}
				}
				else
					Key.t_table[keyx].valid = OFF_INVALID;  //С������ʱ�䣬�ɿ���Ч
			}
			else  //������Ч���ɿ���0
			{
				time_clean(Key.t_table[keyx].start_on);  //�尴�¼�ʱ��־��ʱ��
				
				Key.press[keyx] = PRESS_OFF;  //��0�㶯ģʽ���㶯ģʽ�ɿ�״̬
			}
		}
	}
}

/* ����ɨ�裬��ִ���û��������ܳ��� */
void key_scan(Key_CallBack task)
{
	uint8_t i;  //����������
	uint8_t cnt = 0;  //��¼����ͬʱ���µĸ���
	
	static uint8_t flag[KEY_NUM] = {0};  //����ͬʱ���º󣬼�¼���ļ������µı�־λ
	
	for(i = 0; i < KEY_NUM; i++)  //���������������һ��������а�������Ϣ
		keyx_read(i);
	
	for(i = 0; i < KEY_NUM; i++)
	{
		if(Key.mode[i] == SHORT_MODE)
			cnt++;  //��¼����ͬʱ���µĸ���
	}
	
	if(cnt > 1)  //��1�����ϰ���ͬʱ���º󣬼�¼���ļ�������
	{
		for(i = 0; i < KEY_NUM; i++)
		{
			if(Key.mode[i] == SHORT_MODE)
				flag[i] = 1;
		}
	}
	
	for(i = 0; i < KEY_NUM; i++)
	{
		if(Key.key[i] && flag[i])  //��0��ϰ����ɹ����������ϰ������ܣ���ֹ�ɿ���������ִ�и��Ե�����������
		{
			flag[i] = 0;
			Key.key[i] = 0;
			Key.count[i] = 0;
		}
	}
	
	task(Key.key, Key.mode, Key.count, Key.press);  //ִ�а�������
	
	for(i = 0; i < KEY_NUM; i++)  //ִ���갴�����ܺ���0���а����������Ϣ
	{
		if(Key.key[i])  //���ɹ�����0��Ӧ������Ϣ
		{
			Key.key[i] = 0;
			Key.count[i] = 0;
		}
		Key.mode[i] = 0;
	}
}
