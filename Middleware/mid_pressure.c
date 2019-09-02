#include "mid_pressure.h"

IIC_Master_WRInfo_TypeDef iic_info =
{
	.port = 2,  //端口2
	.device_adr = PRES_DEVICE_ADDRESS,  //地址0xEE
	.dev_adr_tenbit_flag = 0,  //非10位地址
	.register_adr = 0x00,  //初始寄存器地址
	.reg_adr_twobyte_flag = 0,  //非2字节寄存器地址
	.readreg_delay_flag = 0,  //读寄存器发读后不延时
	.readreg_delay_nms = 0,  //延时0 ms
	.data = NULL,  //初始数据指针空
	.len = 1,  //初始数据长度1
	.error_resend_times = 3,  //失败重读次数3
};  //IIC读取信息

uint16_t c1, c2, c3, c4, c5, c6;  //校准数据

//设备复位，成功返回0，失败返回1
uint8_t pres_dev_reset(void)
{
	uint8_t result = 1;
	uint8_t command = 0x1E;
	
	iic_info.data = &command;
	iic_info.len = 1;
	
	result = IIC_Master_Write(&iic_info);
	
    return result;
}

//气压读取函数，成功返回0，失败返回1
uint8_t pressure_read(float * data)
{
	// uint8_t buf[4] = {0};  //读取数据缓存数组
	// uint32_t data = 0;  //读取数据缓存
	// float light = 0;  //数据处理结果返回值
	// IIC_Master_WRInfo_TypeDef iic_info =
	// {
		// .port = 1,
		// .device_adr = PRES_DEVICE_ADDRESS,
        // .dev_adr_tenbit_flag = 0,
		// .register_adr = 0x00,
		// .reg_adr_twobyte_flag = 0,
		// .readreg_delay_flag = 0,
		// .readreg_delay_nms = 0,
		// .data = buf,
		// .len = 4,
        // .error_resend_times = 3
	// };  //IIC读取信息
	
	// if(!IIC_Master_ReadRegister(&iic_info))  //IIC读取数据并保存结果到缓存数组，并判断是否成功
        // data = buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);  //缓存数据
	// light = (float)data * 1.4 / 1000;  //计算结果
	
	// return light;
}

//读取校准数据，成功返回0，失败返回1
uint8_t calibration_data_read(void)
{
	uint8_t buf[2] = {0};
    uint8_t command = 0xA2;
	
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c1 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;  //出现错误返回1
	}
	else
		return 1;
	
	command = 0xA4;
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c2 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;
	}
	else
		return 1;
	
	command = 0xA6;
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c3 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;
	}
	else
		return 1;
	
	command = 0xA8;
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c4 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;
	}
	else
		return 1;
	
	command = 0xAA;
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c5 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;
	}
	else
		return 1;
	
	command = 0xAC;
	iic_info.data = &command;
	iic_info.len = 1;
	if(!IIC_Master_Write(&iic_info))
	{
		iic_info.data = &buf;
		iic_info.len = 2;
		if(IIC_Master_ReadDirect(&iic_info))
			c6 = ((uint16_t)buf[0] << 8) | buf[1];
		else
			return 1;
	}
	else
		return 1;
	
	return 0;
}

//读取气压数据
void digital_pressure_read(void)
{
    
}

//读取温度数据
void digital_temperature_read(void)
{
    
}
