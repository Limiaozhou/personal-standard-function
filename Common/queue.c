#include "queue.h"

//内部函数
static uint8_t is_queue_empty(pQueue_TypeDef pqueue);  //判断队列是否为空，是返回1，否返回0
static uint8_t is_queue_full(pQueue_TypeDef pqueue);  //判断队列是否为满，是返回1，否返回0
static void queue_write_single(pQueue_TypeDef pqueue, Data_t data);  //写入队列单个数据
static Data_t queue_read_sigle(pQueue_TypeDef pqueue);  //读出队列单个数据

/* 队列初始化，设置队列数据尺寸并分配数据空间 */
void queue_init(pQueue_TypeDef pqueue, uint32_t size)
{
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->len = 0;
	pqueue->size = size;
	pqueue->data = (Data_t*) malloc(size * sizeof(Data_t));  //为data分配内存
}

/* 队列删除，删除分配的数据空间 */
void queue_delete(pQueue_TypeDef pqueue)
{
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->len = 0;
	pqueue->size = 0;
	free(pqueue->data);  //释放data内存
}

/* 判断队列是否为空，是返回1，否返回0 */
static uint8_t is_queue_empty(pQueue_TypeDef pqueue)
{
	if(!pqueue->len)
		return 1;
	
	return 0;
}

/* 判断队列是否为满，是返回1，否返回0 */
static uint8_t is_queue_full(pQueue_TypeDef pqueue)
{
	if(pqueue->len >= pqueue->size)
		return 1;
	
	return 0;
}

/* 写入队列单个数据，移动尾指针 */
static void queue_write_single(pQueue_TypeDef pqueue, Data_t data)
{
	if(is_queue_full(pqueue))
		return;  //队列满时不能写
	
	pqueue->data[pqueue->tail] = data;  //写队列尾指针数据
	
	pqueue->tail = (pqueue->tail + 1) % pqueue->size;  //队列尾指针到最大尺寸后从0开始
	pqueue->len++;  //记录队列中数据长度
}

/* 写入队列指定长度数据，顺移尾指针，成功返回0，失败返回1 */
uint8_t queue_write(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len)
{
	if(len <= (pqueue->size - pqueue->len))  //要写的长度 <= 队列中空余数据长度
	{
		while(len--)
			queue_write_single(pqueue, *(pdata++));
		
		return 0;  //成功返回0
	}
	
	return 1;  //失败返回1
}

/* 读出队列单个数据，移动头指针，并清0已读出的数据 */
static Data_t queue_read_sigle(pQueue_TypeDef pqueue)
{
	Data_t data;
	
	if(is_queue_empty(pqueue))
		return 0;  //队列空时不能读
	
	data = pqueue->data[pqueue->head];  //读队列头指针数据
	pqueue->data[pqueue->head] = 0;  //清0读出后的指针数据
	
	pqueue->head = (pqueue->head + 1) % pqueue->size;  //队列头指针到最大尺寸后从0开始
	pqueue->len--;  //记录队列中数据长度
	
	return data;
}

/* 读出队列指定长度数据，顺移头指针，并清0已读出的数据，成功返回0，失败返回1 */
uint8_t queue_read(pQueue_TypeDef pqueue, Data_t* pdata, uint32_t len)
{
	if(len <= pqueue->len)  //要读的长度 <= 队列中数据长度
	{
		while(len--)
			*(pdata++) = queue_read_sigle(pqueue);
		
		return 0;  //成功返回0
	}
	
	return 1;  //失败返回1
}

/* 计算队列中数据长度 */
uint32_t get_queue_len(pQueue_TypeDef pqueue)
{
	// if(pqueue->tail >= pqueue->head)
		// pqueue->len = pqueue->tail - pqueue->head;
	// else
		// pqueue->len = pqueue->size - pqueue->head + pqueue->tail;
	
	return pqueue->len;
}

/* 计算队列中保存的数据之和 */
uint32_t queue_data_sum(pQueue_TypeDef pqueue)
{
	Data_t data, sum = 0;  //数据、数据和
	uint32_t  len;  //数据长度
	uint32_t phead;  //队列头指针缓存
	
	len = pqueue->len;  //取已保存数据长度
	phead = pqueue->head;
	
	while(len--)
	{
		data = pqueue->data[phead];  //取数据
		phead = (phead + 1) % pqueue->size;
		sum += data;  //计算数据和
	}
	
	return sum;
}
