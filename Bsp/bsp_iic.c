#include "bsp_iic.h"

static pIIC_Hard_Master_WRInfo_TypeDef piic_wrinfo;
//static pIIC_Hard_Master_ReadReg_Info_TypeDef piic_reginfo;

static IIC_WriteRead_Select_TypeDef iic_wr_sel = Write_Sel;

static piic_hard_read_data_deal pread_data_deal;

static void IIC_Hard_Master_WriteRead(pIIC_Hard_Master_WRInfo_TypeDef piic);  //Ӳ��IIC��д����������ָ��
static uint8_t IIC_Hard_Master_Error_Deal(void);  //�����жϴ����д��󷵻�1�����򷵻�0

//Ӳ��IIC��ʼ�����������ʱ�ӡ��������ӻ��ĵ�ַ����ַģʽ
void IIC_Hard_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, I2C_AddMode_TypeDef AddMode, piic_hard_read_data_deal pdeal)
{
	//��ʼ����ӦIO
	GPIO_DeInit(GPIOE);
	GPIO_Init(GPIOE, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW);
	GPIO_Init(GPIOE, GPIO_PIN_2, GPIO_MODE_OUT_OD_HIZ_SLOW);
	GPIO_WriteHigh(GPIOE, GPIO_PIN_1);
	GPIO_WriteHigh(GPIOE, GPIO_PIN_2);
	
	I2C_DeInit();
	//�������ʱ�ӡ��������ӻ��ĵ�ַ������ģʽռ�ձȡ�Ӧ��ģʽ����ַģʽ������ʱ��
	I2C_Init(OutputClockFrequencyHz, OwnAddress, I2C_DUTYCYCLE_2, I2C_ACK_CURR, AddMode, CLK_GetClockFreq()/1000000);
	I2C_Cmd(ENABLE);
	I2C_ITConfig(I2C_IT_EVT, ENABLE);  //ʹ���¼����շ��������ж�
    I2C_ITConfig(I2C_IT_BUF, ENABLE);
    I2C_ITConfig(I2C_IT_ERR, ENABLE);
	pread_data_deal = pdeal;  //��ȡ���ݴ���ص�����
}

//Ӳ��IIC��д��������������ָ�롢��дѡ�񣬳ɹ�����0��ʧ�ܣ�����æ������1
uint8_t IIC_Hard_Master_WriteRead_Start(pIIC_Hard_Master_WRInfo_TypeDef piic, IIC_WriteRead_Select_TypeDef sel)
{
	uint8_t resent_times = 1;  //ʧ���ط�����
	
	piic_wrinfo = piic;  //ָ�봫��
	iic_wr_sel = sel;
	resent_times = piic_wrinfo->error_resend_times;
	
	while(resent_times--)
	{
		if(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))  //�ж�����æ
		{
			delay_us(1);
			continue;
		}
		
		I2C_GenerateSTART(ENABLE);  //��ʼ
		
		return 0;
	}
	
	return 1;
}

