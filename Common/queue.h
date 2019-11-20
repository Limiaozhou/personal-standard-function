#ifndef __QUEUE_H
#define __QUEUE_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //���������ض������ͷ�ļ�
#endif

//#include "typedef.h"  //���������ض������ͷ�ļ�

//������������
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8λ��intΪ16,32λ��intΪ32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef struct
{
    uint32_t head;  //ͷָ�룬ָ��Ҫ��ȡ���ݵ�ַ
	uint32_t tail;  //βָ�룬ָ��Ҫд�����ݵ�ַ
	
	uint32_t len;  //�����ѱ������ݳ���
	uint32_t queue_size;  //���гߴ�
    
    uint8_t * pdata;  //����ָ��
    uint8_t data_size;  //���ݳߴ磨ռ�ֽ�����
}Queue_TypeDef;  //���нṹ��

typedef Queue_TypeDef* pQueue_TypeDef;

pQueue_TypeDef queue_init(uint32_t queue_size, uint8_t data_size);  //���г�ʼ�������ö������ݳߴ粢�������ݿռ�
void queue_delete(pQueue_TypeDef pqueue);  //����ɾ����ɾ����������ݿռ�

uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //д�����ָ���������ݣ��ɹ�����0��ʧ�ܷ���1
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //��������ָ���������ݣ�����0�Ѷ��������ݣ��ɹ�����0��ʧ�ܷ���1

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //������������ݳ���
uint32_t queue_data_sum(pQueue_TypeDef pqueue);  //��������б��������֮��

#endif
