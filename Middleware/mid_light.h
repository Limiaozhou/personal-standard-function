#ifndef __MID_LIGHT_H
#define __MID_LIGHT_H

#include "bsp_iic_simulation_master.h"

#define LIGHT_PORT Pe6Pe7
#define LIGHT_DEVICE_ADR 0x94

uint8_t light_read(float * light_data);  //���նȶ�ȡ�������ɹ�����0��ʧ�ܷ���1

#endif
