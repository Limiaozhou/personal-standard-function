#include "bsp_queue.h"

static unsigned char is_queue_empty(pQueue_TypeDef queue);
static unsigned char is_queue_full(pQueue_TypeDef queue);
static void queue_write_single(pQueue_TypeDef queue, unsigned char data);
static unsigned char queue_read_sigle(pQueue_TypeDef queue);

/* 队列初始化 */
void queue_init(pQueue_TypeDef queue, unsigned char size)
{
	queue->head = 0;
	queue->tail = 0;
	queue->len = 0;
	queue->size = size;
	queue->data = (unsigned char*) malloc(size * sizeof(unsigned char));  //为data分配内存
}

/* 队列删除 */
void queue_delete(pQueue_TypeDef queue)
{
	queue->head = 0;
	queue->tail = 0;
	queue->len = 0;
	queue->size = 0;
	free(queue->data);  //释放data内存
}

/* 判断队列是否为空 */
static unsigned char is_queue_empty(pQueue_TypeDef queue)
{
	if(!queue->len)
		return 1;
	
	return 0;
}

/* 判断队列是否为满 */
static unsigned char is_queue_full(pQueue_TypeDef queue)
{
	if(queue->len >= queue->size)
		return 1;
	
	return 0;
}

/* 写入队列单个数据，移动尾指针 */
static void queue_write_single(pQueue_TypeDef queue, unsigned char data)
{
	if(is_queue_full(queue))
		return;  //队列满时不能写
	
	queue->data[queue->tail] = data;  //写队列尾指针数据
	
	queue->tail = (queue->tail + 1) % queue->size;  //队列尾指针到最大尺寸后从0开始
	queue->len++;  //记录队列中数据长度
}

/* 写入队列指定长度数据，顺移尾指针 */
void queue_write(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len)
{
	if(len <= (queue->size - queue->len))  //要写的长度 <= 队列中空余数据长度
	{
		while(len--)
			queue_write_single(queue, *(pdata++));
	}
}

/* 读出队列单个数据，移动头指针 */
static unsigned char queue_read_sigle(pQueue_TypeDef queue)
{
	unsigned char data;
	
	if(is_queue_empty(queue))
		return 0;  //队列空时不能读
	
	data = queue->data[queue->head];  //读队列头指针数据
	queue->data[queue->head] = 0;  //清0读出后的指针数据
	
	queue->head = (queue->head + 1) % queue->size;  //队列头指针到最大尺寸后从0开始
	queue->len--;  //记录队列中数据长度
	
	return data;
}

/* 读出队列指定长度数据，顺移头指针 */
void queue_read(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len)
{
	if(len <= queue->len)  //要读的长度 <= 队列中数据长度
	{
		while(len--)
			*(pdata++) = queue_read_sigle(queue);
	}
}

/* 计算队列中数据长度 */
unsigned char get_queue_len(pQueue_TypeDef queue)
{
	if(queue->tail >= queue->head)
		queue->len = queue->tail - queue->head;
	else
		queue->len = queue->size - queue->head + queue->tail;
	
	return queue->len;
}

/* 计算队列中保存的数据之和 */
unsigned char queue_data_sum(pQueue_TypeDef queue)
{
	unsigned char len, len_num, len_all = 0;  //已保存帧长队列 每帧长度、帧数、帧总长度
	unsigned char ihead;  //帧长队列头指针

	len_num = queue->len;  //取保存了几个帧长
	ihead = queue->head;

	while(len_num--)
	{
		len = queue->data[ihead];  //取每帧长
		ihead = (ihead + 1) % queue->size;
		len_all += len;  //计算已报存的总帧长
	}

	return len_all;
}
