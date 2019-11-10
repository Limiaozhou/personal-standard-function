#ifndef __MID_TIMERS_H
#define __MID_TIMERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stdlib.h"
#include "stm32f10x.h"  //包含数据类型等

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#define TIMERS_MAX_TIMEOUT 0xFFFFFFFF

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

  /*
   * Set a Timer
   */
uint8_t timer_task_start( uint32_t timeout, uint32_t reloadTimeout, uint8_t taskPriority_flag, void (*timeout_task)(void) );

  /*
   * Stop a Timer
   */
uint8_t timer_task_stop( void (*timeout_task)(void) );

  /*
   * Update timer tables
   */
void Timer_Update( void * updateTime );  //时间间隔

void timeout_task_loop(void);

  /*
   * Adjust timer tables
   */
void timers_adjust( void * timer_elapsed );  //时间流

  /*
   * Get the tick count of a Timer.
   */
uint32_t get_timeout( void (*timeout_task)(void) );

  /*
   * Count active timers
   */
uint8_t timer_num_active( void );

  /*
   * Get the next timer expiration.
   * This function should only be called in OSAL_PwrMgr.c
   */
uint32_t next_timeout( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __MID_TIMERS_H */
