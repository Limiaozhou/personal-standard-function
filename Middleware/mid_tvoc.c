#include "mid_tvoc.h"

static uint8_t tvoc_buf[13] = {0};  //缓存数组

static IIC_Hard_Master_WRInfo_TypeDef iic_info =
{
	.device_adr = TVOC_DEVICE_ADDRESS,  //地址0xA2
	.dev_adr_tenbit_flag = 0,  //非10位地址
	.data = tvoc_buf,  //数据指针赋值
	.len = 13,  //初始数据长度13
	.error_resend_times = 3,  //失败重读次数3
};  //IIC读取信息

static uint16_t tvoc_check_sum(uint8_t * buf, uint16_t len);  //和校验计算

//tvoc及其co2读取开始函数，成功返回0，失败返回1
uint8_t tvoc_eco2_read_start(void)
{
	if(IIC_Hard_Master_WriteRead_Start(&iic_info, ReadDirect_Sel))
		return 1;
	
	return 0;
}

//tvoc数据处理函数
void tvoc_data_deal(uint8_t * buf, uint16_t len)
{
	uint8_t check_sum = 0;  //和校验位
	
	check_sum = (uint8_t)(((~tvoc_check_sum(&buf[1], 11)) + 1) & 0xFF);  //计算校验
	
	if((buf[0] == 0xFF) && (buf[3] != 0x02) && (check_sum == buf[12]))  //判断数据
	{
		Evndat.TVOC = (((uint16_t)buf[8] << 8) + buf[9]) / 1000.0;
		Evndat.tvocco2 = ((uint16_t) buf[1] << 8) + buf[2];
		
		if(Evndat.TVOC <= 0)
			Evndat.TVOC = 0.01;
		if(Evndat.tvocco2 < 400)
			Evndat.tvocco2 = 400;
		else if(Evndat.tvocco2 > 4500)
			Evndat.tvocco2 = 4500;
	}
}

//和校验计算
static uint16_t tvoc_check_sum(uint8_t * buf, uint16_t len)
{
	uint16_t check_sum = 0;
	while(len--)
		check_sum += *(buf++);
	
	return check_sum;
}
