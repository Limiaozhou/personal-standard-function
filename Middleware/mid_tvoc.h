#ifndef __MID_TEMPERATURE_H
#define __MID_TEMPERATURE_H

#include "bsp_iic.h"

#define TVOC_DEVICE_ADDRESS 0x80  //设备地址

//tvoc读取函数，成功返回0，失败返回1
uint8_t tvoc_eco2_read(float * tvoc_data, float * eco2_data);

#endif
