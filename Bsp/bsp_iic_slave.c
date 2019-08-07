#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

#define READ_NUM_MAX 256  //�������������2^8

iic_slave_be_written  port1_be_written;  //IIC�˿�1��д����
iic_slave_be_read     port1_be_read;  //IIC�˿�1��������

uint8_t start_flag = 1;  //������ʼ�źű�־���ɹ�Ϊ0�����ɿ�ʼ���յ�ַ�������ź�
uint8_t stop_receive_flag = 1;  //��д�ֽ�ʱ��ֹͣ�������ݱ�־λ��0��һ�ֽ��ꣻ1δֹͣ��2ֹͣ
uint8_t sda_interrupt_flag = 1;  //sda�����жϿ��ر�־����Ϊ0����ֹSDA��SCL����ͬһ�˿�ʱ��SCL�������ж�

uint16_t read_num = 0;  //��дʱ�����������ֽ���
uint16_t read_len = 0;  //��дʱ�����պʹ��ݸ���д�ص����������ֽ���

static void IIC_Slave_State_GetStopOnly(uint8_t port);  //ֻ����ֹͣ�ź�״̬
static void IIC_Slave_State_GetStop(uint8_t port);  //�ɽ���ֹͣ�ź�״̬
static void IIC_Slave_State_GetStartOnly(uint8_t port);  //ֻ���տ�ʼ�ź�״̬
static void IIC_Slave_State_GetStart(uint8_t port);  //�ɽ��տ�ʼ�ź�״̬
static void IIC_Slave_State_GetDataOnly(uint8_t port);  //ֻ���������ź�״̬
static void IIC_Slave_State_GetData(uint8_t port);  //�ɽ��������ź�״̬

static uint8_t IIC_Slave_Start(uint8_t port);  //�ӻ������ʼ�źţ��ɹ��Ż�0
static uint8_t IIC_Slave_Stop(uint8_t port);  //�ӻ����ֹͣ�źţ��ɹ��Ż�0

static void IIC_Slave_SendAck(uint8_t port, uint8_t ack);  //�ӻ�����Ӧ���źţ�0:ACK��1:NAK
static uint8_t IIC_Slave_ReceiveAck(uint8_t port);  //�ӻ�����Ӧ���źţ�0:ACK��1:NAK

static int8_t IIC_Slave_SendByte(uint8_t port, uint8_t byte);  //�ӻ�����һ�ֽڣ�����һ�ֽڣ��ڵ�9�����巵��Ӧ���źţ����򷵻�-1
static uint8_t IIC_Slave_ReceiveByte(uint8_t port, uint8_t * byte);  //�ӻ�����һ�ֽڣ�������һ���ֽڣ��ڵ�9������ʱ����0�����򷵻�1

static uint8_t IIC_Slave_Read(uint8_t port, uint8_t * data);  //�ӻ�����ָ���������ݣ���һ���ֽڷ���Ӧ��󣬿�������ֹͣ

static void IIC_Slave_SoP_Judge(uint8_t port);  //TTC�ӻ������ж���ͣ�ź�
static int8_t IIC_Slave_Address_Judge(uint8_t port);  //TTC�ӻ������ж��豸7λ��ַ����ַ��ȷ������Ӧ����Ϊд����0��Ϊ������1�����󷵻�2�����������ֹͣ�ź�״̬����ַ����δ��ɷ���-1
static void IIC_Slave_Data_Deal(uint8_t port);  //IIC�ӻ����������ź�

void data_clear(uint8_t * data, uint16_t len);  //��0ָ����������

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

//ֻ����ֹͣ�ź�״̬��SCL�ж�����أ�SDA�����ش���
static void IIC_Slave_State_GetStopOnly(uint8_t port)
{
	switch(port)
    {
        case 1 :
			IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
			IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
			IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
		break;
        
        default :
        break;
    }
}

//�ɽ���ֹͣ�ź�״̬
static void IIC_Slave_State_GetStop(uint8_t port)
{
	switch(port)
    {
        case 1 :
			IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
			IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
		break;
        
        default :
        break;
    }
}

//ֻ���տ�ʼ�ź�״̬��SCL�ж�����أ�SDA�½��ش���
static void IIC_Slave_State_GetStartOnly(uint8_t port)
{
	switch(port)
    {
        case 1 :
			IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
			IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
			IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
		break;
        
        default :
        break;
    }
}

//�ɽ��տ�ʼ�ź�״̬
static void IIC_Slave_State_GetStart(uint8_t port)
{
	switch(port)
    {
        case 1 :
			IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
			// IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
			IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
			
			sda_interrupt_flag = 0;  //sda�����жϿ��ر�־��
		break;
        
        default :
        break;
    }
}

