#include "mid_pm.h"

/* uart3��ȡ���ݴ��� pdata������ָ�룬 len�����ݳ��� */
void pm_read_deal(uint8_t * pdata, uint32_t len)
{
    uint32_t i = 0;  //��������
    uint8_t head_buf[2] = {0x42, 0x4D};  //֡ͷ
    uint16_t check_sum = 0;  //��У��
	
    while((len--) >= 32)
    {
        if((pdata[0+i] == head_buf[0]) && (pdata[1+i] == head_buf[1]))
        {
			check_sum = check_sum_get(&pdata[0+i], 30);
			if(check_sum == (((uint16_t)pdata[30+i] << 8) + pdata[31+i]))
			{
				Evndat.pm25 = ((uint16_t)pdata[12+i] << 8) + pdata[13+i];
				break;
			}
        }
        i++;
    }
}
