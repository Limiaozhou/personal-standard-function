#ifndef _CO2_H_
#define _CO2_H_


#include "stm8s_gpio.h"
#include "stm8s_it.h"
#define u8  unsigned char 
#define u32 unsigned long 
#define EXTIC_vector           0x00

void init_Co2();
void Get_CO2(float *tempfs);
void Init_Timer4(void);

#endif