//ֻ���������ź�״̬��SDA���жϣ�SCL���������ж�
static void IIC_Slave_State_GetDataOnly(uint8_t port)
{
	switch(port)
    {
        case 1 :
			IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
			// IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
			IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
			IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
			
			sda_interrupt_flag = 1;  //sda�����жϿ��ر�־��
		break;
        
        default :
        break;
    }
}

//�ɽ��������ź�״̬
static void IIC_Slave_State_GetData(uint8_t port)
{
	switch(port)
    {
        case 1 :
			// IIC_SLAVE_SDA_EXTI_UP();  //SDA�����ش������Խ���ֹͣ�ź�
			IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
			IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
		break;
        
        default :
        break;
    }
}

//�ӻ������ʼ�źţ�SDA�½��أ�SCL���ڸߵ�ƽ���ɹ��Ż�0��SDA���жϣ�SCL���������жϣ�ֻ��������
static uint8_t IIC_Slave_Start(uint8_t port)
{
    switch(port)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN && (IIC_SLAVE_SDA_EXTI_EDGE == 0x02) && (!sda_interrupt_flag))  //SCL���ڸߵ�ƽ��SDA�½���
            {
				IIC_Slave_State_GetDataOnly(port);  //ֻ���������ź�״̬
				
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
			   IIC_Slave_State_GetStartOnly(port);  //ֻ���տ�ʼ�ź�״̬
			   
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
			static uint8_t num = 0;  //����bitλ����¼
			
			IIC_SLAVE_SDA_DIR_OUT();
			
			if(num++ < 8)  //ǰ8�����巢�����ݸ�bitλ
			{
				bit = !!(byte & (0x80 >> num));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
				IIC_SLAVE_SDA_OUT(bit);  //�������bit
			}
			else  //��9���������Ӧ�𣬲�����Ӧ���ź�
			{
				IIC_SLAVE_SDA_DIR_IN();
				num = 0;
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
			static uint8_t num = 0;  //����bitλ����¼
			
			if(!num)
				*byte = 0;  //��ʼ��0��֮���ٸ�ֵ
			
            if(num++ < 8)  //ǰ8������������ݸ�bitλ
                *byte |= IIC_SLAVE_SDA_PIN_IN << (7 - num);
			else  //��9�����巵��0�����򷵻�1
			{
				num = 0;
				return 0;
			}
        break;
        
        default :
        break;
    }

    return 1;
}

//�ӻ�����ָ���������ݣ�����һ���ֽ��з���-1������һ���ֽڷ���0����������������ݻ��յ���Ӧ���źŷ���1����ֻ����ֹͣ
int8_t IIC_Slave_Write(uint8_t port, uint8_t * data, uint16_t len)
{
	static uint16_t num = 0;  //�����ֽ�����¼
	int8_t state = -1;  //�����ֽں�������״̬����
	
	state = IIC_Slave_SendByte(port, *(data + num));  //�����ֽ�bitλ�������ط���״̬
	
	if(state != (-1))  //������һ�ֽڣ��ڵ�9�����岻�Ƿ���-1
	{
		if(state == 0)  //����ΪACKӦ���ź�
			num++;
		else if(state == 1)  //����ΪNACK��Ӧ���ź�
			num = 0;
		
		if((num >= len) || (state == 1))  //�����ֽ����ﵽ������յ���Ӧ���źţ�ֹͣ���ͣ���ʼ����ֹͣ�ź�
		{
			IIC_Slave_State_GetStopOnly(port);  //ֻ����ֹͣ�ź�״̬
			
			start_flag = 1;  //ֹͣ���յ�ַ�������ź�
			num = 0;
		}
	}
	
	return state;
}

//�ӻ�����ָ���������ݣ���һ���ֽڷ���Ӧ��󣬿����ɽ���ֹͣ�������ֽ����ﵽ���ֵ��ֹͣ�������ݣ�ֻ����ֹͣ�ź�
static void IIC_Slave_Read(uint8_t port, uint8_t * data)
{
	static uint8_t byte = 0;  //���ݻ���
	
	if(!IIC_Slave_ReceiveByte(port, &byte))  //������һ���ֽڣ���9�����巵��0
	{
		*(data + read_num) = byte;  //������һ���ֽ��ٸ�ֵ
		read_num++;
		read_len = read_num;
		
		if(read_num < READ_NUM_MAX)
		{
			IIC_Slave_SendAck(port, 0);  //������һ���ֽڣ�����һ����ЧӦ���ź�
			
			IIC_Slave_State_GetStop(port);  //�ɽ���ֹͣ�ź�״̬
			
			stop_receive_flag = 0;
		}
		else    //�����ֽ����ﵽ���ֵ��ֹͣ�������ݣ���ʼֻ����ֹͣ�ź�
		{
			IIC_Slave_SendAck(port, 1);  //�������������ݣ�����һ����Ӧ���ź�
			
			IIC_Slave_State_GetStopOnly(port);  //ֻ����ֹͣ�ź�״̬
			
			stop_receive_flag = 2;
			start_flag = 1;  //ֹͣ���յ�ַ�������ź�
			read_num = 0;
		}
	}
}

