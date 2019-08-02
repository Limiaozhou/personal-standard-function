#include "bsp_queue.h"

//内部函数
static uint8_t is_queue_empty(pQueue_TypeDef queue);  //判断队列是否为空
static uint8_t is_queue_full(pQueue_TypeDef queue);  //判断队列是否为满
static void queue_write_single(pQueue_TypeDef queue, Data_t data);  //写入队列单个数据
static Data_t queue_read_sigle(pQueue_TypeDef queue);  //读出队列单个数据

/* 队列初始化，设置队列数据尺寸并分配数据空间 */
void queue_init(pQueue_TypeDef queue, uint16_t size)
{
	queue->head = 0;
	queue->tail = 0;
	queue->len = 0;
	queue->size = size;
	queue->data = (Data_t*) malloc(size * sizeof(Data_t));  //为data分配内存
}

/* 队列删除，删除分配的数据空间 */
void queue_delete(pQueue_TypeDef queue)
{
	queue->head = 0;
	queue->tail = 0;
	queue->len = 0;
	queue->size = 0;
	free(queue->data);  //释放data内存
}

/* 判断队列是否为空 */
static uint8_t is_queue_empty(pQueue_TypeDef queue)
{
	if(!queue->len)
		return 1;
	
	return 0;
}

/* 判断队列是否为满 */
static uint8_t is_queue_full(pQueue_TypeDef queue)
{
	if(queue->len >= queue->size)
		return 1;
	
	return 0;
}

/* 写入队列单个数据，移动尾指针 */
static void queue_write_single(pQueue_TypeDef queue, Data_t data)
{
	if(is_queue_full(queue))
		return;  //队列满时不能写
	
	queue->data[queue->tail] = data;  //写队列尾指针数据
	
	queue->tail = (queue->tail + 1) % queue->size;  //队列尾指针到最大尺寸后从0开始
	queue->len++;  //记录队列中数据长度
}

/* 写入队列指定长度数据，顺移尾指针 */
void queue_write(pQueue_TypeDef queue, Data_t *pdata, uint16_t len)
{
	if(len <= (queue->size - queue->len))  //要写的长度 <= 队列中空余数据长度
	{
		while(len--)
			queue_write_single(queue, *(pdata++));
	}
}

/* 读出队列单个数据，移动头指针，并清0已读出的数据 */
static Data_t queue_read_sigle(pQueue_TypeDef queue)
{
	Data_t data;
	
	if(is_queue_empty(queue))
		return 0;  //队列空时不能读
	
	data = queue->data[queue->head];  //读队列头指针数据
	queue->data[queue->head] = 0;  //清0读出后的指针数据
	
	queue->head = (queue->head + 1) % queue->size;  //队列头指针到最大尺寸后从0开始
	queue->len--;  //记录队列中数据长度
	
	return data;
}

/* 读出队列指定长度数据，顺移头指针，并清0已读出的数据 */
void queue_read(pQueue_TypeDef queue, Data_t *pdata, uint16_t len)
{
	if(len <= queue->len)  //要读的长度 <= 队列中数据长度
	{
		while(len--)
			*(pdata++) = queue_read_sigle(queue);
	}
}

/* 计算队列中数据长度 */
uint16_t get_queue_len(pQueue_TypeDef queue)
{
	// if(queue->tail >= queue->head)
		// queue->len = queue->tail - queue->head;
	// else
		// queue->len = queue->size - queue->head + queue->tail;
	
	return queue->len;
}

/* 计算队列中保存的数据之和 */
Data_t queue_data_sum(pQueue_TypeDef queue)
{
	Data_t data, sum = 0;  //数据、数据和
	uint16_t  len;  //数据长度
	uint16_t phead;  //队列头指针缓存

	len = queue->len;  //取已保存数据长度
	phead = queue->head;

	while(len--)
	{
		data = queue->data[phead];  //取每帧长
		phead = (phead + 1) % queue->size;
		sum += data;  //计算已报存的总帧长
	}

	return sum;
}
