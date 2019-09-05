////////////////////////////////////////////////////////////////////////////////// 
//内存操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

#include "mem.h"

/***************************************
*@DESCRIPTION: --清零
*
*@Input: --pdest： 目的指针   len：数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	
//清除串口接收函数
void mem_clear(unsigned char *pdest, int len)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = 0;
    }
}

/***************************************
*@DESCRIPTION: --清零
*
*@Input: --pdest： 目的指针   len：数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	
/*
void mem_fclear(float*pdest, int len)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = 0;
    }
}*/
/***************************************
*@DESCRIPTION: --置值
*
*@Input: --pdest： 目的指针   len：数据长度  dat:设置数据
*
*@Out: --无
*
*@Return: --无
****************************************/	

void mem_set(unsigned char *pdest, int len,unsigned char dat)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = dat;
    }
}

/***************************************
*@DESCRIPTION: --复制
*
*@Input: --pdest： 目的指针   psrc：源指针  len：数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	

void mem_copy(unsigned char *pdest, unsigned char *psrc,int len)
{ 
	  int i;
    for( i=0;i<len; i++)
    {
      *pdest++ = *psrc++;
    }
}
void mem_copy_convert_port(unsigned char *pdest, unsigned char *psrc,int len)
{ 
    int i;
    unsigned char *temp=psrc+len-1;
    for( i=0;i<len; i++)
    {
      *pdest++ = *temp--;
    }
}

/***************************************
*@DESCRIPTION: --复制
*
*@Input: --pdest： 目的指针   psrc：源指针  len：数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	
/*
void mem_fcopy(float *pdest, float *psrc,unsigned char len)
{ 
	  int i;
    for( i=0;i<len; i++)
    {
      *pdest++ = *psrc++;
    }
}
*/