//TTC�ӻ������ж���ͣ�ź�
static void IIC_Slave_SoP_Judge(uint8_t port)
{
	if(!IIC_Slave_Start(port))  //������ʼ�ɹ���Ϊ0
		start_flag = 0;  //������������
	
	if(!IIC_Slave_Stop(port))  //����ֹͣ�ɹ���Ϊ0
	{
		start_flag = 1;  //�رս�������
		
		if(!stop_receive_flag)
			stop_receive_flag = 2;
	}
	else if(!stop_receive_flag)  //��д�ֽ�ʱ��ֹͣ�������ݱ�־λ��0��һ�ֽ��ꣻ1δֹͣ��2ֹͣ
	{
		IIC_Slave_State_GetDataOnly(port);  //ֻ���������ź�״̬
		
		stop_receive_flag = 1;
	}
		
}

//TTC�ӻ������ж��豸7λ��ַ����ַ��ȷ������Ӧ����Ϊд����0��Ϊ������1�����󷵻�2�����������ֹͣ�ź�״̬����ַ����δ��ɷ���-1
static int8_t IIC_Slave_Address_Judge(uint8_t port)
{
	static uint8_t byte = 0;  //���ݻ���
	uint8_t address = 0;  //��ȡ��ַ����
	int8_t state = -1;  //״̬����
	
	if(!IIC_Slave_ReceiveByte(port, &byte))  //������һ���ֽڣ���9�����巵��0
	{
		address = byte;  //������һ���ֽ��ٸ�ֵ
		
		if((address & (~0x01)) == (IIC_DEVICE_ADDRESS & (~0x01)))  //�ж�7λ��ַ
		{
			IIC_Slave_SendAck(port, 0);  //��ַ��ȷ��Ӧ���ź�
			
			state = address & 0x01;  //ȡ��д״̬
		}
		else  //��ַ����
			state = 2;
	}
	else
		state = -1;  //���յ�ַ�����У�����-1
	
	if(state == 2)  //��ַ���󣬿�ʼֻ����ֹͣ�ź�
	{
		IIC_Slave_State_GetStopOnly(port);  //ֻ����ֹͣ�ź�״̬
		
		start_flag = 1;  //ֹͣ���յ�ַ�������ź�
	}
	
	return state;
}

//IIC�ӻ����������ź�
static void IIC_Slave_Data_Deal(uint8_t port)
{
	static int8_t state = -1;  //��ַ�жϱ�־
	static uint8_t read_buf[READ_NUM_MAX] = {0};  //�������ݻ���
	
	if(!start_flag)  //������ʼ�ɹ�
	{
		if((state == -1) || (state == 2))  //��ַ����δ��ɻ����ʱ����������Ϊ��ַ
			state = IIC_Slave_Address_Judge(port);  //�жϽ��յ�ַ
		else if(state == 0)  //����Ϊд�źţ���ȡ���յ������ݵ�����
			IIC_Slave_Read(port, read_buf);
		else if(state == 1)  //����Ϊ���źţ��������ݣ����ñ����ص�����
		{
			port1_be_read();  //���ñ����ص�����
			state = -1;  //���½��յ�ַ
		}
	}
	
	if(stop_receive_flag == 2)  //��дʱ�����������ݣ��������ݵ���д�ص�����
	{
		port1_be_written(read_buf, read_len);  //�������ݵ���д�ص�����
		data_clear(read_buf, read_len);  //��0�Ѵ�������
		
		read_num = 0;  //��0�ѽ������ݳ��ȣ���ͷ��ʼ
		read_len = 0;
		start_flag = 1;  //ֹͣ���յ�ַ�������ź�
		state = -1;  //���½��յ�ַ
	}
}

//��0ָ����������
void data_clear(uint8_t * data, uint16_t len)
{
	while(len--)
		*(data++) = 0;
}

//STM8 PB�ڽ����жϺ�����SDA��SCL����
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
	IIC_Slave_SoP_Judge(1);  //��ַ�ж�
	IIC_Slave_Data_Deal(1);  //���ݴ���
}
