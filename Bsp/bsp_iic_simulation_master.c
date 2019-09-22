#include "bsp_iic_simulation_master.h"

//ģ��IIC���б��ʼ��
IIC_Simulation_GPIO_TypeDef IIC_GPIO_List[IIC_SIMULATION_MASTER_NUM] = IIC_GPIO_LIST;

//�ڲ�����
//__weak static void delay_us(uint16_t nus);  //��ʱn us���ڲ�ʹ��
//__weak static void delay_ms(uint16_t nms);  //��ʱn ms

static void iic_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny);
static void iic_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level);
static void iic_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir);
static uint8_t iic_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx);

static void IIC_Simulation_Master_Start(uint8_t port);  //��ʼ�ź�
static void IIC_Simulation_Master_Stop(uint8_t port);  //ֹͣ�ź�

static void IIC_Simulation_Master_SendAck(uint8_t port, uint8_t ack);  //����Ӧ���ź�
static uint8_t IIC_Simulation_Master_ReceiveAck(uint8_t port);  //����Ӧ���ź�

static uint8_t IIC_Simulation_Master_SendByte(uint8_t port, uint8_t byte);  //����һ���ֽ�
static uint8_t IIC_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack);  //����һ���ֽ�

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

//ģ��IIC IO�ڳ�ʼ��
static void iic_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny)
{
	GPIO_Init(GPIOx, PortPinx, GPIO_MODE_OUT_OD_HIZ_SLOW);  //��©�������
	GPIO_Init(GPIOy, PortPiny, GPIO_MODE_OUT_OD_HIZ_SLOW);
}

//ģ��IIC IO�ڵ�ƽ�����0�����
static void iic_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//ģ��IIC IO������/������0����
static void iic_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//ģ��IIC IO�ڵ�ƽ��ȡ�����ص�ƽֵ
static uint8_t iic_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}

//ģ��IIC��ʼ����Ϊ����״̬��scl��sda��Ϊ�ߣ�scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
void IIC_Simulation_Master_Init(void)
{
	uint8_t i;  //IIC������
	for(i = 0; i < IIC_SIMULATION_MASTER_NUM; i++)
		iic_simulation_gpio_init(IIC_GPIO_List[i].scl_port, IIC_GPIO_List[i].scl_pin, IIC_GPIO_List[i].sda_port, IIC_GPIO_List[i].sda_pin);
}

//��ʼ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɸߵ�ƽ��ɵ͵�ƽʱ
static void IIC_Simulation_Master_Start(uint8_t port)
{
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_High_Level);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_Low_Level);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_IIC);
	}
}

//ֹͣ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɵ͵�ƽ��ɸߵ�ƽ
static void IIC_Simulation_Master_Stop(uint8_t port)
{
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_Low_Level);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_High_Level);
		delay_us(DELAY_US_IIC);
	}
}

//����Ӧ���źţ�0:ACK��1:NAK��scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
static void IIC_Simulation_Master_SendAck(uint8_t port, uint8_t ack)
{
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, ack);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_IIC);
	}
}

//����Ӧ���źţ�0:ACK��1:NAK
static uint8_t IIC_Simulation_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_High_Level);  //�ͷ������ߣ�׼����ȡ����
		iic_simulation_gpio_direction(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_In);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_IIC);
		ack = !!iic_simulation_gpio_read(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin);
		iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_IIC);
		iic_simulation_gpio_direction(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_Out);
	}
	
    return ack;
}

//����һ�ֽڣ�����Ӧ���ź�
static uint8_t IIC_Simulation_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		for(i = 0; i < 8; i++)
		{
			bit = !!(byte & (0x80 >> i));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
			iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, bit);
			delay_us(DELAY_US_IIC);
			iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
			delay_us(DELAY_US_IIC);
			iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_Low_Level);
			delay_us(DELAY_US_IIC);
		}
	}
	
    return IIC_Simulation_Master_ReceiveAck(port);
}

