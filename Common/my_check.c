#include "my_check.h"

//和校验计算
uint16_t check_sum_get(uint8_t * buf, uint16_t len)
{
	uint16_t check_sum = 0;
	
	while(len--)
		check_sum += *(buf++);
	
	return check_sum;
}
