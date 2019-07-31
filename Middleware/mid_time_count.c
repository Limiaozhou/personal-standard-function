#include "mid_time_count.h"

/* 定时器计时函数 */
uint8_t Time_Count(pTime_GetTypeDef p)
{
	if(p->key)  //开启定时器计时
	{
		if(p->key_start)
		{
			p->start = time_common;  //更新开始时间
			p->count = 0;  //清0计数时间
			p->key_start = 0;  //开始时间更新锁关
		}
		
		p->now = time_common;  //更新现在时间
		
		if(p->now != p->start)
		{
			if(p->now > p->start)
				p->count += p->now - p->start;  //累加计时，计数时间可超过定时器最大计时周期
			else  //now < start，说明定时器时间超过范围，从0开始重新计数
				p->count += TIM_MAX - p->start + p->now + 1;
			
			p->start = p->now;  //更新开始时间
			
			if(p->count >= p->set)  //时间达到设置值
			{
				p->count = 0;
				return TIME_READY;  //返回计时已到
			}
		}
		
		return TIME_UNREADY;  //返回计时未到
	}
	else
	{
		p->key_start = 1;  //开始时间更新锁开
		return TIME_UNREADY;
	}
}
