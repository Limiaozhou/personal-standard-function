#ifndef __MID_TVOC_H
#define __MID_TVOC_H

#include "bsp_iic.h"
#include "data.h"

#define TVOC_DEVICE_ADDRESS 0xA2  //�豸��ַ

uint8_t tvoc_eco2_read_start(void);  //tvoc����co2��ȡ��ʼ�������ɹ�����0��ʧ�ܷ���1
void tvoc_data_deal(uint8_t * buf, uint16_t len);  //tvoc���ݴ�����

#endif
