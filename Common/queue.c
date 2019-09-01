#include "queue.h"

//内部函数
static uint8_t is_queue_empty(pQueue_TypeDef pqueue);  //判断队列是否为空，是返回1，否返回0
static uint8_t is_queue_full(pQueue_TypeDef pqueue);  //判断队列是否为满，是返回1，否返回0
static void queue_write_single(pQueue_TypeDef pqueue, void* pdata);  //写入队列单个数据
static void queue_read_single(pQueue_TypeDef pqueue, void* pdata);  //读出队列单个数据

/* 队列初始化，设置队列数据尺寸并分配数据空间 */
void queue_init(pQueue_TypeDef pqueue, uint32_t size, DataType_Select_Typedef datatype)
{
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->len = 0;
	pqueue->size = size;
    pqueue->data_type = datatype;
    switch(pqueue->data_type)
    {
        case Uint8_Type:
            pqueue->data.data_uint8 = (uint8_t*) malloc(size * sizeof(uint8_t));  //为data分配内存
        break;
        
        case Int8_Type:
            pqueue->data.data_int8 = (int8_t*) malloc(size * sizeof(int8_t));
        break;
        
        case Uint16_Type:
            pqueue->data.data_uint16 = (uint16_t*) malloc(size * sizeof(uint16_t));
        break;
        
        case Int16_Type:
            pqueue->data.data_int16 = (int16_t*) malloc(size * sizeof(int16_t));
        break;
        
        case Uint32_Type:
            pqueue->data.data_uint32 = (uint32_t*) malloc(size * sizeof(uint32_t));
        break;
        
        case Int32_Type:
            pqueue->data.data_int32 = (int32_t*) malloc(size * sizeof(int32_t));
        break;
        
        default:
            pqueue->data.data_uint8 = (uint8_t*) malloc(size * sizeof(uint8_t));
        break;
    }
}

/* 队列删除，删除分配的数据空间 */
void queue_delete(pQueue_TypeDef pqueue)
{
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->len = 0;
	pqueue->size = 0;
    switch(pqueue->data_type)
    {
        case Uint8_Type:
            free(pqueue->data.data_uint8);  //释放data内存
        break;
        
        case Int8_Type:
            free(pqueue->data.data_int8);
        break;
        
        case Uint16_Type:
            free(pqueue->data.data_uint16);
        break;
        
        case Int16_Type:
            free(pqueue->data.data_int16);
        break;
        
        case Uint32_Type:
            free(pqueue->data.data_uint32);
        break;
        
        case Int32_Type:
            free(pqueue->data.data_int32);
        break;
        
        default:
            free(pqueue->data.data_uint8);
        break;
    }
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
static void queue_write_single(pQueue_TypeDef pqueue, void* pdata)
{
	if(is_queue_full(pqueue))
		return;  //队列满时不能写
	
    switch(pqueue->data_type)
    {
        case Uint8_Type:
            pqueue->data.data_uint8[pqueue->tail] = *(uint8_t*)pdata;  //写队列尾指针数据
        break;
        
        case Int8_Type:
            pqueue->data.data_int8[pqueue->tail] = *(int8_t*)pdata;  //写队列尾指针数据
        break;
        
        case Uint16_Type:
            pqueue->data.data_uint16[pqueue->tail] = *(uint16_t*)pdata;  //写队列尾指针数据
        break;
        
        case Int16_Type:
            pqueue->data.data_int16[pqueue->tail] = *(int16_t*)pdata;  //写队列尾指针数据
        break;
        
        case Uint32_Type:
            pqueue->data.data_uint32[pqueue->tail] = *(uint32_t*)pdata;  //写队列尾指针数据
        break;
        
        case Int32_Type:
            pqueue->data.data_int32[pqueue->tail] = *(int32_t*)pdata;  //写队列尾指针数据
        break;
        
        default:
            pqueue->data.data_uint8[pqueue->tail] = *(uint8_t*)pdata;  //写队列尾指针数据
        break;
    }
	
	pqueue->tail = (pqueue->tail + 1) % pqueue->size;  //队列尾指针到最大尺寸后从0开始
	pqueue->len++;  //记录队列中数据长度
}

/* 写入队列指定长度数据，顺移尾指针，成功返回0，失败返回1 */
uint8_t queue_write(pQueue_TypeDef pqueue, void* pdata, uint32_t len)
{
	if(len <= (pqueue->size - pqueue->len))  //要写的长度 <= 队列中空余数据长度
	{
        switch(pqueue->data_type)
        {
            case Uint8_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (uint8_t*)pdata + 1;
                }
            break;
            
            case Int8_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (int8_t*)pdata + 1;
                }
            break;
            
            case Uint16_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (uint16_t*)pdata + 1;
                }
            break;
            
            case Int16_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (int16_t*)pdata + 1;
                }
            break;
            
            case Uint32_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (uint32_t*)pdata + 1;
                }
            break;
            
            case Int32_Type:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (int32_t*)pdata + 1;
                }
            break;
            
            default:
                while(len--)
                {
                    queue_write_single(pqueue, pdata);
                    pdata = (uint8_t*)pdata + 1;
                }
            break;
        }
		
		return 0;  //成功返回0
	}
	
	return 1;  //失败返回1
}

