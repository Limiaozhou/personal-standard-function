#ifndef __MID_LIGHT_H
#define __MID_LIGHT_H

//#include "bsp_iic_master.h"
#include "bsp_iic_simulation_master.h"

uint8_t light_read(float * light_data);  //光照度读取函数，成功返回0，失败返回1

#endif
