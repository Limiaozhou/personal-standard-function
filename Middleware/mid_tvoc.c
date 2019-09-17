#include "mid_tvoc.h"

static uint8_t tvoc_buf[13] = {0};  //��������

static IIC_Hard_Master_WRInfo_TypeDef iic_info =
{
	.device_adr = TVOC_DEVICE_ADDRESS,  //��ַ0xA2
	.dev_adr_tenbit_flag = 0,  //��10λ��ַ
	.data = tvoc_buf,  //����ָ�븳ֵ
	.len = 13,  //��ʼ���ݳ���13
	.error_resend_times = 3,  //ʧ���ض�����3
};  //IIC��ȡ��Ϣ

static uint16_t tvoc_check_sum(uint8_t * buf, uint16_t len);  //��У�����

//tvoc����co2��ȡ��ʼ�������ɹ�����0��ʧ�ܷ���1
uint8_t tvoc_eco2_read_start(void)
{
	if(IIC_Hard_Master_WriteRead_Start(&iic_info, ReadDirect_Sel))
		return 1;
	
	return 0;
}

//tvoc���ݴ�����
void tvoc_data_deal(uint8_t * buf, uint16_t len)
{
	uint8_t check_sum = 0;  //��У��λ
	
	check_sum = (uint8_t)(((~tvoc_check_sum(&buf[1], 11)) + 1) & 0xFF);  //����У��
	
	if((buf[0] == 0xFF) && (buf[3] != 0x02) && (check_sum == buf[12]))  //�ж�����
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

//��У�����
static uint16_t tvoc_check_sum(uint8_t * buf, uint16_t len)
{
	uint16_t check_sum = 0;
	while(len--)
		check_sum += *(buf++);
	
	return check_sum;
}
