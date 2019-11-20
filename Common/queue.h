#ifndef __QUEUE_H
#define __QUEUE_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

//#include "typedef.h"  //数据类型重定义包含头文件

//数据类型声明
// typedef unsigned char uint8_t;
// typedef signed char int8_t;
// typedef unsigned int uint16_t;
// typedef signed int int16_t;  //8位机int为16,32位机int为32
// typedef unsigned long int uint32_t;
// typedef signed long int int32_t;

typedef struct
{
    uint32_t head;  //头指针，指向要读取数据地址
	uint32_t tail;  //尾指针，指向要写入数据地址
	
	uint32_t len;  //队列已保存数据长度
	uint32_t queue_size;  //队列尺寸
    
    uint8_t * pdata;  //数据指针
    uint8_t data_size;  //数据尺寸（占字节数）
}Queue_TypeDef;  //队列结构体

typedef Queue_TypeDef* pQueue_TypeDef;

pQueue_TypeDef queue_init(uint32_t queue_size, uint8_t data_size);  //队列初始化，设置队列数据尺寸并分配数据空间
void queue_delete(pQueue_TypeDef pqueue);  //队列删除，删除分配的数据空间

uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //写入队列指定长度数据，成功返回0，失败返回1
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len);  //读出队列指定长度数据，并清0已读出的数据，成功返回0，失败返回1

uint32_t get_queue_len(pQueue_TypeDef pqueue);  //计算队列中数据长度
uint32_t queue_data_sum(pQueue_TypeDef pqueue);  //计算队列中保存的数据之和

#endif
