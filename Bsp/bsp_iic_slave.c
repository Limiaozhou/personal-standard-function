#include "bsp_iic_master.h"
#include "bsp_iic_slave.h"

//��ʼ����SCL���жϣ�SDA���½����жϣ��Լ�⿪ʼ�ź�
void IIC_Slave_Init(unsigned char type)
{
    switch(type)
    {
        case 1 :
            IIC_SLAVE_SDA_DIR_IN();  //SDA����
            IIC_SLAVE_SCL_DIR_IN();
            IIC_SLAVE_SDA_UP();  //SDA�������룬�������
            IIC_SLAVE_SCL_UP();  //SCL�������룬�������
            IIC_SLAVE_SDA_EXTI_DOWN();  //SDA�½��ش���
            IIC_SLAVE_SCL_EXTI_DOWN();  //SCL�½��ش���
            IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
            IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
        break;
        
        default :
        break;
    }
}

//�ӻ������ʼ�źţ�SDA�½��أ�SCL���ڸߵ�ƽʱ
static unsigned char IIC_Slave_Start(unsigned char type)
{
    switch(type)
    {
        case 1 :
            if(IIC_SLAVE_SCL_PIN_IN)  //SCL���ڸߵ�ƽ���ɹ�
            {
                IIC_SLAVE_SDA_IQ_OFF();  //SDA���ж�
                IIC_SLAVE_SCL_EXTI_UP();  //����SCL�����ش���
                IIC_SLAVE_SCL_IQ_ON();  //SCL���ж�
                return 0;  //����0
            }
        break;
        
        default :
        break;
    }
    
    return 1;  //ʧ�ܷ���1
}

//�ӻ����ֹͣ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɵ͵�ƽ��ɸߵ�ƽ
//static unsigned char IIC_Slave_Stop(unsigned char type)
//{
//    switch(type)
//    {
//        case 1 :
//            if(IIC_SLAVE_SDA_PIN_IN && IIC_SLAVE_SCL_PIN_IN)
//            {
//                IIC_SLAVE_SDA_DIR_IN();  //SDA����
//                IIC_SLAVE_SDA_EXTI_UPDOWN();  //SDA�����غ��½��ش���
//                IIC_SLAVE_SCL_EXTI_UPDOWN();  //SCL�����غ��½��ش���
//                IIC_SLAVE_SDA_IQ_ON();  //SDA�ж����뿪���������
//                IIC_SLAVE_SCL_IQ_OFF();  //SCL�ж�����أ��������
//                return 0;  //�ɹ�����0
//            }
//        break;
//        
//        default :
//        break;
//    }
//    
//    return 1;  //ʧ�ܷ���1
//}

//�ӻ�����Ӧ���źţ�0:ACK��1:NAK
//static void IIC_Slave_SendAck(unsigned char type, unsigned char ack)
//{
//    switch(type)
//    {
//        case 1 :
//            IIC_SLAVE_SDA_DIR_OUT();
//            IIC_SLAVE_SDA_OUT(ack);
//            IIC_SLAVE_SDA_DIR_IN();
//        break;
//        
//        default :
//        break;
//    }
//}

//�ӻ�����Ӧ���źţ�0:ACK��1:NAK
//static unsigned char IIC_Slave_ReceiveAck(unsigned char type)
//{
//    unsigned char ack = 1;
//    
//    switch(type)
//    {
//        case 1 :
//            ack = IIC_SLAVE_SDA_PIN_IN;
//        break;
//        
//        default :
//        break;
//    }
//    
//    return ack;
//}

//�ӻ�����һ�ֽ�
static unsigned char IIC_Slave_ReceiveByte(unsigned char type)
{
    unsigned char i, byte = 0;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < 8; i++)
            {
                byte |= IIC_SLAVE_SDA_PIN_IN << (7 - i);
            }
        break;
        
        default :
        break;
    }

    return byte;
}

//
void IIC_Slave_Deal(unsigned char type)
{
    static unsigned char state = 0;
    
    
    if((!state) && (!IIC_Slave_Start(type)))
        state = 1;  //�յ���ʼ�źţ�������
    
    if(state)
    {
        if(IIC_Slave_ReceiveByte(type) == SlaveAddress)
            ;
    }
}

//STM8 PB�ڽ����жϺ���
#pragma vector=EXTI1_vector  //6
__interrupt void EXTI1_IRQHandler(void)
{
    
}
