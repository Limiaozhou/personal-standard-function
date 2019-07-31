#include "mid_time_count.h"

/* ��ʱ����ʱ���� */
uint8_t Time_Count(pTime_GetTypeDef p)
{
	if(p->key)  //������ʱ����ʱ
	{
		if(p->key_start)
		{
			p->start = time_common;  //���¿�ʼʱ��
			p->count = 0;  //��0����ʱ��
			p->key_start = 0;  //��ʼʱ���������
		}
		
		p->now = time_common;  //��������ʱ��
		
		if(p->now != p->start)
		{
			if(p->now > p->start)
				p->count += p->now - p->start;  //�ۼӼ�ʱ������ʱ��ɳ�����ʱ������ʱ����
			else  //now < start��˵����ʱ��ʱ�䳬����Χ����0��ʼ���¼���
				p->count += TIM_MAX - p->start + p->now + 1;
			
			p->start = p->now;  //���¿�ʼʱ��
			
			if(p->count >= p->set)  //ʱ��ﵽ����ֵ
			{
				p->count = 0;
				return TIME_READY;  //���ؼ�ʱ�ѵ�
			}
		}
		
		return TIME_UNREADY;  //���ؼ�ʱδ��
	}
	else
	{
		p->key_start = 1;  //��ʼʱ���������
		return TIME_UNREADY;
	}
}
