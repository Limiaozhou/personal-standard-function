#include "mid_temperature.h"

IIC_Hard_Master_WRInfo_TypeDef iic_info =
{
	.device_adr = TVOC_DEVICE_ADDRESS,  //地址0xEE
	.dev_adr_tenbit_flag = 0,  //非10位地址
	.len = 13,  //初始数据长度1
	.error_resend_times = 3,  //失败重读次数3
};  //IIC读取信息

static uint8_t temp_dev_reset(void);  //设备复位，成功返回0，失败返回1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg);  //读取气压或温度数据，成功返回0，失败返回1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data);  //温湿度数据读取并计算函数，成功返回0，失败返回1

//tvoc及其co2读取函数，成功返回0，失败返回1
uint8_t tvoc_eco2_read(float * tvoc_data, float * eco2_data)
{
	if(temp_humi_convert(temp_data, humi_data))  //气压及其温度数据读取并计算，并判断是否成功
	{
		return 1;
	}
	
	return 0;
}

//读取tvoc数据，成功返回0，失败返回1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg)
{
    uint8_t buf[3] = {0};  //缓存数组，前两字节数据，后一字节校验
	
	iic_info.register_adr = reg,
	iic_info.wr_info.data = buf;
	iic_info.wr_info.len = 3;
	
	if((reg == Temp_Host) || (reg == Humi_Host))  //保持模式时，发读后延时
		iic_info.readreg_delay_flag = Delay_Back;
	else
		iic_info.readreg_delay_flag = Delay_Front;
	
	if(IIC_Master_ReadRegister(&iic_info))  //IIC读取数据并保存结果到缓存数组，并判断是否成功
		return 1;
	
	*data = ((uint16_t)buf[0] << 8) | buf[1];
	
	return 0;  //没有出现过错误，返回0
}

//温湿度数据读取并计算函数，成功返回0，失败返回1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data)
{
	uint16_t srh = 0, st = 0;
	
	if(digital_data_read(&st, Temp_NoHost))
		return 1;
	
	if(digital_data_read(&srh, Humi_NoHost))
		return 1;
	
	st &= ~0x03;  //数据为14bit，最低两位为状态位
	srh &= ~0x03;
	
	*temp_data = -46.85 + (175.72 * (float)st / 65536);
	*humi_data = -6 + (125 * (float)srh / 65536);
	
	return 0;
}
