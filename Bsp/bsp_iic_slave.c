#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

iic_slave_be_written  port1_be_written;  //IIC�˿�1��д����
iic_slave_be_read     port1_be_read;  //IIC�˿�1��������

uint8_t start_flag = 1;  //������ʼ�źű�־���ɹ�Ϊ0�����ɿ�ʼ���յ�ַ�������ź�
uint8_t stop_receive_flag = 1;  //��д�ֽ�ʱ����ͬʱ����ֹͣ�źű�־����Ϊ0
uint8_t sda_interrupt_flag = 1;  //sda�����жϿ��ر�־����Ϊ0����ֹSDA��SCL����ͬһ�˿�ʱ��SCL�������ж�

static uint8_t IIC_Slave_Start(uint8_t port);  //�ӻ������ʼ�źţ��ɹ��Ż�0
static uint8_t IIC_Slave_Stop(uint8_t port);  //�ӻ����ֹͣ�źţ��ɹ��Ż�0

static void IIC_Slave_SendAck(uint8_t port, uint8_t ack);  //�ӻ�����Ӧ���źţ�0:ACK��1:NAK
static uint8_t IIC_Slave_ReceiveAck(uint8_t port);  //�ӻ�����Ӧ���źţ�0:ACK��1:NAK

static int8_t IIC_Slave_SendByte(uint8_t port, uint8_t byte);  //�ӻ�����һ�ֽڣ�����һ�ֽڣ��ڵ�9�����巵��Ӧ���źţ����򷵻�-1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte);  //�ӻ�����һ�ֽڣ�������һ���ֽڣ��ڵ�9������ʱ����0�����򷵻�1

static int8_t IIC_Slave_Write(uint8_t port, uint8_t * data, uint16_t len);  //�ӻ�����ָ���������ݣ�����һ���ֽ��з���-1������һ���ֽڷ���0����������������ݻ��յ���Ӧ���źŷ���1������������ֹͣ
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint16_t len);  //�ӻ�����ָ���������ݣ���һ���ֽڷ���Ӧ��󣬿�������ֹͣ

static void IIC_Slave_SoP_Judge(uint8_t port);  //TTC�ӻ������ж���ͣ�ź�

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
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
        break;
        
        default :
        break;
    }
}

//�ӻ������ʼ�źţ�SDA�½��أ�SCL���ڸߵ�ƽ���ɹ��Ż�0��SDA���жϣ�SCL���������жϣ��Խ�������
static uint8_t IIC_Slave_Start(uint8_t port)
{
    switch(port)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x02) && (!sda_interrupt_flag))  //SCL���ڸߵ�ƽ��SDA�½���
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
				// IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
                IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
                IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
				
				sda_interrupt_flag = 1;  //sda�����жϿ��ر�־��
				
                return 0;  //�ɹ�����0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //ʧ�ܷ���1
}

//�ӻ����ֹͣ�źţ�SDA�����أ�SCL���ڸߵ�ƽ���ɹ��Ż�0��SCL���жϣ�SDA���½����жϣ��Լ�⿪ʼ�ź�
static uint8_t IIC_Slave_Stop(uint8_t port)
{
   switch(port)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x01) && (!sda_interrupt_flag))  //SCL���ڸߵ�ƽ��SDA������
           {
			   IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
               // IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
               IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
               
			   sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
			   
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
           IIC_SLAVE_SDA_OUT(ack);  //�������ackӦ��
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
           ack = IIC_SLAVE_SDA_PIN_IN;  //��������ackӦ��
       break;
       
       default :
       break;
   }
   
   return ack;
}

//�ӻ�����һ�ֽڣ�����һ�ֽڣ��ٵ�9�����巵��Ӧ���źţ����򷵻�-1
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
				IIC_SLAVE_SDA_OUT(bit);  //�������bit
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

//�ӻ�����һ�ֽڣ�������һ���ֽڣ��ڵ�9������ʱ����0�����򷵻�1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //����bitλ����¼
			
			if(!i)
				*byte = 0;  //��ʼ��0��֮���ٸ�ֵ
			
            if(i++ < 8)  //ǰ8������������ݸ�bitλ
                *byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
			else  //��9�����巵��0�����򷵻�1
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

