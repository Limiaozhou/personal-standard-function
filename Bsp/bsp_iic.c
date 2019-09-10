#include "bsp_iic.h"

IIC_Hard_Master_WRInfo_TypeDef iic_wrinfo;
IIC_Hard_Master_ReadReg_Info_TypeDef iic_reginfo;

//�ڲ�����
//__weak static void delay_us(uint16_t nus);  //��ʱn us���ڲ�ʹ��
//__weak static void delay_ms(uint16_t nms);  //��ʱn ms

//��ʱn us���ڲ�ʹ��
//__weak static void delay_us(uint16_t nus)
//{
//	while (nus--)
//	{
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//		asm("NOP");
//	}
//}
//
////��ʱn ms
//__weak static void delay_ms(uint16_t nms)
//{
//	while (nms--)
//		delay_us(1000);
//}

//��ʼ�����ʱ�ӡ��������ӻ��ĵ�ַ����ַģʽ
void IIC_Hard_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, I2C_AddMode_TypeDef AddMode)
{
	I2C_DeInit();
	//�������ʱ�ӡ��������ӻ��ĵ�ַ������ģʽռ�ձȡ�Ӧ��ģʽ����ַģʽ������ʱ��
	I2C_Init(OutputClockFrequencyHz, OwnAddress, I2C_DUTYCYCLE_2, I2C_ACK_CURR, AddMode, CLK_GetClockFreq()/1000000);
	I2C_Cmd(ENABLE);
	I2C_ITConfig(I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);  //ʹ���¼����շ��������ж�
}

//����IIC��д
uint8_t IIC_Hard_Master_Write_Read(pIIC_Hard_Master_WRInfo_TypeDef piic, piic_hard_master_write_read pfun)
{
	iic_wrinfo = *piic;
	
	while(iic_wrinfo.error_resend_times--)
	{
		if(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))  //�ж�����æ��
			continue;
		
		I2C_GenerateSTART(ENABLE);  //��ʼ
		
		return 0;
	}
	
	return 1;
}

//д�豸���ṹ�������豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Hard_Master_Write(pIIC_Hard_Master_WRInfo_TypeDef piic)
{
	static uint8_t i = 0, j = 0;  //����������������
	static int8_t write_read_flag = -1;  //���ͽ���״̬����
	if(I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION))  //�жϿ�ʼ
	{
		if(iic_wrinfo.dev_adr_tenbit_flag)  //10λ��ַģʽ���ȷ��͸�λ�ֽ�
			I2C_SendData((uint8_t)(iic_wrinfo.device_adr >> 8));
		else
			I2C_Send7bitAddress((uint8_t)(iic_wrinfo.device_adr & 0xFE), I2C_DIRECTION_TX);  //���͵�ַ
	}
	if(I2C_GetFlagStatus(I2C_FLAG_HEADERSENT))  //�ж�10λ��ַ���ֽ��ѷ���
		I2C_Send7bitAddress((uint8_t)(iic_wrinfo.device_adr & 0xFE), I2C_DIRECTION_TX);  //���͵�ַ
	if(I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED))  //�жϵ�ַ�������
		write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER)  //�жϷ��ͽ���״̬��1Ϊ���գ�0Ϊ����
	if(write_read_flag == 0)  //�ж�Ϊ����
	{
		if(I2C_GetFlagStatus(I2C_FLAG_TXEMPTY))  //�жϷ������ݼĴ���Ϊ�գ���������ʹ���
		{
			i++;
			if(i >= iic_wrinfo.len)  //�жϷ������
			{
				if(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))  //ûд��������
				{
					i = 0;  //���·��ͼ���
					write_read_flag = -1;  //�巢�ͽ��ձ�־
					I2C_GenerateSTOP(ENABLE);  //ֹͣ
				}
			}
			I2C_SendData(*(iic_wrinfo.data++));  //�������ݣ�д��Ĵ�������������
		}
	}
	else if(write_read_flag == 1)  //�ж�Ϊ����
	{
		// I2C_GenerateSTART(ENABLE);  //��ʼ
		if(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY))  //�жϽ������ݼĴ����ǿգ������괥��
		{
			*(iic_wrinfo.data++) = I2C_ReceiveData();  //��������
			j++;
			if(j >= iic_wrinfo.len)
			{
				j = 0;  //���½��ռ���
				write_read_flag = -1;  //�巢�ͽ��ձ�־
				I2C_AcknowledgeConfig(I2C_ACK_CURR);  //��ǰ�ֽڷ���Ӧ��Ϊ�´δ���׼��
			}
			else if(j >= iic_wrinfo.len - 1)  //�����ڶ����ֽ�
			{
				I2C_AcknowledgeConfig(I2C_ACK_NONE);  //������Ӧ��
				I2C_GenerateSTOP(ENABLE);  //ֹͣ
			}
		}
	}
	if(I2C_GetFlagStatus(I2C_FLAG_BUSERROR))  //�ж����ߴ��󣬽��մ�����ʼ��ֹͣ
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_BUSERROR);  //��0
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ACKNOWLEDGEFAILURE))  //�ж�Ӧ��ʧ�ܣ����շ�Ӧ��
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_ACKNOWLEDGEFAILURE);  //��0
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ARBITRATIONLOSS))  //�ж��ٲ�ʧ��
	{
		I2C_ClearFlag(I2C_FLAG_ARBITRATIONLOSS);  //��0
	}
}

//���豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //���������ֽ�����¼
    
	while(piic->error_resend_times--)
	{
		IIC_Master_Start(piic->port);
		
        if(piic->dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //�豸�͵�ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ����ע����������ȼ�+����&
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		for(i = 0; i < piic->len; i++)  //����ָ����������
		{
			if(i < piic->len - 1)
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 0);  //����û�����꣬������ЧӦ��
			else
				*(piic->data + i) = IIC_Master_ReceiveByte(piic->port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		
		IIC_Master_Stop(piic->port);
		
		return 0;  //��û�г���ֹͣ���������������0
	}
	
	return 1;  //����3�ζ����󣬷���1
}

//���豸�Ĵ������ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־���Ĵ�����ַ���䳤�ȡ��Ƿ���ʱ����ʱʱ�䡢���ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic)
{
	uint8_t i;  //���������ֽ�����¼
    
	while(piic->wr_info.error_resend_times--)
	{
		//��д�Ĵ�����ַ
		IIC_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))  //�豸�͵�ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))  //�豸��ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		if(piic->reg_adr_twobyte_flag)  //�ӻ�2�ֽڼĴ�����ַģʽ�ж�
		{
			if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr >> 8)))  //�Ĵ����ߵ�ַ
			{
				IIC_Master_Stop(piic->wr_info.port);
				continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
			}
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //�Ĵ����͵�ַ
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
		}
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //�Ĵ�����ַ
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		
		if(piic->readreg_delay_flag == Delay_Front)
			delay_ms(piic->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		//�ٶ�����
		IIC_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))  //�豸�͵�ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->wr_info.port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		if(piic->readreg_delay_flag == Delay_Back)
			delay_ms(piic->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		for(i = 0; i < piic->wr_info.len; i++)  //����ָ����������
		{
			if(i < piic->wr_info.len - 1)
				*(piic->wr_info.data + i) = IIC_Master_ReceiveByte(piic->wr_info.port, 0);  //����û�����꣬������ЧӦ��
			else
				*(piic->wr_info.data + i) = IIC_Master_ReceiveByte(piic->wr_info.port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		IIC_Master_Stop(piic->wr_info.port);
		
		return 0;  //��û�г���ֹͣ���������������0
	}
    
    return 1;  //����3�ζ����󣬷���1
}

//I2C�жϴ���
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
}
