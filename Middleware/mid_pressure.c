#include "mid_pressure.h"

typedef enum
{
	Pressure_Select = 0,
	Temperature_Select
}Pres_Temp_Sele_TypeDef;

IIC_Master_WRInfo_TypeDef iic_info =
{
	.port = 2,  //�˿�2
	.device_adr = PRES_DEVICE_ADDRESS,  //��ַ0xEE
	.dev_adr_tenbit_flag = 0,  //��10λ��ַ
//	.data = NULL,  //��ʼ����ָ���
	.len = 1,  //��ʼ���ݳ���1
	.error_resend_times = 3,  //ʧ���ض�����3
};  //IIC��ȡ��Ϣ

uint16_t c1, c2, c3, c4, c5, c6;  //У׼����

static uint8_t pres_dev_reset(void);  //�豸��λ���ɹ�����0��ʧ�ܷ���1
static uint8_t calibration_data_read(void);  //��ȡУ׼���ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t digital_data_read(uint32_t * data, Pres_Temp_Sele_TypeDef sele);  //��ȡ��ѹ���¶����ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t pres_temp_convert(float * pres_data, float * temp_data);  //��ѹ�����¶����ݶ�ȡ�����㺯�����ɹ�����0��ʧ�ܷ���1

//��ѹ�����¶ȶ�ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t pres_temp_read(float * pres_data, float * temp_data)
{
	static uint8_t reset_flag = 1;  //��λ��־
	static uint8_t cali_get_flag = 1;  //��ȡУ׼���ݱ�־
	
	if(reset_flag)
	{
		if(pres_dev_reset())  //��λ�����ж��Ƿ�ɹ�
			return 1;
		reset_flag = 0;
		cali_get_flag = 1;
	}
	
	if(cali_get_flag)
	{
		if(calibration_data_read())  //��ȡУ׼���ݣ����ж��Ƿ�ɹ�
		{
			reset_flag = 1;
			cali_get_flag = 1;
			return 1;
		}
		cali_get_flag = 0;
	}
	
	if(pres_temp_convert(pres_data, temp_data))  //��ѹ�����¶����ݶ�ȡ�����㣬���ж��Ƿ�ɹ�
	{
		reset_flag = 1;
		cali_get_flag = 1;
		return 1;
	}
	
	return 0;
}

//�豸��λ���ɹ�����0��ʧ�ܷ���1
static uint8_t pres_dev_reset(void)
{
	uint8_t result = 1;
	uint8_t command = 0x1E;
	
	iic_info.data = &command;
	iic_info.len = 1;
	
	result = IIC_Master_Write(&iic_info);
	
    return result;
}

//��ȡУ׼���ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t calibration_data_read(void)
{
	uint8_t buf[2] = {0};
    uint8_t command = 0xA2;
	
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))  //��������
		return 1;  //���ִ��󷵻�1
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))  //��ȡ����
		return 1;
	c1 = ((uint16_t)buf[0] << 8) | buf[1];
	
	command = 0xA4;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	c2 = ((uint16_t)buf[0] << 8) | buf[1];
	
	command = 0xA6;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	c3 = ((uint16_t)buf[0] << 8) | buf[1];
	
	command = 0xA8;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	c4 = ((uint16_t)buf[0] << 8) | buf[1];
	
	command = 0xAA;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	c5 = ((uint16_t)buf[0] << 8) | buf[1];
	
	command = 0xAC;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	iic_info.data = buf;
	iic_info.len = 2;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	c6 = ((uint16_t)buf[0] << 8) | buf[1];
	
	return 0;  //û�г��ֹ����󣬷���0
}

//��ȡ��ѹ���¶����ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t digital_data_read(uint32_t * data, Pres_Temp_Sele_TypeDef sele)
{
    uint8_t buf[3] = {0};
    uint8_t command = 0x48;  //��ѹת����������
	
	if(sele == Temperature_Select)
		command = 0x58;  //�¶�ת����������
	
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;  //���ִ��󷵻�1
	delay_ms(DELAY_CONVERSION);  //��ʱ�ȴ�ADC����ת�����
	
	command = 0x00;
	iic_info.data = &command;
	iic_info.len = 1;
	if(IIC_Master_Write(&iic_info))
		return 1;
	
	iic_info.data = buf;
	iic_info.len = 3;
	if(IIC_Master_ReadDirect(&iic_info))
		return 1;
	*data = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
	
	return 0;  //û�г��ֹ����󣬷���0
}

//��ѹ�����¶����ݶ�ȡ�����㺯�����ɹ�����0��ʧ�ܷ���1
static uint8_t pres_temp_convert(float * pres_data, float * temp_data)
{
	uint32_t d1 = 0, d2 = 0;  //��ѹ���¶�����������
	int32_t dt = 0, temp = 0;
	int64_t off = 0, sens = 0, p = 0;
	int32_t t2 = 0;
	int64_t off2 = 0, sens2 = 0;
	
	if(digital_data_read(&d1, Pressure_Select))
		return 1;
	
	if(digital_data_read(&d2, Temperature_Select))
		return 1;
	
	dt = d2 - ((uint32_t)c5 << 8);  //d2 - c5 * 2^8�����ȼ�+����<<
	temp = 2000 + (dt * c6 >> 23);  //2000 + dt * c6 / 2^23
	if(temp < 2000)
	{
		t2 = dt * dt >> 31;
		off2 = 5 * (temp - 2000) * (temp - 2000) >> 1;
		sens2 = 5 * (temp - 2000) * (temp - 2000) >> 2;
		
		if(temp < -1500)
		{
			off2 = off2 + 7 * (temp + 1500) * (temp + 1500);
			sens2 = sens2 + (11 * (temp + 1500) * (temp + 1500) >> 1);
		}
	}
	temp = temp - t2;
	
	off = ((uint64_t)c2 << 16) + (c4 * dt >> 7);
	sens = ((uint64_t)c1 << 15) + (c3 * dt >> 8);
	off = off - off2;
	sens = sens - sens2;
	p = ((d1 * sens >> 21) - off) >> 15;
	
	*pres_data = p / 1000.0;  //mbar->KPa��1mbar = 100Pa = 0.1KPa
	*temp_data = temp / 100.0;
	
	return 0;
}
