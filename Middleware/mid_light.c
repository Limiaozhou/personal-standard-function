#include "mid_light.h"

//光照度传感器初始化
void Light_Init(void)
{
    IIC_Master_Init(1);
}

//光照度读取函数，返回光照度数据
float light_read(void)
{
	uint8_t buf[4] = {0};  //读取数据缓存数组
	uint32_t data = 0;  //读取数据缓存
	float light = 0;  //数据处理结果返回值
	IIC_Master_WRInfo_TypeDef iic_info =
	{
		.port = 1,
		.device_adr = 0x94,
        .dev_adr_tenbit_flag = 0,
		.register_adr = 0x00,
		.reg_adr_twobyte_flag = 0,
		.read_delay_flag = 1,
		.read_delay_nms = 15,
		.data = buf,
		.len = 4,
        .error_resend_times = 3
	};  //IIC读取信息
	
	if(!IIC_Master_ReadRegister(&iic_info))  //IIC读取数据并保存结果到缓存数组，并判断是否成功
        data = buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);  //缓存数据
	light = (float)data * 1.4 / 1000;  //计算结果
	
	return light;
}
