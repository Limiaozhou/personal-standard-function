#include "app_uart.h"
#include "check.h"
#include "string.h"

#define UART_NET Uart1
#define UART_SENSOR Uart3
#define UART_DEBUG Uart5

static uint8_t net_addr[8] = { 0x4F, 0x45, 0x41, 0x03, 0x00, 0x1A, 0x00, 0x00 };  //硬件编号地址
static Sensor_Data sensor;  //传感器联合数据
static Sensor_485_Type sensor_type = Sensor485_1;  //传感器类型
static uint8_t resend_num = 0;  //重发次数

static void sensor_485_type_loop(void);  //读取485传感器类型循环

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
static void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num);

static void sensor_485_deal(uint8_t * pdata, uint32_t len);  //处理485传感器数据

//发送数据到网络
void network_data_write(void)
{
    Send_NetData send_netdata;
    
	send_netdata.head = 0xAA55;
    send_netdata.len = sizeof(send_netdata) - 3;
	send_netdata.ver = 38;
    
	memcpy(send_netdata.addr, net_addr, 8);
    
	send_netdata.data.cmd = 0xD0;
	send_netdata.data.len = sizeof(send_netdata.data) - 2;
    
    send_netdata.data.temp.key[0] = 0x00;
	send_netdata.data.temp.key[1] = 0x80;
	memcpy(send_netdata.data.temp.dat, &sensor.temp, 4);
    
	send_netdata.data.turbidity.key[0] = 0x00;
	send_netdata.data.turbidity.key[1] = 0xAA;
	memcpy(send_netdata.data.turbidity.dat, &sensor.turbidity, 4);
    
	send_netdata.data.ca.key[0] = 0x00;
	send_netdata.data.ca.key[1] = 0x1A;
	memcpy(send_netdata.data.ca.dat, &sensor.ca, 4);
    
    send_netdata.data.ph.key[0] = 0x00;
	send_netdata.data.ph.key[1] = 0xA6;
	memcpy(send_netdata.data.ph.dat, &sensor.ph, 4);
    
    send_netdata.data.Do.key[0] = 0x00;
	send_netdata.data.Do.key[1] = 0xA4;
	memcpy(send_netdata.data.Do.dat, &sensor.Do, 4);
    
    send_netdata.data.nh4.key[0] = 0x00;
	send_netdata.data.nh4.key[1] = 0xA2;
	memcpy(send_netdata.data.nh4.dat, &sensor.nh4, 4);
    
    send_netdata.fill[0] = 0;
    send_netdata.fill[1] = 0;
    
	send_netdata.crc16 = check_crc16_modbus_calc(&send_netdata.ver, send_netdata.len - 2);  //从ver开始crc校验
    
    uart_write( UART_NET, (uint8_t *)&send_netdata, sizeof(send_netdata) );
    uart_write( UART_DEBUG, (uint8_t *)&send_netdata, sizeof(send_netdata) );
    uart_write( UART_DEBUG, (uint8_t *)&sensor, sizeof(sensor) );
}

//发送485传感器读取指令
void sensor_485_write(void)
{
    uint8_t send_sensor[8] = {0};
    
    switch(sensor_type)
    {
        case Sensor485_1 :
            mb_rtu_read_holdreg(send_sensor, Sensor485_1, 0x03, 0x01, 0x02);
        break;
        
        case Sensor485_2 :
            mb_rtu_read_holdreg(send_sensor, Sensor485_2, 0x03, 0x01, 0x02);
        break;
        
        case Sensor485_3 :
            mb_rtu_read_holdreg(send_sensor, Sensor485_3, 0x04, 0x07, 0x04);
        break;
    }
    
    if(++resend_num >= 3)
        sensor_485_type_loop();
    
    uart_write( UART_SENSOR, send_sensor, sizeof(send_sensor) );
}

//接收处理485传感器数据
void sensor_485_read(void)
{
    uart_read(UART_SENSOR, sensor_485_deal);
}

//debug串口发送数据
void uart_debug_send(void)
{
    uart_send_loop(UART_DEBUG);
}

//读取485传感器类型循环
static void sensor_485_type_loop(void)
{
    if(++sensor_type >= num_of_sensor_485)
        sensor_type = Sensor485_1;
    
    resend_num = 0;
}

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
static void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num)
{
    uint16_t crc = 0;
    
    *(pbuf + 0) = dev_adr;
    *(pbuf + 1) = func;
    *(pbuf + 2) = (reg_adr >> 8) & 0xFF;
    *(pbuf + 3) = reg_adr & 0xFF;
    *(pbuf + 4) = (reg_num >> 8) & 0xFF;
    *(pbuf + 5) = reg_num & 0xFF;
    
    crc = check_crc16_modbus_calc(pbuf, 6);
    
    *(pbuf + 6) = crc & 0xFF;
    *(pbuf + 7) = (crc >> 8) & 0xFF;
}

//处理485传感器数据
static void sensor_485_deal(uint8_t * pdata, uint32_t len)
{
    Float_Byte float_data;
    
    uart_write( UART_DEBUG, pdata, len );
    
    while(len--)
    {
        switch(*pdata)
        {
            case Sensor485_1 :  //头校验
                if( !check_crc16_modbus_calc(pdata, *(pdata + 2) + 5) )  //crc16校验，为0表示通过
                {
                    float_data.byte.byte1 = *(pdata + 4);
                    float_data.byte.byte2 = *(pdata + 3);
                    float_data.byte.byte3 = *(pdata + 6);
                    float_data.byte.byte4 = *(pdata + 5);
                    
                    sensor.turbidity = float_data.data;
                    sensor_485_type_loop();
                }
            break;
            
            case Sensor485_2 :
                if( !check_crc16_modbus_calc(pdata, *(pdata + 2) + 5) )
                {
                    float_data.byte.byte1 = *(pdata + 4);
                    float_data.byte.byte2 = *(pdata + 3);
                    float_data.byte.byte3 = *(pdata + 6);
                    float_data.byte.byte4 = *(pdata + 5);
                    
                    sensor.ca = float_data.data;
                    sensor_485_type_loop();
                }
            break;
            
            case Sensor485_3 :
                if( !check_crc16_modbus_calc(pdata, *(pdata + 2) + 5) )
                {
                    sensor.ph = *(pdata + 4);
                    sensor.ph += (uint32_t)*(pdata + 3) << 8;
                    sensor.ph /= 100;
                    
                    sensor.Do = *(pdata + 6);
                    sensor.Do += (uint32_t)*(pdata + 5) << 8;
                    sensor.Do /= 100;
                    
                    sensor.nh4 = *(pdata + 8);
                    sensor.nh4 += (uint32_t)*(pdata + 7) << 8;
                    sensor.nh4 /= 100;
                    
                    sensor.temp = *(pdata + 10);
                    sensor.temp += (uint32_t)*(pdata + 9) << 8;
                    sensor.temp /= 100;
                    
                    sensor_485_type_loop();
                }
            break;
        }
        
        ++pdata;
    }
}
