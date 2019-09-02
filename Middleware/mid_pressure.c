#include "mid_pressure.h"

IIC_Master_WRInfo_TypeDef iic_info =
{
	.port = 2,  //�˿�2
	.device_adr = PRES_DEVICE_ADDRESS,  //��ַ0xEE
	.dev_adr_tenbit_flag = 0,  //��10λ��ַ
	.register_adr = 0x00,  //��ʼ�Ĵ�����ַ
	.reg_adr_twobyte_flag = 0,  //��2�ֽڼĴ�����ַ
	.readreg_delay_flag = 0,  //���Ĵ�����������ʱ
	.readreg_delay_nms = 0,  //��ʱ0 ms
	.data = NULL,  //��ʼ����ָ���
	.len = 1,  //��ʼ���ݳ���1
	.error_resend_times = 3,  //ʧ���ض�����3
};  //IIC��ȡ��Ϣ

uint16_t c1, c2, c3, c4, c5, c6;  //У׼����

//�豸��λ���ɹ�����0��ʧ�ܷ���1
uint8_t pres_dev_reset(void)
{
	uint8_t result = 1;
	uint8_t command = 0x1E;
	
	iic_info.data = &command;
	iic_info.len = 1;
	
	result = IIC_Master_Write(&iic_info);
	
    return result;
}

//��ѹ��ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t pressure_read(float * data)
{
	// uint8_t buf[4] = {0};  //��ȡ���ݻ�������
	// uint32_t data = 0;  //��ȡ���ݻ���
	// float light = 0;  //���ݴ���������ֵ
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
	// };  //IIC��ȡ��Ϣ
	
	// if(!IIC_Master_ReadRegister(&iic_info))  //IIC��ȡ���ݲ����������������飬���ж��Ƿ�ɹ�
        // data = buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);  //��������
	// light = (float)data * 1.4 / 1000;  //������
	
	// return light;
}

//��ȡУ׼���ݣ��ɹ�����0��ʧ�ܷ���1
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
			return 1;  //���ִ��󷵻�1
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

//��ȡ��ѹ����
void digital_pressure_read(void)
{
    
}

//��ȡ�¶�����
void digital_temperature_read(void)
{
    
}
