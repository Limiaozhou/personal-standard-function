#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f7xx_hal.h"

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint32_t Pin;
	uint8_t Pull;
}KEY_InitType;  //KEY��ʼ���ṹ��

typedef struct
{
	uint8_t valid;  //����Ч��
	
	uint8_t start_on;  //���¼�ʱ��־
	uint32_t time_on;  //���¼�ʱ
	uint8_t start_off;  //�ɿ���ʱ��־
	uint32_t time_off;  //�ɿ���ʱ
	
	uint32_t shake_on_time;  //��������ʱ��
	uint32_t shake_off_time;  //�ɿ�����ʱ��
	uint32_t long_on_time;  //����ʱ��
	uint32_t over_off_time;  //�ɿ����ʱ��
}KEY_TimeType;  //����ʱ��ṹ��

#define KEY_NUM 4  //KEY�������

typedef struct
{
	uint8_t key[KEY_NUM];  //�����ɹ���־
	uint8_t mode[KEY_NUM];  //���̰�ģʽ
	uint8_t count[KEY_NUM];  //�̰�����
	uint8_t press[KEY_NUM];  //�㶯ģʽ������һֱ��Ч���ɿ�����ʧЧ
	
	KEY_TimeType t_table[KEY_NUM];
}KEY_Type;  //�������ṹ��

typedef void (*Key_CallBack)(uint8_t* key, uint8_t* mode, uint8_t* cnt, uint8_t* press);  //��������ص�����

#define Key(x) (x)  //������

#define Press_Key(x) (0x0C & (1 << (x)))  //ѡ���ĸ�����Ϊ�㶯ģʽ����λѡ��1Ϊ�㶯����C-12-1100���򰴼�3��4Ϊ�㶯
#define Key_Judge(x, y, z) ((((*((x) + (y))) == (z)) && ((y) < KEY_NUM)) && (!Press_Key(y)))  //KEY����ǵ㶯��Ϣ�жϣ�x����ַ��yƫ�Ƶ�ַ��z�ж�ֵ
#define Press_Judge(x, y, z) ((((*((x) + (y))) == (z)) && ((y) < KEY_NUM)) && Press_Key(y))  //KEY�㶯�жϣ�x����ַ��yƫ�Ƶ�ַ��z�ж�ֵ

#define KEY0_PIN               GPIO_PIN_3  //KEY0������غ궨��
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

#define KEY_SUCCESS 1  //���³ɹ�

#define SHORT_MODE 1 //�̰�ģʽ
#define LONG_MODE  2 //����ģʽ

#define PRESS_OFF 0  //�㶯ģʽ�ɿ�״̬
#define PRESS_ON  1  //�㶯ģʽ����״̬

#define ON_INVALID  0  //�̰���Ч
#define ON_VALID    1  //�̰���Ч
#define OFF_INVALID 2  //�ɿ���Ч
#define OFF_VALID   3  //�ɿ���Ч

void Key_GPIO_Init(void);

void keyx_read(uint8_t keyx);
void key_scan(Key_CallBack task);

#endif
