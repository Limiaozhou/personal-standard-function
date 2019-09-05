////////////////////////////////////////////////////////////////////////////////// 
//内存操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

#ifndef __MEM_H
#define __MEM_H

#include "string.h"

/******************@DESCRIPTION: --清零**************/	
extern void mem_clear(unsigned char*pdest, int len);
extern void mem_fclear(float*pdest, int len);
/******************@DESCRIPTION: --置值**************/	
extern void mem_set(unsigned char *pdest, int len,unsigned char dat);
/******************@DESCRIPTION: --复制*************/	
extern void mem_copy(unsigned char *pdest, unsigned char *psrc,int len);
extern void mem_fcopy(float *pdest, float *psrc,unsigned char len);

extern char* find_str(const char *s1, const char *s2,int len,int len2);  
extern void mem_copy_convert_port(unsigned char *pdest, unsigned char *psrc,int len);
#endif
