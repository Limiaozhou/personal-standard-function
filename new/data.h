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
	float light_bhvi;  //����ǿ��
	float TVOC;  //TVOC
	float tvocco2;  //tvoc��CO2
	float temp20;  //�¶�
	float humi20;  //ʪ��
	float press;  //��ѹ
	float press_temp;  //��ѹ�¶�
}EVNDAT;

extern EVNDAT Evndat;

void data_deal(void);

#endif
