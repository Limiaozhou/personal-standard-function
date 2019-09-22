#ifndef __MID_TEMPERATURE_H
#define __MID_TEMPERATURE_H

#include "bsp_iic_simulation_master.h"

typedef enum
{
	Temp_Host = 0xE3,
	Humi_Host = 0xE5,
	Temp_NoHost = 0xF3,
	Humi_NoHost = 0xF5
}Temp_Reg_TypeDef;

#define TEMP_PORT Pb6Pb7
#define TEMP_DEVICE_ADDRESS 0x80  //�豸��ַ

//��ʪ�ȶ�ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t temp_humi_read(float * temp_data, float * humi_data);

#endif
