#include "bsp_iic_master.h"

//�ڲ�����
//__weak static void delay_us(uint16_t nus);  //��ʱn us���ڲ�ʹ��
//__weak static void delay_ms(uint16_t nms);  //��ʱn ms

static void IIC_Master_Start(uint8_t port);  //��ʼ�ź�
static void IIC_Master_Stop(uint8_t port);  //ֹͣ�ź�

static void IIC_Master_SendAck(uint8_t port, uint8_t ack);  //����Ӧ���ź�
static uint8_t IIC_Master_ReceiveAck(uint8_t port);  //����Ӧ���ź�

static uint8_t IIC_Master_SendByte(uint8_t port, uint8_t byte);  //����һ���ֽ�
static uint8_t IIC_Master_ReceiveByte(uint8_t port, uint8_t ack);  //����һ���ֽ�

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

//��ʼ��Ϊ����״̬��scl��sda��Ϊ�ߣ�scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
void IIC_Master_Init(uint8_t port)
{
    switch(port)
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
static void IIC_Master_Start(uint8_t port)
{
    switch(port)
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
static void IIC_Master_Stop(uint8_t port)
{
    switch(port)
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
static void IIC_Master_SendAck(uint8_t port, uint8_t ack)
{
    switch(port)
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
static uint8_t IIC_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
    switch(port)
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
static uint8_t IIC_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
    switch(port)
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

    return IIC_Master_ReceiveAck(port);
}

//����һ�ֽڣ�������Ӧ��
static uint8_t IIC_Master_ReceiveByte(uint8_t port, uint8_t ack)
{
    uint8_t i, byte = 0;
    
    switch(port)
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
	
	IIC_Master_SendAck(port, ack);  //������һ���ֽڣ�����Ӧ��
	
    return byte;
}

//д�豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Master_Write(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //���������ֽ�����¼
	uint8_t error_flag = 0;  //���ʹ����־
    
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
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //�豸�͵�ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //�豸��ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		for(i = 0; i < piic->len; i++)
		{
			if(IIC_Master_SendByte(piic->port, *(piic->data++)))  //����
			{
				IIC_Master_Stop(piic->port);
				error_flag = 1;  //��1���ʹ����־
				break;  //���ͳ����ֹͣ������ǰforѭ��
			}
		}
		
		if(!error_flag)
		{
			IIC_Master_Stop(piic->port);
			return 0;  //��û�г���ֹͣ���������������0
		}
		
		error_flag = 0;  //��0���ʹ����־
	}
    
    return 1;  //����3�ζ����󣬷���1
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
    
	while(piic->error_resend_times--)
	{
		//��д�Ĵ�����ַ
		IIC_Master_Start(piic->port);
        if(piic->dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //�豸�͵�ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //�豸��ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		if(piic->reg_adr_twobyte_flag)  //�ӻ�2�ֽڼĴ�����ַģʽ�ж�
		{
			if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr >> 8)))  //�Ĵ����ߵ�ַ
			{
				IIC_Master_Stop(piic->port);
				continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
			}
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr & 0xFF)))  //�Ĵ����͵�ַ
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
		}
        else
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->register_adr & 0xFF)))  //�Ĵ�����ַ
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
        }
		
		//�ٶ�����
		IIC_Master_Start(piic->port);
        if(piic->dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //�豸�͵�ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
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
		if(piic->readreg_delay_flag)
			delay_ms(piic->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
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
