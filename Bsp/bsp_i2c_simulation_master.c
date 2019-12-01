#include "bsp_i2c_simulation_master.h"

//ģ��I2C���б��ʼ��
I2C_Simulation_GPIO_TypeDef I2C_GPIO_List[number_of_i2c_simulation] = I2C_GPIO_LIST;

#if defined STM32_STANDARD
static void i2c_simulation_gpio_init(I2C_Simulation_GPIO_TypeDef * pI2C_GPIO);
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_OutLevel_TypeDef level);
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_Direction_TypeDef dir);
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#elif defined STM8
static void i2c_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny);
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_OutLevel_TypeDef level);
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir);
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx);
#endif

static void I2C_Simulation_Master_Start(uint8_t port);  //��ʼ�ź�
static void I2C_Simulation_Master_Stop(uint8_t port);  //ֹͣ�ź�

static void I2C_Simulation_Master_SendAck(uint8_t port, uint8_t ack);  //����Ӧ���ź�
static uint8_t I2C_Simulation_Master_ReceiveAck(uint8_t port);  //����Ӧ���ź�

static uint8_t I2C_Simulation_Master_SendByte(uint8_t port, uint8_t byte);  //����һ���ֽڣ�����Ӧ���ź�
static uint8_t I2C_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack);  //����һ���ֽڣ�����Ӧ���ź�

#if defined STM32_STANDARD
//ģ��I2C IO�ڳ�ʼ��
static void i2c_simulation_gpio_init(I2C_Simulation_GPIO_TypeDef * pI2C_GPIO)
{
    if( (!pI2C_GPIO->GPIO_scl) || (!pI2C_GPIO->GPIO_sda) )
        return;  //ͨ��û�оͷ���
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(pI2C_GPIO->RCC_APB2Periph_scl, ENABLE);
    RCC_APB2PeriphClockCmd(pI2C_GPIO->RCC_APB2Periph_sda, ENABLE);
    
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    
    GPIO_InitStruct.GPIO_Pin = pI2C_GPIO->Pin_scl;
    GPIO_Init(pI2C_GPIO->GPIO_scl, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = pI2C_GPIO->Pin_sda;
    GPIO_Init(pI2C_GPIO->GPIO_sda, &GPIO_InitStruct);
}

//ģ��I2C IO�ڵ�ƽ�����0�����
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//ģ��I2C IO������/������0����
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//ģ��I2C IO�ڵ�ƽ��ȡ�����ص�ƽֵ
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}

#elif defined STM8
//ģ��I2C IO�ڳ�ʼ��
static void i2c_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny)
{
	GPIO_Init(GPIOx, PortPinx, GPIO_MODE_OUT_PP_HIGH_SLOW);  //��©�������
	GPIO_Init(GPIOy, PortPiny, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

//ģ��I2C IO�ڵ�ƽ�����0�����
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//ģ��I2C IO������/������0����
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//ģ��I2C IO�ڵ�ƽ��ȡ�����ص�ƽֵ
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}
#endif

//ģ��I2C��ʼ����Ϊ����״̬��scl��sda��Ϊ�ߣ�scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
void I2C_Simulation_Master_Init(void)
{
	uint8_t i;  //I2C������
	for(i = 0; i < number_of_i2c_simulation; i++)
		i2c_simulation_gpio_init(I2C_GPIO_List[i].GPIO_scl, I2C_GPIO_List[i].scl_pin, I2C_GPIO_List[i].GPIO_sda, I2C_GPIO_List[i].sda_pin);
}

//��ʼ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɸߵ�ƽ��ɵ͵�ƽʱ
static void I2C_Simulation_Master_Start(uint8_t port)
{
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_High_Level);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_Low_Level);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_I2C);
	}
}

//ֹͣ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɵ͵�ƽ��ɸߵ�ƽ
static void I2C_Simulation_Master_Stop(uint8_t port)
{
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_Low_Level);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_High_Level);
		delay_us(DELAY_US_I2C);
	}
}

//����Ӧ���źţ�0:ACK��1:NAK��scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
static void I2C_Simulation_Master_SendAck(uint8_t port, uint8_t ack)
{
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, ack);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_I2C);
	}
}

//����Ӧ���źţ�0:ACK��1:NAK
static uint8_t I2C_Simulation_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_High_Level);  //�ͷ������ߣ�׼����ȡ����
		i2c_simulation_gpio_direction(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_In);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
		delay_us(DELAY_US_I2C);
		ack = !!i2c_simulation_gpio_read(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin);
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_Low_Level);
		delay_us(DELAY_US_I2C);
		i2c_simulation_gpio_direction(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_Out);
	}
	
    return ack;
}

//����һ�ֽڣ�����Ӧ���ź�
static uint8_t I2C_Simulation_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
	if(port < number_of_i2c_simulation)
	{
		for(i = 0; i < 8; i++)
		{
			bit = !!(byte & (0x80 >> i));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
			i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, bit);
			delay_us(DELAY_US_I2C);
			i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
			delay_us(DELAY_US_I2C);
			i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_Low_Level);
			delay_us(DELAY_US_I2C);
		}
	}
	
    return I2C_Simulation_Master_ReceiveAck(port);
}

//����һ�ֽڣ�������Ӧ��
static uint8_t I2C_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack)
{
    uint8_t i, byte = 0;
    
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_High_Level);
		i2c_simulation_gpio_direction(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_In);
		for(i = 0; i < 8; i++)
		{
			i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_High_Level);
			delay_us(DELAY_US_I2C);
			byte |= ((!!i2c_simulation_gpio_read(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin)) << (7 - i));
			i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_scl, I2C_GPIO_List[port].scl_pin, GPIO_Low_Level);
			delay_us(DELAY_US_I2C);
		}
		i2c_simulation_gpio_direction(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_Out);
	}
	
	I2C_Simulation_Master_SendAck(port, ack);  //������һ���ֽڣ�����Ӧ��
	
    return byte;
}

