#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdlib.h"  //内存分配函数包含头文件
#include "stm8s.h"  //数据类型重定义包含头文件

//#include "typedef.h"  //数据类型重定义包含头文件

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

//typedef uint8_t Data_t;  //环形队列数据的类型

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
	uint32_t head;  //头指针，指向要读取数据地址
	uint32_t tail;  //尾指针，指向要写入数据地址
	
	uint32_t len;  //队列已保存数据长度
	uint32_t size;  //队列数据最大尺寸
	
	void * data;  //数据位
    DataType_Typedef data_type;
}Queue_TypeDef;  //数据队列结构体

typedef Queue_TypeDef* pQueue_TypeDef;  //指向队列结构体的指针

void queue_init(pQueue_TypeDef pqueue, uint32_t size, DataType_Typedef datatype);  //队列初始化，设置队列数据尺寸并分配数据空间
void queue_delete(pQueue_TypeDef pqueue);  //队列删除，删除分配的数据空间

uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //写入队列指定长度数据，成功返回0，失败返回1
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //读出队列指定长度数据，并清0已读出的数据，成功返回0，失败返回1

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //计算队列中数据长度
uint32_t queue_data_sum(pQueue_TypeDef pqueue);  //计算队列中保存的数据之和

#endif
