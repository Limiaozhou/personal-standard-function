/**************** (C) COPYRIGHT  ��۵���Ƕ��ʽ���������� **********************
 * �ļ���  ��i2c_ee.c
 * ����    ��I2C �������ö�д������    
 * ʵ��ƽ̨����۵���STM8������
 * ��汾  ��V2.1.0
 * ����    �����  QQ:779814207
 * ����    ��
 * �޸�ʱ�� ��2011-12-20

  ��۵���STM8������Ӳ������
    |--------------------|
    |  I2C_SCL-PE1       |
    |  I2C_SDA-PE2       |
    |--------------------|

*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_i2c.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
  STANDARDSPEED=100000,
  FASTSPEED=400000
}I2C_SpeedMode_TypeDef;

#define I2C1_SLAVE_ADDRESS7    0x00

void I2C_MASTERMODE_Init(I2C_SpeedMode_TypeDef I2C_Speed);

//void Read_8816(u8 *pBuffer, u8 index, u8 NumByteToRead);
void IIC_TVOC(unsigned char *buf,char len);
void TVOC_ReadDat(float * tvoc, float * tovcco2);

#endif /* __I2C_EEPROM_H */

/******************* (C) COPYRIGHT ��۵���Ƕ��ʽ���������� *****END OF FILE****/


