#include "bsp_i2c_simulation_master.h"

//模拟I2C口列表初始化
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

static void I2C_Simulation_Master_Start(uint8_t port);  //起始信号
static void I2C_Simulation_Master_Stop(uint8_t port);  //停止信号

static void I2C_Simulation_Master_SendAck(uint8_t port, uint8_t ack);  //发送应答信号
static uint8_t I2C_Simulation_Master_ReceiveAck(uint8_t port);  //接收应答信号

static uint8_t I2C_Simulation_Master_SendByte(uint8_t port, uint8_t byte);  //发送一个字节，返回应答信号
static uint8_t I2C_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack);  //接收一个字节，发送应答信号

#if defined STM32_STANDARD
//模拟I2C IO口初始化
static void i2c_simulation_gpio_init(I2C_Simulation_GPIO_TypeDef * pI2C_GPIO)
{
    if( (!pI2C_GPIO->GPIO_scl) || (!pI2C_GPIO->GPIO_sda) )
        return;  //通道没有就返回
    
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

//模拟I2C IO口电平输出，0输出低
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//模拟I2C IO口输入/出方向，0输入
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//模拟I2C IO口电平读取，返回电平值
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}

#elif defined STM8
//模拟I2C IO口初始化
static void i2c_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny)
{
	GPIO_Init(GPIOx, PortPinx, GPIO_MODE_OUT_PP_HIGH_SLOW);  //开漏低速输出
	GPIO_Init(GPIOy, PortPiny, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

//模拟I2C IO口电平输出，0输出低
static void i2c_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//模拟I2C IO口输入/出方向，0输入
static void i2c_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//模拟I2C IO口电平读取，返回电平值
static uint8_t i2c_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}
#endif

//模拟I2C初始化，为空闲状态，scl与sda都为高，scl高电平收发稳定sda电平，低电平才能改sda电平
void I2C_Simulation_Master_Init(void)
{
	uint8_t i;  //I2C口索引
	for(i = 0; i < number_of_i2c_simulation; i++)
		i2c_simulation_gpio_init(I2C_GPIO_List[i].GPIO_scl, I2C_GPIO_List[i].scl_pin, I2C_GPIO_List[i].GPIO_sda, I2C_GPIO_List[i].sda_pin);
}

//起始信号，当SCL处于高电平时，SDA由高电平变成低电平时
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

//停止信号，当SCL处于高电平时，SDA由低电平变成高电平
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

//发送应答信号，0:ACK，1:NAK，scl高电平收发稳定sda电平，低电平才能改sda电平
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

//接收应答信号，0:ACK，1:NAK
static uint8_t I2C_Simulation_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
	if(port < number_of_i2c_simulation)
	{
		i2c_simulation_gpio_write(I2C_GPIO_List[port].GPIO_sda, I2C_GPIO_List[port].sda_pin, GPIO_High_Level);  //释放数据线，准备读取数据
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

//发送一字节，返回应答信号
static uint8_t I2C_Simulation_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
	if(port < number_of_i2c_simulation)
	{
		for(i = 0; i < 8; i++)
		{
			bit = !!(byte & (0x80 >> i));  //取逻辑值0或1，否则P0_0=bit时，只读取bit数据的最低位bit0的值
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

//接收一字节，并发送应答
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
	
	I2C_Simulation_Master_SendAck(port, ack);  //接收完一个字节，发送应答
	
    return byte;
}

//写设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度、失败重发次数；成功返回0，失败则重发，都失败返回1
uint8_t I2C_Simulation_Master_Write(pI2C_Master_WRInfo_TypeDef pi2c)
{
    uint8_t i;  //发送数据字节数索引
	uint8_t error_flag = 0;  //发送错误标志
    uint8_t resend_times = 1;  //重发次数缓存
    
    resend_times = pi2c->error_resend_times;
    
	while(resend_times--)
	{
		I2C_Simulation_Master_Start(pi2c->port);
        
        if(pi2c->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            //设备高地址 + 写信号，bit0=0为写，bit0=1为读
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)((pi2c->device_adr >> 8) & 0xFE)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动I2C和发送
            }
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(pi2c->device_adr & 0xFF)))  //设备低地址
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
			if(I2C_Simulation_Master_SendByte(pi2c->port, *(pi2c->data++)))  //数据
			{
				I2C_Simulation_Master_Stop(pi2c->port);
				error_flag = 1;  //置1发送错误标志
				break;  //发送出错就停止，跳当前for循环
			}
		}
		
		if(!error_flag)
		{
			I2C_Simulation_Master_Stop(pi2c->port);
			return 0;  //若没有出错，停止后结束函数，返回0
		}
		
		error_flag = 0;  //清0发送错误标志
	}
    
    return 1;  //发送几次都错误，返回1
}

//读设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度、失败重发次数；成功返回0，失败则重发，都失败返回1
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
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(pi2c->device_adr & 0xFF)))  //注意运算符优先级+高于&
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
            
            //重新开始再发高位加读信号
            I2C_Simulation_Master_Start(pi2c->port);
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)(((pi2c->device_adr >> 8) & 0xFE) + 1)))
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
        else
        {
            if(I2C_Simulation_Master_SendByte(pi2c->port, (uint8_t)((pi2c->device_adr & 0xFE) + 1)))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
            {
                I2C_Simulation_Master_Stop(pi2c->port);
                continue;
            }
        }
		for(i = 0; i < pi2c->len; i++)  //接收指定长度数据
		{
			if(i < pi2c->len - 1)
				*(pi2c->data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->port, 0);  //数据没接收完，发送有效应答
			else
				*(pi2c->data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->port, 1);  //数据接收完，发送非应答
		}
		
		I2C_Simulation_Master_Stop(pi2c->port);
		
		return 0;
	}
	
	return 1;
}

//读设备寄存器，结构体输入端口、设备地址及其10位地址模式标志、寄存器地址和其长度、是否延时及延时时间、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t I2C_Simulation_Master_ReadRegister(pI2C_Master_ReadReg_Info_TypeDef pi2c)
{
	uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = pi2c->wr_info.error_resend_times;
    
	while(resend_times--)
	{
		//先写寄存器地址
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
		if(pi2c->reg_adr_twobyte_flag)  //从机2字节寄存器地址模式判断
		{
			if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->register_adr >> 8)))  //寄存器高地址
			{
				I2C_Simulation_Master_Stop(pi2c->wr_info.port);
				continue;
			}
            if(I2C_Simulation_Master_SendByte(pi2c->wr_info.port, (uint8_t)(pi2c->register_adr & 0xFF)))  //寄存器低地址
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
			delay_ms(pi2c->readreg_delay_nms);  //延时等待数据准备完成
		//再读数据
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
			delay_ms(pi2c->readreg_delay_nms);  //延时等待数据准备完成
		for(i = 0; i < pi2c->wr_info.len; i++)  //接收指定长度数据
		{
			if(i < pi2c->wr_info.len - 1)
				*(pi2c->wr_info.data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->wr_info.port, 0);  //数据没接收完，发送有效应答
			else
				*(pi2c->wr_info.data + i) = I2C_Simulation_Master_ReceiveByte(pi2c->wr_info.port, 1);  //数据接收完，发送非应答
		}
		I2C_Simulation_Master_Stop(pi2c->wr_info.port);
		
		return 0;
	}
    
    return 1;
}
