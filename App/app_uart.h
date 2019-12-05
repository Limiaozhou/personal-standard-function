#ifndef __APP_UART_H
#define __APP_UART_H

#include "bsp_usart.h"

/*** 传感数据结构体 ***/
typedef struct
{
    uint8_t key[2];  //服务器上对应传感器寄存器地址
    uint8_t dat[4];
}SendData_Item;

/*** 检测参数编号和数据 ***/
typedef struct
{
    uint8_t cmd;
    uint8_t len;  //长度，数据字节数，6 * n
    
    SendData_Item temp;  //温度
//    SendData_Item humi;  //湿度
//    SendData_Item light;  //光照
//    SendData_Item co2;
//    SendData_Item pm25;  //pm2.5
//    SendData_Item ultvio;  //紫外线
//    SendData_Item tvoc;  //有机气体
//    SendData_Item fx;  //风向
//    SendData_Item fs;  //风速
//    SendData_Item so2;
//    SendData_Item pressure;  //气压
//    SendData_Item yuxue;  //是否有雨雪
    SendData_Item turbidity;  //浊度
    SendData_Item ca;
    SendData_Item ph;
    SendData_Item Do;  //溶解氧
    SendData_Item nh4;  //浊度
    
//    SendData_Item shade;  //遮阳
//    SendData_Item ventilate;  //风机
//    SendData_Item juanmo;  //卷膜
//    SendData_Item wat4;  //水帘
//    SendData_Item yang;  //内，遮阳1
//    SendData_Item yang1;  //外，遮阳2
}SendData;

//要发送的数据结构体
typedef struct
{
    uint16_t head;  //2，帧头
    uint8_t len;  //1，长度，后面字节数，不包括本字节
    uint8_t ver;  //1，数据是否加密标志位
    uint8_t addr[8];  // device code,8，设备地址
    SendData data;  //传感器数据,2 + 6 * n
    uint8_t fill[2];  //2
    uint16_t crc16;  //crc16,2，从ver开始校验
}Send_NetData;

/*** 检测环境参数类别名称 ***/
typedef struct
{
    float temp;  //温度
//    float humi;  //湿度
//    float light;  //光照
//    float co2;
//    float pm25;  //pm2.5
//    float ultvio;  //紫外线
//    float tvoc;  //有机气体
//    float fx;  //风向
//    float fs;  //风速
//    float so2;
//    float pressure;  //气压
//    float yuxue;  //是否有雨雪
    float turbidity;  //浊度
    float ca;
    float ph;
    float Do;  //溶解氧
    float nh4;  //浊度
    
//    float shade;  //遮阳
//    float ventilate;  //风机
//    float juanmo;  //卷膜
//    float wat4;  //水帘
//    float yang;  //内，遮阳1
//    float yang1;  //外，遮阳2
}Sensor_Data;

typedef enum
{
    Sensor485_1 = 1,
    Sensor485_2,
    Sensor485_3,
    num_of_sensor_485
}Sensor_485_Type;

typedef union
{
    float data;
    struct
    {
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
    }byte;
}Float_Byte;

void network_data_write(void);

void sensor_485_write(void);
void sensor_485_read(void);

void uart_debug_send(void);

#endif
