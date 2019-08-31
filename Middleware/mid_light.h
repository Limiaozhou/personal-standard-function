#ifndef __MID_LIGHT_H
#define __MID_LIGHT_H

#include "bsp_iic_master.h"

void Light_Init(void);  //光照度传感器初始化
float light_read(void);  //读取光照度，返回光照度数据

#endif
