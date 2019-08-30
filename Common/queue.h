#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdlib.h"
#include "typedef.h"

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef int32_t Data_t;  //环形队列数据的类型

typedef struct
{
	uint32_t head;  //头指针，指向要读取数据地址
	uint32_t tail;  //尾指针，指向要写入数据地址
	
	uint32_t len;  //队列已保存数据长度
	uint32_t size;  //队列数据最大尺寸
	
	Data_t* data;  //数据位
}Queue_TypeDef;  //数据队列结构体

typedef Queue_TypeDef* pQueue_TypeDef;  //指向队列结构体的指针

void queue_init(pQueue_TypeDef pqueue, uint32_t size);  //队列初始化，设置队列数据尺寸并分配数据空间
void queue_delete(pQueue_TypeDef pqueue);  //队列删除，删除分配的数据空间

void uint8_t queue_write(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len);  //写入队列指定长度数据
void uint8_t queue_read(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len);  //读出队列指定长度数据，并清0已读出的数据

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //计算队列中数据长度
Data_t queue_data_sum(pQueue_TypeDef pqueue);  //计算队列中保存的数据之和

#endif
