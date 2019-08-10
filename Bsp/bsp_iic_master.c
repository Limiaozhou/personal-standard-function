#include "bsp_iic_master.h"

//�ڲ�����
static void delay_us(uint16_t nus);  //��ʱn us���ڲ�ʹ��

static void IIC_Master_Start(uint8_t type);  //��ʼ�ź�
static void IIC_Master_Stop(uint8_t type);  //ֹͣ�ź�

static void IIC_Master_SendAck(uint8_t type, uint8_t ack);  //����Ӧ���ź�
static uint8_t IIC_Master_ReceiveAck(uint8_t type);  //����Ӧ���ź�

static uint8_t IIC_Master_SendByte(uint8_t type, uint8_t byte);  //����һ���ֽ�
static uint8_t IIC_Master_ReceiveByte(uint8_t type);  //����һ���ֽ�

//��ʱn us���ڲ�ʹ��
static void delay_us(uint16_t nus)
{
	while (nus--)
	{
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
}

//��ʼ��Ϊ����״̬��scl��sda��Ϊ�ߣ�scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
void IIC_Master_Init(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_DIR_OUT();  //SDA���
            IIC_MASTER_SCL_DIR_OUT();
            IIC_MASTER_SDA_UP();  //SDA�������룬�������
            IIC_MASTER_SCL_UP();  //SCL�������룬�������
            IIC_MASTER_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        case 2 :
            IIC_MASTER2_SDA_DIR_OUT();  //SDA���
            IIC_MASTER2_SCL_DIR_OUT();
            IIC_MASTER2_SDA_UP();  //SDA�������룬�������
            IIC_MASTER2_SCL_UP();  //SCL�������룬�������
            IIC_MASTER2_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER2_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        case 3 :
            IIC_MASTER3_SDA_DIR_OUT();  //SDA���
            IIC_MASTER3_SCL_DIR_OUT();
            IIC_MASTER3_SDA_UP();  //SDA�������룬�������
            IIC_MASTER3_SCL_UP();  //SCL�������룬�������
            IIC_MASTER3_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER3_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        default :
        break;
    }
}

//��ʼ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɸߵ�ƽ��ɵ͵�ƽʱ
static void IIC_Master_Start(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//ֹͣ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɵ͵�ƽ��ɸߵ�ƽ
static void IIC_Master_Stop(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(LOW);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(LOW);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(LOW);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//����Ӧ���źţ�0:ACK��1:NAK
static void IIC_Master_SendAck(uint8_t type, uint8_t ack)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(ack);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(ack);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(ack);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//����Ӧ���źţ�0:ACK��1:NAK
static uint8_t IIC_Master_ReceiveAck(uint8_t type)
{
    uint8_t ack = 1;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);

            IIC_MASTER_SDA_DIR_IN();

            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER_SDA_PIN_IN;
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);

            IIC_MASTER2_SDA_DIR_IN();

            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER2_SDA_PIN_IN;
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);

            IIC_MASTER3_SDA_DIR_IN();

            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER3_SDA_PIN_IN;
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return ack;
}

//����һ�ֽڣ�����Ӧ���ź�
static uint8_t IIC_Master_SendByte(uint8_t type, uint8_t byte)
{
    uint8_t i, bit = 1;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
                IIC_MASTER_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 2 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER2_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 3 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER3_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        default :
        break;
    }

    return IIC_Master_ReceiveAck(type);
}

//����һ�ֽ�
static uint8_t IIC_Master_ReceiveByte(uint8_t type)
{
    uint8_t i, byte = 0;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER_SDA_PIN_IN << (7 - i);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER2_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER2_SDA_PIN_IN << (7 - i);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER3_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER3_SDA_PIN_IN << (7 - i);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return byte;
}

//д���ݵ��豸�Ĵ������ɹ�����0��ʧ�ܷ���1
uint8_t IIC_Master_Write(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len)
{
    uint8_t i;  //���������ֽ�
	uint8_t resend_times = 3;  //ʧ���ط�����
	uint8_t state;  //����״̬
    
	while(resend_times--)
	{
		IIC_Master_Start(type);

		if(IIC_Master_SendByte(type, device_adr + 0))  //�豸��ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
		{
			IIC_Master_Stop(type);
			return 1;
		}
		if(IIC_Master_SendByte(type, reg_adr))  //�Ĵ�����ַ
		{
			IIC_Master_Stop(type);
			return 1;
		}
		for(i = 0; i < len; i++)
		{
			if(IIC_Master_SendByte(type, *(data++)))  //�Ĵ�������
			{
				IIC_Master_Stop(type);
				return 1;
			}
		}
		
		IIC_Master_Stop(type);
	}
    
    return 0;
}

//���豸ָ���������ݣ������źţ���ʧ����������
void IIC_Master_Read(uint8_t type, uint8_t device_adr, uint8_t reg_adr, uint8_t * data, uint8_t len)
{
    uint8_t i;
    
    IIC_Master_Start(type);
    if(IIC_Master_SendByte(type, device_adr + 1))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
    {
        IIC_Master_Stop(type);
        return;
    }
    for(i = 0; i < len; i++)
    {
        *(data + i) = IIC_Master_ReceiveByte(type);
        if(i < len - 1)
            IIC_Master_SendAck(type, 0);  //����һ���ֽڣ�����һ����ЧӦ���ź�
    }
    IIC_Master_SendAck(type, 1);  //���������ݣ�����һ����Ӧ���ź�

    IIC_Master_Stop(type);
}
