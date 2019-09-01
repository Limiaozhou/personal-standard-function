#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdlib.h"  //�ڴ���亯������ͷ�ļ�
#include "stm8s.h"  //���������ض������ͷ�ļ�

//#include "typedef.h"  //���������ض������ͷ�ļ�

//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

//typedef uint8_t Data_t;  //���ζ������ݵ�����

typedef enum
{
    Uint8_Type = 0,
    Int8_Type,
    Uint16_Type,
    Int16_Type,
    Uint32_Type,
    Int32_Type
}DataType_Typedef;

typedef struct
{
	uint32_t head;  //ͷָ�룬ָ��Ҫ��ȡ���ݵ�ַ
	uint32_t tail;  //βָ�룬ָ��Ҫд�����ݵ�ַ
	
	uint32_t len;  //�����ѱ������ݳ���
	uint32_t size;  //�����������ߴ�
	
	void * data;  //����λ
    DataType_Typedef data_type;
}Queue_TypeDef;  //���ݶ��нṹ��

typedef Queue_TypeDef* pQueue_TypeDef;  //ָ����нṹ���ָ��

void queue_init(pQueue_TypeDef pqueue, uint32_t size, DataType_Typedef datatype);  //���г�ʼ�������ö������ݳߴ粢�������ݿռ�
void queue_delete(pQueue_TypeDef pqueue);  //����ɾ����ɾ����������ݿռ�

uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //д�����ָ���������ݣ��ɹ�����0��ʧ�ܷ���1
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //��������ָ���������ݣ�����0�Ѷ��������ݣ��ɹ�����0��ʧ�ܷ���1

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //������������ݳ���
uint32_t queue_data_sum(pQueue_TypeDef pqueue);  //��������б��������֮��

#endif
