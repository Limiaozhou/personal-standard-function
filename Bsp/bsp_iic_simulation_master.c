#include "bsp_iic_simulation_master.h"

//模拟IIC口列表初始化
IIC_Simulation_GPIO_TypeDef IIC_GPIO_List[IIC_SIMULATION_MASTER_NUM] = IIC_GPIO_LIST;

//内部函数
//__weak static void delay_us(uint16_t nus);  //延时n us，内部使用
//__weak static void delay_ms(uint16_t nms);  //延时n ms

static void iic_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny);
static void iic_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level);
static void iic_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir);
static uint8_t iic_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx);

static void IIC_Simulation_Master_Start(uint8_t port);  //起始信号
static void IIC_Simulation_Master_Stop(uint8_t port);  //停止信号

static void IIC_Simulation_Master_SendAck(uint8_t port, uint8_t ack);  //发送应答信号
static uint8_t IIC_Simulation_Master_ReceiveAck(uint8_t port);  //接收应答信号

static uint8_t IIC_Simulation_Master_SendByte(uint8_t port, uint8_t byte);  //发送一个字节
static uint8_t IIC_Simulation_Master_ReceiveByte(uint8_t port, uint8_t ack);  //接收一个字节

//延时n us，内部使用
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
////延时n ms
//__weak static void delay_ms(uint16_t nms)
//{
//	while (nms--)
//		delay_us(1000);
//}

//模拟IIC IO口初始化
static void iic_simulation_gpio_init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_TypeDef* GPIOy, GPIO_Pin_TypeDef PortPiny)
{
	GPIO_Init(GPIOx, PortPinx, GPIO_MODE_OUT_OD_HIZ_SLOW);  //开漏低速输出
	GPIO_Init(GPIOy, PortPiny, GPIO_MODE_OUT_OD_HIZ_SLOW);
}

//模拟IIC IO口电平输出，0输出低
static void iic_simulation_gpio_write(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, uint8_t level)
{
	if(level)
		GPIO_WriteHigh(GPIOx, PortPinx);
	else
		GPIO_WriteLow(GPIOx, PortPinx);
}

//模拟IIC IO口输入/出方向，0输入
static void iic_simulation_gpio_direction(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx, GPIO_Direction_TypeDef dir)
{
	if(dir == GPIO_Out)
		GPIOx->DDR |= (uint8_t)PortPinx;
	else
		GPIOx->DDR &= (uint8_t)(~PortPinx);
}

//模拟IIC IO口电平读取，返回电平值
static uint8_t iic_simulation_gpio_read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPinx)
{
	return ((uint8_t)GPIO_ReadInputPin(GPIOx, PortPinx));
}

//模拟IIC初始化，为空闲状态，scl与sda都为高，scl高电平收发稳定sda电平，低电平才能改sda电平
void IIC_Simulation_Master_Init(void)
{
	uint8_t i;  //IIC口索引
	for(i = 0; i < IIC_SIMULATION_MASTER_NUM; i++)
		iic_simulation_gpio_init(IIC_GPIO_List[i].scl_port, IIC_GPIO_List[i].scl_pin, IIC_GPIO_List[i].sda_port, IIC_GPIO_List[i].sda_pin);
}

//起始信号，当SCL处于高电平时，SDA由高电平变成低电平时
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

//停止信号，当SCL处于高电平时，SDA由低电平变成高电平
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

//发送应答信号，0:ACK，1:NAK，scl高电平收发稳定sda电平，低电平才能改sda电平
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

