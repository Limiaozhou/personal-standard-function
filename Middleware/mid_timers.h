#ifndef __MID_TIMERS_H
#define __MID_TIMERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bsp_time.h"
#include "stdlib.h"

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
uint8_t timer_task_start( uint32_t timeout, uint32_t reloadTimeout, void (*timeout_task)(void) );

  /*
   * Stop a Timer
   */
uint8_t timer_task_stop( void (*timeout_task)(void) );

  /*
   * Update timer tables
   */
void Timer_Update( uint32_t updateTime );

  /*
   * Adjust timer tables
   */
void timers_adjust( void );

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
