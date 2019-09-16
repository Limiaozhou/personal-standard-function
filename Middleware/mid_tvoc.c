#include "mid_temperature.h"

// IIC_Master_WRInfo_TypeDef iic_info =
// {
	// .port = 3,  //端口2
	// .device_adr = TEMP_DEVICE_ADDRESS,  //地址0xEE
	// .dev_adr_tenbit_flag = 0,  //非10位地址
// //	.data = NULL,  //初始数据指针空
	// .len = 1,  //初始数据长度1
	// .error_resend_times = 3,  //失败重读次数3
// };  //IIC读取信息

static IIC_Master_ReadReg_Info_TypeDef iic_info =
{
	.wr_info.port = 3,
	.wr_info.device_adr = TEMP_DEVICE_ADDRESS,
	.wr_info.dev_adr_tenbit_flag = 0,
	// .wr_info.data = buf,
	.wr_info.len = 1,
	.wr_info.error_resend_times = 3,
	.register_adr = 0x00,
	.reg_adr_twobyte_flag = 0,
	.readreg_delay_flag = Delay_Front,
	.readreg_delay_nms = 90
};

static uint8_t temp_dev_reset(void);  //设备复位，成功返回0，失败返回1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg);  //读取气压或温度数据，成功返回0，失败返回1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data);  //温湿度数据读取并计算函数，成功返回0，失败返回1

//气压及其温度读取函数，成功返回0，失败返回1
uint8_t temp_humi_read(float * temp_data, float * humi_data)
{
	static uint8_t reset_flag = 1;  //复位标志
	
	if(reset_flag)
	{
		if(temp_dev_reset())  //复位，并判断是否成功
			return 1;
		reset_flag = 0;
		delay_ms(20);
	}
	
	if(temp_humi_convert(temp_data, humi_data))  //气压及其温度数据读取并计算，并判断是否成功
	{
		reset_flag = 1;
		return 1;
	}
	
	return 0;
}

//设备复位，成功返回0，失败返回1
static uint8_t temp_dev_reset(void)
{
	uint8_t result = 1;
	uint8_t command = TEMP_DEVICE_RESET;
	
	iic_info.wr_info.data = &command;
	iic_info.wr_info.len = 1;
	
	result = IIC_Master_Write(&(iic_info.wr_info));
	
    return result;
}

//读取气压或温度数据，成功返回0，失败返回1
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