//接收应答信号，0:ACK，1:NAK
static uint8_t IIC_Simulation_Master_ReceiveAck(uint8_t port)
{
    uint8_t ack = 1;
    
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		iic_simulation_gpio_write(IIC_GPIO_List[port].sda_port, IIC_GPIO_List[port].sda_pin, GPIO_High_Level);  //释放数据线，准备读取数据
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

//发送一字节，返回应答信号
static uint8_t IIC_Simulation_Master_SendByte(uint8_t port, uint8_t byte)
{
    uint8_t i, bit = 1;
    
	if(port < IIC_SIMULATION_MASTER_NUM)
	{
		for(i = 0; i < 8; i++)
		{
			bit = !!(byte & (0x80 >> i));  //取逻辑值0或1，否则P0_0=bit时，只读取bit数据的最低位bit0的值
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

//接收一字节，并发送应答
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
	
	IIC_Simulation_Master_SendAck(port, ack);  //接收完一个字节，发送应答
	
    return byte;
}

//写设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度、失败重发次数；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Simulation_Master_Write(pIIC_Master_WRInfo_TypeDef piic)
{
    uint8_t i;  //发送数据字节数索引
	uint8_t error_flag = 0;  //发送错误标志
    uint8_t resend_times = 1;  //重发次数缓存
    
    resend_times = piic->error_resend_times;
    
	while(resend_times--)
	{
		IIC_Simulation_Master_Start(piic->port);
        
        if(piic->dev_adr_tenbit_flag)  //从机为10位地址模式判断
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr >> 8)))  //设备高地址
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;  //发送出错就停止，跳过后面循环体语句，直接到while循环条件判断，重新启动IIC和发送
            }
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)(piic->device_adr & 0xFE)))  //设备低地址 + 写信号，bit0=0为写，bit0=1为读
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
			if(IIC_Simulation_Master_SendByte(piic->port, *(piic->data++)))  //数据
			{
				IIC_Simulation_Master_Stop(piic->port);
				error_flag = 1;  //置1发送错误标志
				break;  //发送出错就停止，跳当前for循环
			}
		}
		
		if(!error_flag)
		{
			IIC_Simulation_Master_Stop(piic->port);
			return 0;  //若没有出错，停止后结束函数，返回0
		}
		
		error_flag = 0;  //清0发送错误标志
	}
    
    return 1;  //发送几次都错误，返回1
}

//读设备，结构体输入端口、设备地址及其10位地址模式标志、数据和其长度、失败重发次数；成功返回0，失败则重发，都失败返回1
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
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //注意运算符优先级+高于&
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
        else
        {
            if(IIC_Simulation_Master_SendByte(piic->port, (uint8_t)((piic->device_adr & 0xFE) + 1)))  //设备地址 + 读信号，bit0=0为写，bit0=1为读
            {
                IIC_Simulation_Master_Stop(piic->port);
                continue;
            }
        }
		for(i = 0; i < piic->len; i++)  //接收指定长度数据
		{
			if(i < piic->len - 1)
				*(piic->data + i) = IIC_Simulation_Master_ReceiveByte(piic->port, 0);  //数据没接收完，发送有效应答
			else
				*(piic->data + i) = IIC_Simulation_Master_ReceiveByte(piic->port, 1);  //数据接收完，发送非应答
		}
		
		IIC_Simulation_Master_Stop(piic->port);
		
		return 0;
	}
	
	return 1;
}

//读设备寄存器，结构体输入端口、设备地址及其10位地址模式标志、寄存器地址和其长度、是否延时及延时时间、数据和其长度；成功返回0，失败则重发，都失败返回1
uint8_t IIC_Simulation_Master_ReadRegister(pIIC_Master_ReadReg_Info_TypeDef piic)
{
	uint8_t i;
    uint8_t resend_times = 1;
    
    resend_times = piic->wr_info.error_resend_times;
    
	while(resend_times--)
	{
		//先写寄存器地址
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
		if(piic->reg_adr_twobyte_flag)  //从机2字节寄存器地址模式判断
		{
			if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr >> 8)))  //寄存器高地址
			{
				IIC_Simulation_Master_Stop(piic->wr_info.port);
				continue;
			}
            if(IIC_Simulation_Master_SendByte(piic->wr_info.port, (uint8_t)(piic->register_adr & 0xFF)))  //寄存器低地址
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
			delay_ms(piic->readreg_delay_nms);  //延时等待数据准备完成
		//再读数据
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
			delay_ms(piic->readreg_delay_nms);  //延时等待数据准备完成
		for(i = 0; i < piic->wr_info.len; i++)  //接收指定长度数据
		{
			if(i < piic->wr_info.len - 1)
				*(piic->wr_info.data + i) = IIC_Simulation_Master_ReceiveByte(piic->wr_info.port, 0);  //数据没接收完，发送有效应答
			else
				*(piic->wr_info.data + i) = IIC_Simulation_Master_ReceiveByte(piic->wr_info.port, 1);  //数据接收完，发送非应答
		}
		IIC_Simulation_Master_Stop(piic->wr_info.port);
		
		return 0;
	}
    
    return 1;
}
