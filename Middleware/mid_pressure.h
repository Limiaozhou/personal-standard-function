#ifndef __MID_PRESSURE_H
#define __MID_PRESSURE_H

#include "bsp_iic_master.h"

typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;

#define PRES_DEVICE_ADDRESS 0xEE  //�豸��ַ
#define DELAY_CONVERSION 10  //�ȴ�ADCת����ʱʱ��

//��ѹ�����¶ȶ�ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t pres_temp_read(float * pres_data, float * temp_data);

#endif
