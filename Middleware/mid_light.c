#include "mid_light.h"

float light_read(void)
{
	uint8_t reg_adr = 0x00;
	uint8_t buf[4] = {0};
	uint32_t data = 0;
	float light = 0;
	
	IIC_Master_ReadRegister(1, 0x94, &reg_adr, 1, 1, 15, buf, 4);
	data = buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
	light = (float)data * 1.4 / 1000;
	
	return light;
}