//д�豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȡ�ʧ���ط��������ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t I2C_Simulation_Master_Write(pI2C_Master_WRInfo_TypeDef pi2c)
{
    uint8_t i;  //���������ֽ�������
	uint8_t error_flag = 0;  //���ʹ����־
    uint8_t resend_times = 1;  //�ط���������
    
    resend_times = pi2c->error_resend_times;
    
	while(resend_times--)
	{
		I2C_Simulation_Master_Start(pi2c->port);
        
        if(pi2c->dev_adr_tenbit_flag)  //�ӻ�Ϊ10λ��ַģʽ�ж�
        {
            //�豸�ߵ�ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)((pi2c->device_adr >> 8) & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;  //���ͳ����ֹͣ����������ѭ������䣬ֱ�ӵ�whileѭ�������жϣ���������I2C�ͷ���
            }
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(pi2c->device_adr & 0xFF)))  //�豸�͵�ַ
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(pi2c->device_adr & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
		for(i = 0; i < pi2c->len; i++)
		{
			if(I2C_Simulation_Master_SendByte(pi2c->port, *(pi2c->data++)))  //����
			{
				I2C_Simulation_Master_Stop(pi2c->port);
				error_flag = 1;  //��1���ʹ����־
				break;  //���ͳ����ֹͣ������ǰforѭ��
			}
		}
		
		if(!error_flag)
		{
			I2C_Simulation_Master_Stop(pi2c->port);
			return 0;  //��û�г���ֹͣ���������������0
		}
		
		error_flag = 0;  //��0���ʹ����־
	}
    
    return 1;  //���ͼ��ζ����󣬷���1
}

//���豸���ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־�����ݺ��䳤�ȡ�ʧ���ط��������ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t I2C_Simulation_Master_ReadDirect(pI2C_Master_WRInfo_TypeDef pi2c)
{
    uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = pi2c->error_resend_times;
    
	while(resend_times--)
	{
		I2C_Simulation_Master_Start(pi2c->port);
		
        if(pi2c->dev_adr_tenbit_flag)
        {
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)((pi2c->device_adr >> 8) & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(pi2c->device_adr & 0xFF)))  //ע����������ȼ�+����&
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
            
            //���¿�ʼ�ٷ���λ�Ӷ��ź�
            I2C_Simulation_Master_Start(pi2c->port);
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(((pi2c->device_adr >> 8) & 0xFE) + 1)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)((pi2c->device_adr & 0xFE) + 1)))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
		for(i = 0; i < pi2c->len; i++)  //����ָ����������
		{
			if(i < pi2c->len - 1)
				*(pi2c->data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->port, 0);  //����û�����꣬������ЧӦ��
			else
				*(pi2c->data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		
		I2C_Simulation_Master_Stop(pi2c->port);
		
		return 0;
	}
	
	return 1;
}

//���豸�Ĵ������ṹ������˿ڡ��豸��ַ����10λ��ַģʽ��־���Ĵ�����ַ���䳤�ȡ��Ƿ���ʱ����ʱʱ�䡢���ݺ��䳤�ȣ��ɹ�����0��ʧ�����ط�����ʧ�ܷ���1
uint8_t I2C_Simulation_Master_ReadRegister(pI2C_Master_ReadReg_Info_TypeDef pi2c)
{
	uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = pi2c->wr_info.error_resend_times;
    
	while(resend_times--)
	{
		//��д�Ĵ�����ַ
		I2C_Simulation_Master_Start(pi2c->wr_info.port);
        if(pi2c->wr_info.dev_adr_tenbit_flag)
        {
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)((pi2c->wr_info.device_adr >> 8) & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->wr_info.device_adr & 0xFF)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
        }
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->wr_info.device_adr & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
        }
		if(pi2c->reg_adr_twobyte_flag)  //�ӻ�2�ֽڼĴ�����ַģʽ�ж�
		{
			if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->register_adr >> 8)))  //�Ĵ����ߵ�ַ
			{
				I2C_Simulation_Master_Stop(pi2c->wr_info.port);
				continue;
			}
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->register_adr & 0xFF)))  //�Ĵ����͵�ַ
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
		}
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->register_adr & 0xFF)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
        }
		
		if(pi2c->readreg_delay_flag == Delay_Front)
			delay_ms(pi2c->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		//�ٶ�����
		I2C_Simulation_Master_Start(pi2c->wr_info.port);
        if(pi2c->wr_info.dev_adr_tenbit_flag)
        {
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)((pi2c->wr_info.device_adr >> 8) & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->wr_info.device_adr & 0xFF)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
            
            I2C_Simulation_Master_Start(pi2c->wr_info.port);
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(((pi2c->wr_info.device_adr >> 8) & 0xFE) + 1)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
        }
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)((pi2c->wr_info.device_adr & 0xFE) + 1)))
            {
                I2C_Simulation_Master_Stop(pi2c->wr_info.port);
                continue;
            }
        }
		if(pi2c->readreg_delay_flag == Delay_Back)
			delay_ms(pi2c->readreg_delay_nms);  //��ʱ�ȴ�����׼�����
		for(i = 0; i < pi2c->wr_info.len; i++)  //����ָ����������
		{
			if(i < pi2c->wr_info.len - 1)
				*(pi2c->wr_info.data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->wr_info.port, 0);  //����û�����꣬������ЧӦ��
			else
				*(pi2c->wr_info.data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->wr_info.port, 1);  //���ݽ����꣬���ͷ�Ӧ��
		}
		I2C_Simulation_Master_Stop(pi2c->wr_info.port);
		
		return 0;
	}
    
    return 1;
}
