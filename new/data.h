#ifndef __DATA_H
#define __DATA_H

#include "stdio.h"

#include "check.h"
#include "mem.h"
#include "Iwdg.h"
#include "co2.h"
#include "i2c_eeprom.h"

#include "mid_uart.h"
#include "mid_light.h"
#include "mid_pressure.h"
#include "mid_temperature.h"
#include "mid_tvoc.h"

typedef struct
{
	float co2;  //CO2
	float pm25;  //pm2.5
	float light_bhvi;  //光照强度
	float TVOC;  //TVOC
	float tvocco2;  //tvoc的CO2
	float temp20;  //温度
	float humi20;  //湿度
	float press;  //气压
	float press_temp;  //气压温度
}EVNDAT;

extern EVNDAT Evndat;

void data_deal(void);

#endif
