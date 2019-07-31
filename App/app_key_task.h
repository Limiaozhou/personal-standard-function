#ifndef __APP_KEY_TASK_H
#define __APP_KEY_TASK_H

#include "stm32f7xx_hal.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_tim_common.h"

void key_task(uint8_t* key, uint8_t* mode, uint8_t* cnt, uint8_t* press);

#endif
