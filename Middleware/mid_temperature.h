#ifndef __MID_TEMPERATURE_H
#define __MID_TEMPERATURE_H

#include "bsp_iic_master.h"

typedef enum
{
	Temp_Host = 0xE3,
	Humi_Host = 0xE5,
	Temp_NoHost = 0xF3,
	Humi_NoHost = 0xF5
}Temp_Reg_TypeDef;

#define TEMP_DEVICE_ADDRESS 0x80  //设备地址
#define TEMP_DEVICE_RESET 0xFE  //设备地址

#define DELAY_TEMP 90  //等待ADC转化延时时间

//温湿度读取函数，成功返回0，失败返回1
uint8_t temp_humi_read(float * temp_data, float * humi_data);

#endif