//����һ�ֽڣ�������Ӧ��
static uint8_t IIC_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack)
{
    uint8_t i, byte = 0;
    
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_High_Level);
		iic_simulation_gpio_direction(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_In);
		for(i = 0; i < 8; i++)
		{
			iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_High_Level);
			delay_us(DELAY_US_IIC);
			byte |= ((!!iic_simulation_gpio_read(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin)) << (7 - i));
			iic_simulation_gpio_write(IIC_GPIO_List[port].scl_port, IIC_GPIO_List[port].scl_pin, GPIO_Low_Level);
			delay_us(DELAY_US_IIC);
		}
		iic_simulation_gpio_direction(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_Out);
	}
	
	IIC_Simulation_Master_SendAck(port, ack);  //������һ���ֽڣ�����Ӧ��
	
    return byte;
}

//д�豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȡ�ʧ���ط��������ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Simulation_Master_Write(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //���������ֽ�������
	uint8_t error_flag = 0;  //���ʹ����־
    uint8_t resend_times = 1;  //�ط���������
    
    resend_times = piic->error_resend_times;
    
	while(resend_times--)
	{
		IIC_Simulation_Master_Start(piic->port);
        
        if(piic->dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //�豸�ߵ�ַ
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������IIC�ͷ���
            }
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //�豸�͵�ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
		for(i = 0; i < piic->len; i++)
		{
			if(IIC_Simulation_Master_SendByte(piic->port, *(piic->data++)))  //����
			{
				IIC_Simulation_Master_Stop(piic->port);
				error_flag = 1;  //��1���ʹ����־
				break;  //���ͳ����ֹͣ������ǰforѭ��
			}
		}
		
		if(!error_flag)
		{
			IIC_Simulation_Master_Stop(piic->port);
			return 0;  //��û�г���ֹͣ���������������0
		}
		
		error_flag = 0;  //��0���ʹ����־
	}
    
    return 1;  //���ͼ��ζ����󣬷���1
}

//���豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȡ�ʧ���ط��������ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Simulation_Master_ReadDirect(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = piic->error_resend_times;
    
	while(resend_times--)
	{
		IIC_Simulation_Master_Start(piic->port);
		
        if(piic->dev_adr_tenbit_flag)
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //ע����������ȼ�+����&
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
		for(i = 0; i < piic->len; i++)  //����ָ����������
		{
			if(i < piic->len - 1)
				*(piic->data + i) = IIC_Simulation_Master_ReceiveByte(piic->port, 0);  //����û�����꣬������ЧӦ��
			else
				*(piic->data + i) = IIC_Simulation_Master_ReceiveByte(piic->port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		
		IIC_Simulation_Master_Stop(piic->port);
		
		return 0;
	}
	
	return 1;
}

//���豸�Ĵ������ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־���Ĵ�����ַ���䳤�ȡ��Ƿ���ʱ����ʱʱ�䡢���ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t IIC_Simulation_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic)
{
	uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = piic->wr_info.error_resend_times;
    
	while(resend_times--)
	{
		//��д�Ĵ�����ַ
		IIC_Simulation_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)
        {
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
        }
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr & 0xFE)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
        }
		if(piic->reg_adr_twobyte_flag)  //�ӻ�2�ֽڼĴ�����ַģʽ�ж�
		{
			if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr >> 8)))  //�Ĵ����ߵ�ַ
			{
				IIC_Simulation_Master_Stop(piic->wr_info.port);
				continue;
			}
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //�Ĵ����͵�ַ
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
		}
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
        }
		
		if(piic->readreg_delay_flag == Delay_Front)
			delay_ms(piic->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		//�ٶ�����
		IIC_Simulation_Master_Start(piic->wr_info.port);
        if(piic->wr_info.dev_adr_tenbit_flag)
        {
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->wr_info.device_adr >> 8)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
        }
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)((piic->wr_info.device_adr & 0xFE) + 1)))
            {
                IIC_Simulation_Master_Stop(piic->wr_info.port);
                continue;
            }
        }
		if(piic->readreg_delay_flag == Delay_Back)
			delay_ms(piic->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		for(i = 0; i < piic->wr_info.len; i++)  //����ָ����������
		{
			if(i < piic->wr_info.len - 1)
				*(piic->wr_info.data + i) = IIC_Simulation_Master_ReceiveByte(piic->wr_info.port, 0);  //����û�����꣬������ЧӦ��
			else
				*(piic->wr_info.data + i) = IIC_Simulation_Master_ReceiveByte(piic->wr_info.port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		IIC_Simulation_Master_Stop(piic->wr_info.port);
		
		return 0;
	}
    
    return 1;
}
