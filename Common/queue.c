#include "queue.h"
#include "stdlib.h"  //内存分配函数包含头文件

//内部函数
static uint8_t is_queue_empty(pQueue_TypeDef pqueue);  //判断队列是否为空，是返回1，否返回0
static uint8_t is_queue_full(pQueue_TypeDef pqueue);  //判断队列是否为满，是返回1，否返回0
static void queue_write_single(pQueue_TypeDef pqueue, void* pdata);  //写入队列单个数据
static void queue_read_single(pQueue_TypeDef pqueue, void* pdata);  //读出队列单个数据
static void queue_read_single_only(pQueue_TypeDef pqueue, void* pdata, uint32_t point);  //读出队列指定位置单个数据

/* 队列初始化，设置队列数据尺寸并分配数据空间 */
pQueue_TypeDef queue_init(uint32_t queue_size, uint8_t data_size)
{
    pQueue_TypeDef pqueue = malloc( sizeof(Queue_TypeDef) );
    
    if(!pqueue)
        return (pQueue_TypeDef)NULL;
    
    pqueue->head = 0;
    pqueue->tail = 0;
    pqueue->len = 0;
    pqueue->queue_size = queue_size;
    pqueue->data_size = data_size;
    pqueue->pdata = (uint8_t *)malloc( queue_size * data_size * sizeof(uint8_t) );  //为data分配内存
    
    if(!pqueue->pdata)
        return (pQueue_TypeDef)NULL;
    
    return pqueue;
}

/* 队列删除，删除分配的数据空间 */
void queue_delete(pQueue_TypeDef pqueue)
{
    if(!pqueue)  //为空
        return;
    
    if(pqueue->pdata)
        free(pqueue->pdata);
    free(pqueue);
}

/* 判断队列是否为空，是返回1，否返回0 */
static uint8_t is_queue_empty(pQueue_TypeDef pqueue)
{
    if( (!pqueue) || (!pqueue->len) )
        return 1;
    
	return 0;
}

/* 判断队列是否为满，是返回1，否返回0 */
static uint8_t is_queue_full(pQueue_TypeDef pqueue)
{
    if( (!pqueue) || (pqueue->len >= pqueue->queue_size) )
        return 1;
    
	return 0;
}

/* 写入队列单个数据，移动尾指针 */
static void queue_write_single(pQueue_TypeDef pqueue, void* pdata)
{  //void * pdata 数据不能++，如强制转换后(uint8_t *)pdata++错误
    uint8_t i;
    uint32_t char_tail = 0;
    
	if(is_queue_full(pqueue))
		return;  //队列满时不能写
	
    char_tail = pqueue->tail * pqueue->data_size;
    
    for(i = 0; i < pqueue->data_size; ++i)
        pqueue->pdata[char_tail + i] = (uint8_t)((uint8_t*)pdata)[i];
	
	pqueue->tail = (pqueue->tail + 1) % pqueue->queue_size;  //队列尾指针到最大尺寸后从0开始
	pqueue->len++;  //记录队列中数据长度
}

/* 写入队列指定长度数据，顺移尾指针，成功返回0，失败返回1 */
uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len)
{
    if(!pqueue)
        return 1;
    
	if(len <= (pqueue->queue_size - pqueue->len))  //要写的长度 <= 队列中空余数据长度
	{
        while(len--)
        {
            queue_write_single(pqueue, pdata);
            pdata = (uint8_t*)pdata + pqueue->data_size;
        }
		
		return 0;  //成功返回0
	}
	
	return 1;  //失败返回1
}

/* 读出队列单个数据，移动头指针 */
static void queue_read_single(pQueue_TypeDef pqueue, void* pdata)
{  //注意不要修改pqueue->pdata，如pqueue->pdata = pqueue->pdata[i]就会出错
    uint8_t i;
    uint32_t char_head = 0;
    
	if(is_queue_empty(pqueue))
		return;  //队列空时不能读
	
    char_head = pqueue->head * pqueue->data_size;
    
    for(i = 0; i < pqueue->data_size; ++i)
        ((uint8_t*)pdata)[i] = pqueue->pdata[char_head + i];
	
	pqueue->head = (pqueue->head + 1) % pqueue->queue_size;  //队列头指针到最大尺寸后从0开始
	pqueue->len--;  //记录队列中数据长度
}

/* 读出队列指定长度数据，顺移头指针，成功返回0，失败返回1 */
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len)
{
    if(!pqueue)
        return 1;
    
	if(len <= pqueue->len)  //要读的长度 <= 队列中数据长度
	{
        while(len--)
        {
            queue_read_single(pqueue, pdata);
            pdata = (uint8_t*)pdata + pqueue->data_size;
        }
		
		return 0;  //成功返回0
	}
	
	return 1;  //失败返回1
}

/* 计算队列中数据长度 */
uint32_t get_queue_len(pQueue_TypeDef pqueue)
{
    if(!pqueue)
        return 0;
    
//	 if(pqueue->tail >= pqueue->head)
//		 pqueue->len = pqueue->tail - pqueue->head;
//	 else
//		 pqueue->len = pqueue->size - pqueue->head + pqueue->tail;
	
	return pqueue->len;
}

/* 读出队列指定位置单个数据 */
static void queue_read_single_only(pQueue_TypeDef pqueue, void* pdata, uint32_t point)
{
    uint8_t i;
    uint32_t char_point = 0;
    
	if(is_queue_empty(pqueue))
		return;
	
    char_point = point * pqueue->data_size;
    
    for(i = 0; i < pqueue->data_size; i++)
        ((uint8_t*)pdata)[i] = pqueue->pdata[char_point + i];
}

/* 计算队列中保存的数据之和 */
uint32_t queue_data_sum(pQueue_TypeDef pqueue)
{
    void * pdata = NULL;
    uint32_t sum = 0;  //数据、数据和
	uint32_t  len;  //数据长度
	uint32_t phead;  //队列头指针缓存
	
    if(!pqueue)
        return 0;
    
    pdata = (uint8_t *)malloc(pqueue->data_size * sizeof(uint8_t));
    
    if(!pdata)
        return 0;
    
	len = pqueue->len;  //取已保存数据长度
	phead = pqueue->head;
    
	while(len--)
    {
        queue_read_single_only(pqueue, pdata, phead);
        if(pqueue->data_size == 1)
            sum += *(uint8_t *)pdata;  //取数据并计算数据和
        else if(pqueue->data_size == 2)
            sum += *(uint16_t *)pdata;
        else
            sum += *(uint32_t *)pdata;
        
        phead = (phead + 1) % pqueue->queue_size;
    }
    
    free(pdata);
	
	return sum;
}
