#ifndef __MID_PRESSURE_H
#define __MID_PRESSURE_H

#include "bsp_iic_master.h"

#define PRES_DEVICE_ADDRESS 0xEE  //�豸��ַ
#define DELAY_CONVERSION 12  //ADCת��ʱ��

uint8_t pressure_read(float * data);  //��ȡ��ѹ�����ع���ѹ����
uint8_t pres_dev_reset(void);  //�豸��λ���ɹ�����0��ʧ�ܷ���1

#endif
