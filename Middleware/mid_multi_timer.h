#ifndef __MID_MULTI_TIMER_H_
#define __MID_MULTI_TIMER_H_

#include "stdint.h"
#include "bsp_time.h"

typedef struct Timer
{
    uint32_t timeout;  //定时
    uint32_t repeat;  //重复时间间隔
    void (*timeout_cb)(void);
    struct Timer* next;
}Timer;

#ifdef __cplusplus
extern "C" {
#endif

void timer_init(struct Timer* handle, void(*timeout_cb)(), uint32_t timeout, uint32_t repeat);
int  timer_start(struct Timer* handle);
void timer_stop(struct Timer* handle);
void timer_ticks(void);
void timer_loop(void);

#ifdef __cplusplus
}
#endif

#endif
