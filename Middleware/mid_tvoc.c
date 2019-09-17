#include "mid_temperature.h"

IIC_Hard_Master_WRInfo_TypeDef iic_info =
{
	.device_adr = TVOC_DEVICE_ADDRESS,  //��ַ0xEE
	.dev_adr_tenbit_flag = 0,  //��10λ��ַ
	.len = 13,  //��ʼ���ݳ���1
	.error_resend_times = 3,  //ʧ���ض�����3
};  //IIC��ȡ��Ϣ

static uint8_t temp_dev_reset(void);  //�豸��λ���ɹ�����0��ʧ�ܷ���1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg);  //��ȡ��ѹ���¶����ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data);  //��ʪ�����ݶ�ȡ�����㺯�����ɹ�����0��ʧ�ܷ���1

//tvoc����co2��ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t tvoc_eco2_read(float * tvoc_data, float * eco2_data)
{
	if(temp_humi_convert(temp_data, humi_data))  //��ѹ�����¶����ݶ�ȡ�����㣬���ж��Ƿ�ɹ�
	{
		return 1;
	}
	
	return 0;
}

//��ȡtvoc���ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg)
{
    uint8_t buf[3] = {0};  //�������飬ǰ���ֽ����ݣ���һ�ֽ�У��
	
	iic_info.register_adr = reg,
	iic_info.wr_info.data = buf;
	iic_info.wr_info.len = 3;
	
	if((reg == Temp_Host) || (reg == Humi_Host))  //����ģʽʱ����������ʱ
		iic_info.readreg_delay_flag = Delay_Back;
	else
		iic_info.readreg_delay_flag = Delay_Front;
	
	if(IIC_Master_ReadRegister(&iic_info))  //IIC��ȡ���ݲ����������������飬���ж��Ƿ�ɹ�
		return 1;
	
	*data = ((uint16_t)buf[0] << 8) | buf[1];
	
	return 0;  //û�г��ֹ����󣬷���0
}

//��ʪ�����ݶ�ȡ�����㺯�����ɹ�����0��ʧ�ܷ���1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data)
{
	uint16_t srh = 0, st = 0;
	
	if(digital_data_read(&st, Temp_NoHost))
		return 1;
	
	if(digital_data_read(&srh, Humi_NoHost))
		return 1;
	
	st &= ~0x03;  //����Ϊ14bit�������λΪ״̬λ
	srh &= ~0x03;
	
	*temp_data = -46.85 + (175.72 * (float)st / 65536);
	*humi_data = -6 + (125 * (float)srh / 65536);
	
	return 0;
}
