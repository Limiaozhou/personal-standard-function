#include "mid_pm.h"

/* uart3读取数据处理， pdata：数据指针， len：数据长度 */
void pm_read_deal(uint8_t * pdata, uint32_t len)
{
    uint32_t i = 0;  //数组索引
    uint8_t head_buf[2] = {0x42, 0x4D};  //帧头
    uint16_t check_sum = 0;  //和校验
	
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
