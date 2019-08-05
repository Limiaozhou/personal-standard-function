#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

iic_slave_be_written  port1_be_written;  //IIC�˿�1��д����
iic_slave_be_read     port1_be_read;  //IIC�˿�1��������

uint8_t start_flag = 1;  //������ʼ�źű�־���ɹ���0
uint8_t stop_flag = 1;  //����ֹͣ�źű�־���ɹ���0

static uint8_t IIC_Slave_Start(uint8_t port);  //�ӻ������ʼ�ź�
static uint8_t IIC_Slave_Stop(uint8_t port);  //�ӻ����ֹͣ�ź�
static void IIC_Slave_SendAck(uint8_t port, uint8_t ack);  //�ӻ�����Ӧ���ź�
static uint8_t IIC_Slave_ReceiveAck(uint8_t port);  //�ӻ�����Ӧ���ź�


//��ʼ����SCL���жϣ�SDA���½����жϣ��Լ�⿪ʼ�ź�
void IIC_Slave_Init(uint8_t port, iic_slave_be_written be_written_cb, iic_slave_be_read be_read_cb)
{
    switch(port)
    {
        case 1 :
            port1_be_written = be_written_cb;  //����д�ص�����������IIC�˿�1��д����
			port1_be_read = be_read_cb;  //�������ص�����������IIC�˿�1��������
			
			IIC_SLAVE_SDA_DIR_IN();  //SDA����
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA�������룬�������
            IIC_SLAVE_SCL_UP();  //SCL�������룬�������
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
            // IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش�������SDAΪͬһ�˿ڣ���SDA֮��ѡһ����
            IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
            IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
        break;
        
        default :
        break;
    }
}

//�ӻ������ʼ�źţ�SDA�½��أ�SCL���ڸߵ�ƽ���ɹ��Ż�0
static uint8_t IIC_Slave_Start(uint8_t port)
{
    switch(port)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x02))  //SCL���ڸߵ�ƽ��SDA�½���
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
				// IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
                IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
                IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
                return 0;  //�ɹ�����0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //ʧ�ܷ���1
}

//�ӻ����ֹͣ�źţ�SDA�����أ�SCL���ڸߵ�ƽ���ɹ��Ż�0
static uint8_t IIC_Slave_Stop(uint8_t port)
{
   switch(port)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x01))  //SCL���ڸߵ�ƽ��SDA������
           {
               IIC_SLAVE_SDA_DIR_IN();  //SDA����
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
               // IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
               IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
               IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
               return 0;  //�ɹ�����0
           }
       break;
       
       default :
       break;
   }
   
   return 1;  //ʧ�ܷ���1
}

//�ӻ�����Ӧ���źţ�0:ACK��1:NAK
static void IIC_Slave_SendAck(uint8_t port, uint8_t ack)
{
   switch(port)
   {
       case 1 :
		   IIC_SLAVE_SDA_DIR_OUT();
           IIC_SLAVE_SDA_OUT(ack);
           IIC_SLAVE_SDA_DIR_IN();
       break;
       
       default :
       break;
   }
}

//�ӻ�����Ӧ���źţ�0:ACK��1:NAK
static uint8_t IIC_Slave_ReceiveAck(uint8_t port)
{
   uint8_t ack = 1;
   
   switch(port)
   {
       case 1 :
           ack = IIC_SLAVE_SDA_PIN_IN;
       break;
       
       default :
       break;
   }
   
   return ack;
}

//�ӻ����������źţ�����һ�ֽڷ���Ӧ���źţ����򷵻�-1
static int8_t IIC_Slave_SendByte(uint8_t port, uint8_t byte)
{
	uint8_t bit = 1;
	
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //����bitλ����¼
			
			IIC_SLAVE_SDA_DIR_OUT();
			
			if(i++ < 8)  //ǰ8�����巢�����ݸ�bitλ
			{
				bit = !!(byte & (0x80 >> i));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
				IIC_SLAVE_SDA_OUT(bit);
			}
			else  //��9���������Ӧ�𣬲�����Ӧ���ź�
			{
				IIC_SLAVE_SDA_DIR_IN();
				i = 0;
				return IIC_Slave_ReceiveAck(port);
			}
        break;
        
        default :
        break;
    }
	
	return -1;
}

//�ӻ�����һ�ֽڣ�������һ���ֽڣ��ٽ��յ�9������ʱ����0�����򷵻�1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //����bitλ����¼
			
			if(!i)
				*byte = 0;  //��ʼ��0��֮���ٸ�ֵ
			
            if(i++ < 8)
                *byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
			else
			{
				i = 0;
				return 0;
			}
        break;
        
        default :
        break;
    }

    return 1;
}

//�ӻ�����ָ����������


//�ӻ�����ָ����������
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint8_t len)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //�����ֽ�����¼
			static uint8_t byte = 0;  //���ݻ���
			
			if(!IIC_Slave_ReceiveByte(port, &byte))
			{
				*(data + i) = byte;  //������һ���ֽ��ٸ�ֵ
				if(i < len - 1)
				{
					IIC_Slave_SendAck(port, 0);  //������һ���ֽڣ�����һ����ЧӦ���ź�
					i++;
				}
				else
				{
					IIC_Slave_SendAck(port, 1);  //�������������ݣ�����һ����Ӧ���ź�
					i = 0;
				}
			}
        break;
        
        default :
        break;
    }
}

//TTC�ӻ�������ͣ�ź�
void IIC_Slave_Start_Judge(uint8_t port)
{
	if(start_flag)  //������ʼ�źź����������ٽ��գ�ֱ���������½�������
		start_flag = IIC_Slave_Start(port);  //������ʼ�ɹ���Ϊ0
	
	if(!IIC_Slave_Stop(port))  //����ֹͣ�źųɹ���Ϊ0
		start_flag = 1;
}

//IIC�ӻ����������ź�
void IIC_Slave_Data_Deal(uint8_t port)
{
	
	
	if(!start_flag)  
	{
		
	}
}

//STM8 PB�ڽ����жϺ�����SDA��SCL����
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
}
