#ifndef __MID_PRESSURE_H
#define __MID_PRESSURE_H

#include "bsp_iic_master.h"

#define PRES_DEVICE_ADDRESS 0xEE  //设备地址
#define DELAY_CONVERSION 12  //ADC转化时间

uint8_t pressure_read(float * data);  //读取气压，返回光气压数据
uint8_t pres_dev_reset(void);  //设备复位，成功返回0，失败返回1

#endif
