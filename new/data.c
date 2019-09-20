#include "data.h"

EVNDAT Evndat;

void package_send(void);

void package_send(void)
{
    uint8_t send_dat[150] = {0}; //要发送的数据
	uint8_t len;
	
	*send_dat=0x55;
	*(send_dat+1)=0xaa;
	len = 5 + 6 * 7;
	*(send_dat+2)=len;
	
	*(send_dat+3)=0x00;
	*(send_dat+4)=0x80;
	mem_copy_convert_port(send_dat+5,(unsigned char*)&(Evndat.temp20),4);
	
	*(send_dat+ 9)=0x00;
	*(send_dat+10)=0x82;
	mem_copy_convert_port(send_dat+11,(unsigned char*)&(Evndat.humi20),4);
	
	*(send_dat+15)=0x00;
	*(send_dat+16)=0x06;
	mem_copy_convert_port(send_dat+17,(unsigned char*)&(Evndat.light_bhvi),4);
	
	*(send_dat+21)=0x00;
	*(send_dat+22)=0x7A;
	mem_copy_convert_port(send_dat+23,(unsigned char*)&(Evndat.co2),4);
	
	*(send_dat+27)=0x00;
	*(send_dat+28)=0x7C;
	mem_copy_convert_port(send_dat+29,(unsigned char*)&(Evndat.TVOC),4);
	
	*(send_dat+33)=0x00;
	*(send_dat+34)=0x84;
	mem_copy_convert_port(send_dat+35,(unsigned char*)&(Evndat.press),4);
	
	*(send_dat+39)=0x00;
	*(send_dat+40)=0x68;
	mem_copy_convert_port(send_dat+41,(unsigned char*)&(Evndat.pm25),4);
	
	Check_CalaCRC16(send_dat, len-2);
    
    uart_write(UART1_Select, send_dat, len);
	
    delay_ms(1000);
	len = sprintf((char*)send_dat, "light = %.2f, pres = %.2f, pres_temp = %.2f, temp = %.2f, humi = %.2f, co2 = %.2f, tvoc = %.2f, tovcco2 = %.2f, pm = %.2f", 
                      Evndat.light_bhvi, Evndat.press, Evndat.press_temp, Evndat.temp20, Evndat.humi20, Evndat.co2, Evndat.TVOC, Evndat.tvocco2, Evndat.pm25);
	uart_write(UART1_Select, send_dat, len);
	
	IWDG_Feed();
}

void data_deal(void)
{
//    tvoc_eco2_read_start();  //硬件IIC
//	temp_humi_read(&Evndat.temp20,  &Evndat.humi20);  //温湿度，IIC
//	Get_CO2(&Evndat.co2);  //CO2，PWM
	light_read(&Evndat.light_bhvi);  //光照，IIC
//	TVOC_ReadDat(&Evndat.TVOC, &Evndat.tvocco2);  //TVOC，IIC
//	pres_temp_read(&Evndat.press, &Evndat.press_temp);  //气压，IIC
	
	package_send();
}
