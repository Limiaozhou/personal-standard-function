#include "mid_light.h"

float light_read(void)
{
	uint8_t reg_adr = 0x00;  //寄存器地址
	uint8_t buf[4] = {0};  //读取缓存数据
	uint32_t data = 0;
	float light = 0;
	IIC_Master_WRInfo_TypeDef iic_info =
	{
		.port = 1,
		.device_adr = 0x94,
		.register_adr = &reg_adr,
		.reg_adr_len = 1,
		.read_delay_flag = 1,
		.read_delay_nms = 15,
		.data = buf,
		.len = 4
	};
	
	IIC_Master_ReadRegister(&iic_info);
	data = buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
	light = (float)data * 1.4 / 1000;
	
	return light;
}