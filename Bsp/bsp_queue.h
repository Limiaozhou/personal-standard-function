#ifndef __BSP_QUEUE_H
#define __BSP_QUEUE_H

#include <stdlib.h>

typedef struct
{
	unsigned char head;  //头指针，指向要读取数据地址
	unsigned char tail;  //尾指针，指向要写入数据地址
	
	unsigned char len;  //数据长度
	unsigned char size;  //队列数据最大尺寸
	
	unsigned char *data;  //数据位
}Queue_TypeDef;  //数据队列结构体

typedef Queue_TypeDef* pQueue_TypeDef;  //指向队列结构体的指针

void queue_init(pQueue_TypeDef queue, unsigned char size);
void queue_delete(pQueue_TypeDef queue);

void queue_write(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len);
void queue_read(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len);

unsigned char get_queue_len(pQueue_TypeDef queue);
unsigned char queue_data_sum(pQueue_TypeDef queue);

#endif
