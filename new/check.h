#ifndef __CHECK_h
#define __CHECK_h

//#include <stdbool.h>
#include "stm8s.h"
  
//添加累积和
void Check_Addsum(unsigned char *pdat, unsigned int len);
//累积和检验
bool Check_Checksum(unsigned char *pdat, unsigned int len);
//CRC16计算
void Check_CalaCRC16(unsigned char *pdat, unsigned int len);
//CRC16检验
bool Check_CheckCRC16(unsigned char *pdat, unsigned int len);
#endif
