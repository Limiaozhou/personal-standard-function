////////////////////////////////////////////////////////////////////////////////// 
//校验算法
//xj
//版本：V1.0
//修改日期:2015/8/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

#include "check.h"

/***************************************
*@DESCRIPTION: --校验和计算
*
*@Input: --*pdat：待计算校验和的数据
*			 len：待计算校验和数据的长度
*
*@Return: --无
****************************************/	
void Check_Addsum(unsigned char *pdat, unsigned int len)
{
	int i, sum=0;
	for(i = 0; i < len; i++)
		sum += *pdat++;
	
	*pdat++ = sum >> 8;
	*pdat = sum & 0xFF;
}

/***************************************
*@DESCRIPTION: --校验和计算是否通过
*
*@Input: --*pdat：待计算校验和的数据
*			 len：待计算校验和数据的长度
*
*@Return: --bool：通过返回true，否则返回false
****************************************/	
bool Check_Checksum(unsigned char *pdat, unsigned int len)
{
   int i,sum=0,crc=0;
   for (i = 0; i < len; i++)
   {     
     sum += *pdat++;
   }

    crc = (*pdat++) << 8;
    crc += *pdat++;

    if (sum == crc) return TRUE;

    return FALSE;
}

//CRC16计算
/***************************************
*@DESCRIPTION: --CRC16计算
*
*@Input: --*pdat：待计算CRC16的数据
*			 len：待计算CRC16数据的长度
*
*@Return: --无
****************************************/	
void Check_CalaCRC16(unsigned char *pdat, unsigned int len)
{
  unsigned int i;
  unsigned char j;
  unsigned int crc = 0xFFFF;
  for (i=0; i<len; i++)
  {
    crc ^= ((*(pdat++)) & 0x00FF);
    for (j=0; j<8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      { 
        crc >>= 1;
      }
    }
  }
  
  *pdat++ = crc & 0xFF;
  *pdat = crc>>8;
}

//CRC16计算
/***************************************
*@DESCRIPTION: --CRC16计算是否通过
*
*@Input: --*pdat：待计算CRC16的数据
*			 len：待计算CRC16数据的长度
*
*@Return: --bool：通过返回true，否则返回false
****************************************/	
bool Check_CheckCRC16(unsigned char *pdat, unsigned int len)
{
  unsigned int i,sum;
  unsigned char j;
  unsigned int crc = 0xFFFF;
  for (i=0; i<len; i++)
  {
    crc ^= ((*pdat++) & 0x00FF);
    for (j=0; j<8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      { 
        crc >>= 1;
      }
    }
  }
  
  sum = *pdat++;
  sum += (*pdat) << 8;
  if (sum == crc) return TRUE;

  return FALSE;
}
