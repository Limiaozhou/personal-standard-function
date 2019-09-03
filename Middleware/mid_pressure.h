#ifndef __MID_PRESSURE_H
#define __MID_PRESSURE_H

#include "bsp_iic_master.h"

typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;

#define PRES_DEVICE_ADDRESS 0xEE  //设备地址
#define DELAY_CONVERSION 10  //等待ADC转化延时时间

//气压及其温度读取函数，成功返回0，失败返回1
uint8_t pres_temp_read(float * pres_data, float * temp_data);

#endif
