#include "app_uart.h"
#include "check.h"
#include "string.h"

Send_NetData send_netdata;
SensorType sensor;

void uart1_write(void)
{
    uint8_t addr[8] = { 0x4F, 0x45, 0x41, 0x03, 0x00, 0x15, 0x00, 0x00 };
    
	send_netdata.head = 0xAA55;
    send_netdata.len = sizeof(send_netdata) - 3;
	send_netdata.ver = 38;
    
	memcpy(send_netdata.addr, addr, 8);
    
	send_netdata.data.cmd = 0xD0;
	send_netdata.data.len = sizeof(send_netdata.data) - 2;
    
    send_netdata.data.temp.key[0] = 0x00;
	send_netdata.data.temp.key[1] = 0x80;
	memcpy(send_netdata.data.temp.dat, &sensor.temp, 4);
    
    send_netdata.data.humi.key[0] = 0x00;
	send_netdata.data.humi.key[1] = 0x82;
	memcpy(send_netdata.data.humi.dat, &sensor.humi, 4);
    
    send_netdata.data.light.key[0] = 0x00;
	send_netdata.data.light.key[1] = 0x06;
	memcpy(send_netdata.data.light.dat, &sensor.light, 4);
    
    send_netdata.data.co2.key[0] = 0x00;
	send_netdata.data.co2.key[1] = 0x7A;
	memcpy(send_netdata.data.co2.dat, &sensor.co2, 4);
    
    send_netdata.data.pm25.key[0] = 0x00;
	send_netdata.data.pm25.key[1] = 0x68;
	memcpy(send_netdata.data.pm25.dat, &sensor.pm25, 4);
    
	send_netdata.crc16 = check_crc16_modbus_calc(&send_netdata.ver, send_netdata.len - 2);  //从ver开始crc校验
    
    uart_write( Uart1, (uint8_t *)&send_netdata, sizeof(send_netdata) );
}

void uart3_write(void)
{
    
}