//Ӳ��IIC��д����������ָ��
static void IIC_Hard_Master_WriteRead(pIIC_Hard_Master_WRInfo_TypeDef piic)
{
	static uint8_t write_index = 0, read_index = 0;  //����������������
	static int8_t write_read_flag = -1;  //���ͽ���״̬����
	static uint8_t addr10_read_flag = 0;  //10λ��ַ��ģʽ��־
	uint8_t successful_read_flag = 0;  //�ɹ���ȡ��־
	
	if(I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION))  //�жϿ�ʼ
	{
		if(piic->dev_adr_tenbit_flag)  //�ж�10λ��ַģʽ
		{
			if(iic_wr_sel == ReadDirect_Sel)  //�ж϶�ģʽ
			{
				if(!addr10_read_flag)
					I2C_SendData((uint8_t)(piic->device_adr >> 8));  //���͸�λͷ�ֽ�
				else
					I2C_Send7bitAddress((uint8_t)(piic->device_adr >> 8), I2C_DIRECTION_RX);  //ͷ�ֽڶ�
			}
			else
				I2C_SendData((uint8_t)(piic->device_adr >> 8));
		}
		else
		{
			if(iic_wr_sel == ReadDirect_Sel)
				I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_RX);  //���͵�ַ
			else
				I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_TX);
		}
	}
	if(I2C_GetFlagStatus(I2C_FLAG_HEADERSENT))  //�ж�10λ��ַ���ֽ��ѷ���
		I2C_Send7bitAddress((uint8_t)(piic->device_adr & 0xFE), I2C_DIRECTION_TX);
	if(I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED))  //�жϵ�ַ�������
	{
		if(piic->dev_adr_tenbit_flag)
		{
			if(iic_wr_sel == ReadDirect_Sel)
			{
				if(!addr10_read_flag)
				{
					I2C_GenerateSTART(ENABLE);
					addr10_read_flag = 1;
				}
				else
				{
					write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);
					addr10_read_flag = 0;
				}
			}
			else
				write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);
		}
		else
			write_read_flag = I2C_GetFlagStatus(I2C_FLAG_TRANSMITTERRECEIVER);  //���ͽ���״̬��0Ϊ���գ�1Ϊ����
	}
	if(write_read_flag == 1)  //�ж�Ϊ����
	{
		if(I2C_GetFlagStatus(I2C_FLAG_TXEMPTY))  //�жϷ������ݼĴ���Ϊ�գ���������ʹ���
		{
			write_index++;
			if(write_index >= piic->len)  //�жϷ������
			{
				if(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))  //ûд��������
				{
					write_index = 0;  //���·��ͼ���
					write_read_flag = -1;  //�巢�ͽ��ձ�־
					I2C_GenerateSTOP(ENABLE);  //ֹͣ
				}
			}
			I2C_SendData(*(piic->data++));  //�������ݣ�д��Ĵ�������������
		}
	}
	else if(write_read_flag == 0)  //�ж�Ϊ����
	{
		if(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY))  //�жϽ������ݼĴ����ǿգ������괥��
		{
			read_index++;
			*(piic->data++) = I2C_ReceiveData();  //��������
			if(read_index >= piic->len)
			{
				successful_read_flag = 1;  //��ȡ�ɹ�
				read_index = 0;  //���½��ռ���
				write_read_flag = -1;  //�巢�ͽ��ձ�־
				I2C_AcknowledgeConfig(I2C_ACK_CURR);  //��ǰ�ֽڷ���Ӧ��Ϊ�´δ���׼��
			}
			else if(read_index >= (piic->len - 1))  //�����ڶ����ֽ�
			{
				I2C_AcknowledgeConfig(I2C_ACK_NONE);  //������Ӧ��
				I2C_GenerateSTOP(ENABLE);  //ֹͣ
			}
		}
	}
	
	if(IIC_Hard_Master_Error_Deal())  //�ж��Ƿ����
	{
		write_index = 0;
		read_index = 0;
		write_read_flag = -1;
		addr10_read_flag = 0;
		successful_read_flag = 0;
	}
	
	if(successful_read_flag)
		pread_data_deal(piic->data, piic->len);
}

//�����жϴ����д��󷵻�1�����򷵻�0
static uint8_t IIC_Hard_Master_Error_Deal(void)
{
	uint8_t flag = 0;  //�����־
	
	if(I2C_GetFlagStatus(I2C_FLAG_BUSERROR))  //�ж����ߴ��󣬽��մ�����ʼ��ֹͣ
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_BUSERROR);  //��0
		flag = 1;
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ACKNOWLEDGEFAILURE))  //�ж�Ӧ��ʧ�ܣ����շ�Ӧ��
	{
		I2C_GenerateSTOP(ENABLE);
		I2C_ClearFlag(I2C_FLAG_ACKNOWLEDGEFAILURE);
		flag = 1;
	}
	if(I2C_GetFlagStatus(I2C_FLAG_ARBITRATIONLOSS))  //�ж��ٲ�ʧ��
	{
		I2C_ClearFlag(I2C_FLAG_ARBITRATIONLOSS);
		flag = 1;
	}
	
	return flag;
}

//I2C�жϴ���
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
	IIC_Hard_Master_WriteRead(piic_wrinfo);
}
