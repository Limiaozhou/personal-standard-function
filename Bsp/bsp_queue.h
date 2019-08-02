#ifndef __BSP_QUEUE_H
#define __BSP_QUEUE_H

#include <stdlib.h>

//������������
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int uint16_t;
typedef int int16_t;  //8λ��intΪ16,32λ��intΪ32
typedef unsigned long int uint32_t;
typedef long int int32_t;

typedef int32_t Data_t;  //���ζ������ݵ�����

typedef struct
{
	uint16_t head;  //ͷָ�룬ָ��Ҫ��ȡ���ݵ�ַ
	uint16_t tail;  //βָ�룬ָ��Ҫд�����ݵ�ַ
	
	uint16_t len;  //�����ѱ������ݳ���
	uint16_t size;  //�����������ߴ�
	
	Data_t *data;  //����λ
}Queue_TypeDef;  //���ݶ��нṹ��

typedef Queue_TypeDef* pQueue_TypeDef;  //ָ����нṹ���ָ��

void queue_init(pQueue_TypeDef queue, uint16_t size);
void queue_delete(pQueue_TypeDef queue);

void queue_write(pQueue_TypeDef queue, Data_t *pdata, uint16_t len);
void queue_read(pQueue_TypeDef queue, Data_t *pdata, uint16_t len);

uint16_t get_queue_len(pQueue_TypeDef queue);
Data_t queue_data_sum(pQueue_TypeDef queue);

#endif