//�ӻ�����ָ���������ݣ�����һ���ֽ��з���-1������һ���ֽڷ���0����������������ݻ��յ���Ӧ���źŷ���1������������ֹͣ
static int8_t IIC_Slave_Write(uint8_t port, uint8_t * data, uint16_t len)
{
	static uint8_t i = 0;  //�����ֽ�����¼
	int8_t state = -1;  //�����ֽں�������״̬����
	
	state = IIC_Slave_SendByte(port, *(data + i));  //�����ֽ�bitλ�������ط���״̬
	
	if(state != (-1))  //������һ�ֽڣ��ڵ�9�����岻�Ƿ���-1
	{
		if(state == 0)  //����ΪACKӦ���ź�
			i++;
		else if(state == 1)  //����ΪNACK��Ӧ���ź�
			i = 0;
		
		if((i >= len) || (state == 1))  //�����ֽ����ﵽ������յ���Ӧ���źţ�ֹͣ���ͣ���ʼ����ֹͣ�ź�
		{
			IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
			IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
			IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
			start_flag = 1;  //ֹͣ���յ�ַ�������ź�
			i = 0;
		}
	}
	
	return state;
}

//�ӻ�����ָ���������ݣ���һ���ֽڷ���Ӧ��󣬿�������ֹͣ�������귵��0�����򷵻�1
static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data, uint16_t len)
{
    switch(port)
    {
        case 1 :
			static uint8_t i = 0;  //�����ֽ�����¼
			static uint8_t byte = 0;  //���ݻ���
			
			if(!IIC_Slave_ReceiveByte(port, &byte))  //������һ���ֽڣ���9�����巵��0
			{
				*(data + i) = byte;  //������һ���ֽ��ٸ�ֵ
				
				if(i < len - 1)
				{
					IIC_Slave_SendAck(port, 0);  //������һ���ֽڣ�����һ����ЧӦ���ź�
					
					IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
					IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
					
					sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
					stop_receive_flag = 0;  //��д�ֽ�ʱ����ͬʱ����ֹͣ�źű�־��
					i++;
				}
				else    //�����ֽ����ﵽ��ֹͣ�������ݣ���ʼֻ����ֹͣ�ź�
				{
					IIC_Slave_SendAck(port, 1);  //�������������ݣ�����һ����Ӧ���ź�
					
					IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
					IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
					// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
					IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
					
					sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
					start_flag = 1;  //ֹͣ���յ�ַ�������ź�
					i = 0;
					
					return 0;
				}
			}
        break;
        
        default :
        break;
    }
	
	return 1;
}

//TTC�ӻ������ж���ͣ�ź�
static void IIC_Slave_SoP_Judge(uint8_t port)
{
	if(!IIC_Slave_Start(port))  //������ʼ�ɹ���Ϊ0
		start_flag = 0;  //������������
	
	if(!IIC_Slave_Stop(port))  //����ֹͣ�ɹ���Ϊ0
	{
		start_flag = 1;  //�رս�������
	}
	else if(!stop_receive_flag)  //����ֹͣʧ�ܣ���Ϊ��д�ֽ�ʱ����ֹͣ״̬��
	{
		IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
		// IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
		IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
		
		sda_interrupt_flag = 1;  //sda�����жϿ��ر�־��
		stop_receive_flag = 1;  //��д�ֽ�ʱ����ͬʱ����ֹͣ�źű�־��
	}
		
}

//TTC�ӻ������ж��豸7λ��ַ����ַ��ȷ����Ϊд����0��Ϊ������1�����󷵻�2����ַ����δ��ɷ���-1
static int8_t IIC_Slave_Address_Judge(uint8_t port)
{
	uint8_t address = 0;  //��ȡ��ַ����
	static uint8_t byte = 0;  //���ݻ���
	int8_t state = -1;  //״̬����
	
	if(!IIC_Slave_ReceiveByte(port, &byte))  //������һ���ֽڣ���9�����巵��0
	{
		address = byte;  //������һ���ֽ��ٸ�ֵ
		
		if((address & (~0x01)) == (IIC_DEVICE_ADDRESS & (~0x01)))  //�ж�7λ��ַ
		{
			IIC_Slave_SendAck(port, 0);  //��ַ��ȷ��Ӧ���ź�
			
			if(!(address & 0x01))  //bit0Ϊ0��Ϊд
				state = 0;
			else  //bit0Ϊ1��Ϊ��
				state = 1;
		}
		else  //��ַ����
			state = 2;
	}
	else
		state = -1;  //���յ�ַ�����У�����-1
	
	if(state == 2)  //��ַ���󣬿�ʼ����ֹͣ�ź�
	{
		IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
		IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
		// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
		IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
		
		sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
		start_flag = 1;  //ֹͣ���յ�ַ�������ź�
	}
	
	return state;
}

//IIC�ӻ����������ź�
static void IIC_Slave_Data_Deal(uint8_t port)
{
	if(!start_flag)  //������ʼ�ɹ�
	{
		if()
	}
}

//STM8 PB�ڽ����жϺ�����SDA��SCL����
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
}
