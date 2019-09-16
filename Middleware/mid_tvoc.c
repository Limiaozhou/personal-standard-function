#include "mid_temperature.h"

// IIC_Master_WRInfo_TypeDef iic_info =
// {
	// .port = 3,  //�˿�2
	// .device_adr = TEMP_DEVICE_ADDRESS,  //��ַ0xEE
	// .dev_adr_tenbit_flag = 0,  //��10λ��ַ
// //	.data = NULL,  //��ʼ����ָ���
	// .len = 1,  //��ʼ���ݳ���1
	// .error_resend_times = 3,  //ʧ���ض�����3
// };  //IIC��ȡ��Ϣ

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

static uint8_t temp_dev_reset(void);  //�豸��λ���ɹ�����0��ʧ�ܷ���1
static uint8_t digital_data_read(uint16_t * data, Temp_Reg_TypeDef reg);  //��ȡ��ѹ���¶����ݣ��ɹ�����0��ʧ�ܷ���1
static uint8_t temp_humi_convert(float * temp_data, float * humi_data);  //��ʪ�����ݶ�ȡ�����㺯�����ɹ�����0��ʧ�ܷ���1

//��ѹ�����¶ȶ�ȡ�������ɹ�����0��ʧ�ܷ���1
uint8_t temp_humi_read(float * temp_data, float * humi_data)
{
	static uint8_t reset_flag = 1;  //��λ��־
	
	if(reset_flag)
	{
		if(temp_dev_reset())  //��λ�����ж��Ƿ�ɹ�
			return 1;
		reset_flag = 0;
		delay_ms(20);
	}
	
	if(temp_humi_convert(temp_data, humi_data))  //��ѹ�����¶����ݶ�ȡ�����㣬���ж��Ƿ�ɹ�
	{
		reset_flag = 1;
		return 1;
	}
	
	return 0;
}

//�豸��λ���ɹ�����0��ʧ�ܷ���1
static uint8_t temp_dev_reset(void)
{
	uint8_t result = 1;
	uint8_t command = TEMP_DEVICE_RESET;
	
	iic_info.wr_info.data = &command;
	iic_info.wr_info.len = 1;
	
	result = IIC_Master_Write(&(iic_info.wr_info));
	
    return result;
}

//��ȡ��ѹ���¶����ݣ��ɹ�����0��ʧ�ܷ���1
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
