#ifndef __MID_TIME_COUNT_H
#define __MID_TIME_COUNT_H

#include "stm32f7xx_hal.h"
#include "bsp_tim_common.h"

typedef struct
{
	uint32_t start;  //��ʼʱ��
	uint32_t now;  //����ʱ��
	uint64_t set;  //����ʱ��
	uint64_t count;  //����ʱ��
	
	uint8_t key_start;  //��ʼʱ�������
	uint8_t key;  //��ʱ����
}Time_GetTypeDef;  //��ʱ��ʱ���ȡ�ṹ��

typedef Time_GetTypeDef* pTime_GetTypeDef;  //��ʱ��ʱ���ȡ�ṹ��ָ��

#define TIME_READY   1
#define TIME_UNREADY 0

uint8_t Time_Count(pTime_GetTypeDef p);  //��ʱ����ʱ����

#endif
