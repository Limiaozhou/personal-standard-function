#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdlib.h"
#include "typedef.h"

//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef int32_t Data_t;  //���ζ������ݵ�����

typedef struct
{
	uint32_t head;  //ͷָ�룬ָ��Ҫ��ȡ���ݵ�ַ
	uint32_t tail;  //βָ�룬ָ��Ҫд�����ݵ�ַ
	
	uint32_t len;  //�����ѱ������ݳ���
	uint32_t size;  //�����������ߴ�
	
	Data_t* data;  //����λ
}Queue_TypeDef;  //���ݶ��нṹ��

typedef Queue_TypeDef* pQueue_TypeDef;  //ָ����нṹ���ָ��

void queue_init(pQueue_TypeDef pqueue, uint32_t size);  //���г�ʼ�������ö������ݳߴ粢�������ݿռ�
void queue_delete(pQueue_TypeDef pqueue);  //����ɾ����ɾ����������ݿռ�

void uint8_t queue_write(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len);  //д�����ָ����������
void uint8_t queue_read(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len);  //��������ָ���������ݣ�����0�Ѷ���������

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //������������ݳ���
Data_t queue_data_sum(pQueue_TypeDef pqueue);  //��������б��������֮��

#endif
