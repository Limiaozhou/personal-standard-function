#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

//��ʼ����SCL���жϣ�SDA���½����жϣ��Լ�⿪ʼ�ź�
void IIC_Slave_Init(uint8_t type)
{
    switch(type)
    {
        case 1 :
            IIC_SLAVE_SDA_DIR_IN();  //SDA����
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA�������룬�������
            IIC_SLAVE_SCL_UP();  //SCL�������룬�������
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
            IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
            IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪������������Խ�����ʼ�ź�
            IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
        break;
        
        default :
        break;
    }
}

//�ӻ������ʼ�źţ�SDA�½��أ�SCL���ڸߵ�ƽ���ɹ��Ż�0
static uint8_t IIC_Slave_Start(uint8_t type)
{
    switch(type)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN)  //SCL���ڸߵ�ƽ���ɹ�
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
                IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���������SCL�ߵ�ƽʱ���SDA��ƽ�����������ź�
                IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
                return 0;  //����0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //ʧ�ܷ���1
}

//�ӻ����ֹͣ�źţ�SDA�����أ�SCL���ڸߵ�ƽ���ɹ��Ż�0
static uint8_t IIC_Slave_Stop(uint8_t type)
{
   switch(type)
   {
       case 1 :
           if(IIC_SLAVE_SCL_PIN_IN)  //SCL���ڸߵ�ƽ���ɹ�
           {
               IIC_SLAVE_SDA_DIR_IN();  //SDA����
               IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش������Խ�����ʼ�ź�
               IIC_SLAVE_SCL_EXTI_UP();  //SCL�����ش���
               IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪������������Խ�����ʼ�ź�
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
static void IIC_Slave_SendAck(uint8_t type, uint8_t ack)
{
   switch(type)
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
static uint8_t IIC_Slave_ReceiveAck(uint8_t type)
{
   uint8_t ack = 1;
   
   switch(type)
   {
       case 1 :
           ack = IIC_SLAVE_SDA_PIN_IN;
       break;
       
       default :
       break;
   }
   
   return ack;
}

//�ӻ����������ź�
static void IIC_Slave_SendBit(uint8_t type, uint8_t bit)
{
    switch(type)
    {
        case 1 :
			IIC_SLAVE_SDA_DIR_OUT();
			IIC_SLAVE_SDA_OUT(bit);
			IIC_SLAVE_SDA_DIR_IN();
        break;
        
        default :
        break;
    }

    return byte;
}

//�ӻ�����һ�ֽ�
static uint8_t IIC_Slave_ReceiveByte(uint8_t type)
{
    uint8_t byte = 0;  //�����ֽڻ���
    
    switch(type)
    {
        case 1 :
			static uint8_t i = 0;  //����bitλ����¼
			
            if(i++ >= 7)  //����7��bit�󷵻�һ���ֽ�
            {
                byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
				i = 0;
            }
        break;
        
        default :
        break;
    }

    return byte;
}

//�ӻ�����ָ����������
static uint8_t IIC_Slave_Read(uint8_t type, uint8_t * data, uint8_t len)
{
    uint8_t i;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < len; i++)
			{
				*(data + i) = IIC_Slave_ReceiveByte(type);
				if(i < len - 1)
					IIC_Slave_SendAck(type, 0);  //����һ���ֽڣ�����һ����ЧӦ���ź�
			}
			IIC_Slave_SendAck(type, 1);  //���������ݣ�����һ����Ӧ���ź�
        break;
        
        default :
        break;
    }

    return byte;
}

//
void IIC_Slave_Deal(uint8_t type)
{
    static uint8_t state = 0;
    
    
    if((!state) && (!IIC_Slave_Start(type)))
        state = 1;  //�յ���ʼ�źţ�������
    
    if(state)
    {
        if(IIC_Slave_ReceiveByte(type) == SlaveAddress)
            ;
    }
}

//STM8 PB�ڽ����жϺ�����SDA��SCL����
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
    
}
