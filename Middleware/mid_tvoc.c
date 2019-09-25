#include "mid_tvoc.h"

//static uint8_t tvoc_buf[13] = {0};  //��������
//
//static IIC_Hard_Master_WRInfo_TypeDef iic_info =
//{
//	.device_adr = TVOC_DEVICE_ADDRESS,  //��ַ0xA2
//	.dev_adr_tenbit_flag = 0,  //��10λ��ַ
//	.data = tvoc_buf,  //����ָ�븳ֵ
//	.len = 13,  //��ʼ���ݳ���13
//	.error_resend_times = 3,  //ʧ���ض�����3
//};  //IIC��ȡ��Ϣ

////tvoc����co2��ȡ��ʼ�������ɹ�����0��ʧ�ܷ���1
//uint8_t tvoc_eco2_read_start(void)
//{
//	if(IIC_Hard_Master_WriteRead_Start(&iic_info, ReadDirect_Sel))
//		return 1;
//	
//	return 0;
//}
//
////tvoc���ݴ�����
//void tvoc_data_deal(uint8_t * buf, uint16_t len)
//{
//	uint8_t check_sum = 0;  //��У��λ
//	
//	check_sum = (uint8_t)(((~tvoc_check_sum(&buf[1], 11)) + 1) & 0xFF);  //����У��
//	
//	if((buf[0] == 0xFF) && (buf[3] != 0x02) && (check_sum == buf[12]))  //�ж�����
//	{
//		Evndat.TVOC = (((uint16_t)buf[8] << 8) + buf[9]) / 1000.0;
//		Evndat.tvocco2 = ((uint16_t) buf[1] << 8) + buf[2];
//		
//		if(Evndat.TVOC <= 0)
//			Evndat.TVOC = 0.01;
//		if(Evndat.tvocco2 < 400)
//			Evndat.tvocco2 = 400;
//		else if(Evndat.tvocco2 > 4500)
//			Evndat.tvocco2 = 4500;
//	}
//}

//tvoc����co2��ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t tvoc_eco2_read(float * tvoc, float * tvocco2)
{
    uint8_t buf[13] = {0};  //���ݻ���
    uint8_t check_sum = 0;  //��У��λ
    IIC_Master_WRInfo_TypeDef iic_info =
    {
        .port = TVOC_PORT,
        .device_adr = TVOC_DEVICE_ADDRESS,
        .dev_adr_tenbit_flag = 0,
        .data = buf,
        .len = 13,
        .error_resend_times = 3,
    };  //IIC��ȡ��Ϣ
    
    if(IIC_Simulation_Master_ReadDirect(&iic_info))
        return 1;
    
    check_sum = (uint8_t)(((~check_sum_get(&buf[1], 11)) + 1) & 0xFF);  //����У��
    if((buf[0] == 0xFF) && (buf[3] != 0x02) && (check_sum == buf[12]))  //�ж�����
    {
        *tvoc = (((uint16_t)buf[8] << 8) + buf[9]) / 1000.0;
        *tvocco2 = ((uint16_t) buf[1] << 8) + buf[2];
        
        if(*tvoc <= 0)
            *tvoc = 0.01;
        if(*tvocco2 < 400)
            *tvocco2 = 400;
        else if(*tvocco2 > 4500)
            *tvocco2 = 4500;
    }
    else
    {
        *tvoc = 0.01;
        *tvocco2 = 399;
    }
    
    return 0;
}