/* 读出队列单个数据，移动头指针，并清0已读出的数据 */
static void queue_read_single(pQueue_TypeDef pqueue, void* pdata)
{
	if(is_queue_empty(pqueue))
		return;  //队列空时不能读
	
    switch(pqueue->data_type)
    {
        case Uint8_Type:
            *(uint8_t*)pdata = pqueue->data.data_uint8[pqueue->head];  //读队列头指针数据
            pqueue->data.data_uint8[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        case Int8_Type:
            *(int8_t*)pdata = pqueue->data.data_int8[pqueue->head];  //读队列头指针数据
            pqueue->data.data_int8[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        case Uint16_Type:
            *(uint16_t*)pdata = pqueue->data.data_uint16[pqueue->head];  //读队列头指针数据
            pqueue->data.data_uint16[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        case Int16_Type:
            *(int16_t*)pdata = pqueue->data.data_int16[pqueue->head];  //读队列头指针数据
            pqueue->data.data_int16[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        case Uint32_Type:
            *(uint32_t*)pdata = pqueue->data.data_uint32[pqueue->head];  //读队列头指针数据
            pqueue->data.data_uint32[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        case Int32_Type:
            *(int32_t*)pdata = pqueue->data.data_int32[pqueue->head];  //读队列头指针数据
            pqueue->data.data_int32[pqueue->head] = 0;  //清0读出后的指针数据
        break;
        
        default:
            *(uint8_t*)pdata = pqueue->data.data_uint8[pqueue->head];  //读队列头指针数据
            pqueue->data.data_uint8[pqueue->head] = 0;  //清0读出后的指针数据
        break;
    }
	
	pqueue->head = (pqueue->head + 1) % pqueue->size;  //队列头指针到最大尺寸后从0开始
	pqueue->len--;  //记录队列中数据长度
}

/* 读出队列指定长度数据，顺移头指针，并清0已读出的数据，成功返回0，失败返回1 */
uint8_t queue_read(pQueue_TypeDef pqueue, void* pdata, uint32_t len)
{
	if(len <= pqueue->len)  //要读的长度 <= 队列中数据长度
	{
        switch(pqueue->data_type)
        {
            case Uint8_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (uint8_t*)pdata + 1;
                }
            break;
            
            case Int8_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (int8_t*)pdata + 1;
                }
            break;
            
            case Uint16_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (uint16_t*)pdata + 1;
                }
            break;
            
            case Int16_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (int16_t*)pdata + 1;
                }
            break;
            
            case Uint32_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (uint32_t*)pdata + 1;
                }
            break;
            
            case Int32_Type:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (int32_t*)pdata + 1;
                }
            break;
            
            default:
                while(len--)
                {
                    queue_read_single(pqueue, pdata);
                    pdata = (uint8_t*)pdata + 1;
                }
            break;
        }
		
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
	uint32_t sum = 0;  //数据、数据和
	uint32_t  len;  //数据长度
	uint32_t phead;  //队列头指针缓存
	
	len = pqueue->len;  //取已保存数据长度
	phead = pqueue->head;
	
    switch(pqueue->data_type)
    {
        case Uint8_Type:
            while(len--)
            {
                sum += pqueue->data.data_uint8[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        case Int8_Type:
            while(len--)
            {
                sum += pqueue->data.data_int8[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        case Uint16_Type:
            while(len--)
            {
                sum += pqueue->data.data_uint16[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        case Int16_Type:
            while(len--)
            {
                sum += pqueue->data.data_int16[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        case Uint32_Type:
            while(len--)
            {
                sum += pqueue->data.data_uint32[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        case Int32_Type:
            while(len--)
            {
                sum += pqueue->data.data_int32[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
        
        default:
            while(len--)
            {
                sum += pqueue->data.data_uint8[phead];  //取数据并计算数据和
                phead = (phead + 1) % pqueue->size;
            }
        break;
    }
	
	return sum;
}
