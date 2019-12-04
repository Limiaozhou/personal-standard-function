#include "check.h"

//16位和校验获取
uint16_t check_sum16_calculate(void * pdata, uint16_t len)
{
    uint8_t * ptemp = pdata;
	uint16_t sum = 0;
	
	while(len--)
		sum += *(ptemp++);
	
	return sum;
}

/***************************************
*@DESCRIPTION: --CRC16 MODBUS计算
*
*@Input: --*pdata：待计算CRC16的数据
*		      len：待计算CRC16数据的长度
*
*@Return: --计算结果
****************************************/	
uint16_t check_crc16_modbus_calc(void * pdata, uint16_t len)
{
    uint8_t * ptemp = pdata;
    uint16_t crc = 0xFFFF;
    uint16_t index_len;
    uint8_t index_bit;
    
    for(index_len = 0; index_len < len; index_len++)
    {
        crc ^= ( (*(ptemp++)) & 0x00FF );
        
        for(index_bit = 0; index_bit < 8; index_bit++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
                crc >>= 1;
        }
    }
    
    return crc;
}
