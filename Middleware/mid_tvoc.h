#ifndef __MID_TVOC_H
#define __MID_TVOC_H

#include "bsp_iic.h"
#include "data.h"

#define TVOC_DEVICE_ADDRESS 0xA2  //设备地址

uint8_t tvoc_eco2_read_start(void);  //tvoc及其co2读取开始函数，成功返回0，失败返回1
void tvoc_data_deal(uint8_t * buf, uint16_t len);  //tvoc数据处理函数

#endif
