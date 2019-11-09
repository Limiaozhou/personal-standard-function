/*********************************************************************
 * INCLUDES
 */

#include "mid_timers.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef struct
{
    void *next;
    void (*timeout_task)(void);
    uint32_t timeout;
    uint32_t reloadTimeout;
    uint8_t rmTimer_flag;  //节点移除标志，0-移除
    uint8_t taskPerform_flag;  //任务执行标志，1-执行
    uint8_t taskPriority_flag;  //任务优先执行标志，1-执行，中断调用处理函数
}Timer_TaskTypeDef;

/*********************************************************************
 * GLOBAL VARIABLES
 */

Timer_TaskTypeDef *timerHead;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
Timer_TaskTypeDef *AddTimer(uint32_t timeout, uint32_t reloadTimeout, uint8_t taskPriority_flag, void (*timeout_task)(void));
Timer_TaskTypeDef *FindTimer(void (*timeout_task)(void));
void DeleteTimer(Timer_TaskTypeDef *rmTimer);

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      AddTimer
 *
 * @brief   Add a timer to the timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 * @param   timeout
 *
 * @return  Timer_TaskTypeDef * - pointer to newly created timer
 */
Timer_TaskTypeDef * AddTimer(uint32_t timeout, uint32_t reloadTimeout, uint8_t taskPriority_flag, void (*timeout_task)(void))
{
    Timer_TaskTypeDef *newTimer;
    Timer_TaskTypeDef *srchTimer;

    // Look for an existing timer first
    newTimer = FindTimer(timeout_task);
    if ( newTimer )
    {
        // Timer is found - update it.
        newTimer->timeout = timeout;
        newTimer->reloadTimeout = reloadTimeout;
        newTimer->rmTimer_flag = 1;
        newTimer->taskPerform_flag = 0;
        newTimer->taskPriority_flag = taskPriority_flag;

        return ( newTimer );
    }
    else
    {
        // New Timer
        newTimer = malloc( sizeof( Timer_TaskTypeDef ) );

        if ( newTimer )
        {
            // Fill in new timer
            newTimer->timeout = timeout;
            newTimer->reloadTimeout = reloadTimeout;
            newTimer->rmTimer_flag = 1;
            newTimer->taskPerform_flag = 0;
            newTimer->taskPriority_flag = taskPriority_flag;
            newTimer->timeout_task = timeout_task;
            newTimer->next = (void *)NULL;

            // Does the timer list already exist
            if ( timerHead == NULL )
            {
                // Start task list
                timerHead = newTimer;
            }
            else
            {
                // Add it to the end of the timer list
                srchTimer = timerHead;

                // Stop at the last record
                while ( srchTimer->next )
                    srchTimer = srchTimer->next;

                // Add to the list
                srchTimer->next = newTimer;
            }

            return ( newTimer );
        }
        else
            return ( (Timer_TaskTypeDef *)NULL );
    }
}

/*********************************************************************
 * @fn      FindTimer
 *
 * @brief   Find a timer in a timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 *
 * @return  Timer_TaskTypeDef *
 */
Timer_TaskTypeDef *FindTimer(void (*timeout_task)(void))
{
    Timer_TaskTypeDef *srchTimer;

    // Head of the timer list
    srchTimer = timerHead;

    // Stop when found or at the end
    while ( srchTimer )
    {
        if ( srchTimer->timeout_task == timeout_task )
            break;

        // Not this one, check another
        srchTimer = srchTimer->next;
    }

    return ( srchTimer );
}

/*********************************************************************
 * @fn      DeleteTimer
 *
 * @brief   Delete a timer from a timer list.
 *
 * @param   table
 * @param   rmTimer
 *
 * @return  none
 */
void DeleteTimer(Timer_TaskTypeDef *rmTimer)
{
    // Does the timer list really exist
    if ( rmTimer )
    {
    // Clear the Timer flag and Timer_Update() will delete 
    // the timer from the list.
        rmTimer->rmTimer_flag = 0;
    }
}

/*********************************************************************
 * @fn      timer_task_start
 *
 * @brief
 *
 *   This function is called to start a timer to expire in n mSecs.
 *   When the timer expires, the calling task will get the specified event.
 *
 * @param   uint8 taskID - task id to set timer for
 * @param   uint16 event_id - event to be notified with
 * @param   UNINT16 timeout_value - in milliseconds.
 *
 * @return  SUCCESS, or NO_TIMER_AVAIL.
 */
uint8_t timer_task_start( uint32_t timeout, uint32_t reloadTimeout, uint8_t taskPriority_flag, void (*timeout_task)(void) )
{
    Timer_TaskTypeDef *newTimer;

    // Add timer
    newTimer = AddTimer(timeout, reloadTimeout, taskPriority_flag, timeout_task);

    return ( (newTimer != NULL) ? SUCCESS : ERROR );
}

/*********************************************************************
 * @fn      timer_task_stop
 *
 * @brief
 *
 *   This function is called to stop a timer that has already been started.
 *   If ZSUCCESS, the function will cancel the timer and prevent the event
 *   associated with the timer from being set for the calling task.
 *
 * @param   uint8 task_id - task id of timer to stop
 * @param   uint16 event_id - identifier of the timer that is to be stopped
 *
 * @return  SUCCESS or INVALID_EVENT_ID
 */
uint8_t timer_task_stop( void (*timeout_task)(void) )
{
    Timer_TaskTypeDef *foundTimer;

    // Find the timer to stop
    foundTimer = FindTimer(timeout_task);
    if ( foundTimer )
    {
        DeleteTimer( foundTimer );
    }

    return ( (foundTimer != NULL) ? SUCCESS : ERROR );
}

/*********************************************************************
 * @fn      Timer_Update
 *
 * @brief   Update the timer structures for a timer tick.
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
void Timer_Update( uint32_t updateTime )
{
    Timer_TaskTypeDef *srchTimer;  //当前节点
    Timer_TaskTypeDef *prevTimer;  //前节点

    // Look for open timer slot
    if ( timerHead != NULL )
    {
        // Add it to the end of the timer list
        srchTimer = timerHead;
        prevTimer = (void *)NULL;

        // Look for open timer slot
        while ( srchTimer )
        {
            Timer_TaskTypeDef *freeTimer = NULL;

            if (srchTimer->timeout <= updateTime)
            {
                srchTimer->timeout = 0;
            }
            else
            {
                srchTimer->timeout = srchTimer->timeout - updateTime;
            }

            // Check for reloading
            if ( (srchTimer->timeout == 0) && (srchTimer->reloadTimeout) && (srchTimer->rmTimer_flag) )
            {
                // Notify the task of a timeout
                if(srchTimer->taskPriority_flag)
                {
                    srchTimer->taskPerform_flag = 0;
                    srchTimer->timeout_task();
                }
                else
                    srchTimer->taskPerform_flag = 1;

                // Reload the timer timeout value
                srchTimer->timeout = srchTimer->reloadTimeout;
            }

            // When timeout or delete (rmTimer_flag == 0)
            if ( srchTimer->timeout == 0 || srchTimer->rmTimer_flag == 0 )
            {
                // Take out of list
                if ( prevTimer == NULL )
                    timerHead = srchTimer->next;
                else
                    prevTimer->next = srchTimer->next;

                // Setup to free memory
                freeTimer = srchTimer;

                // Next
                srchTimer = srchTimer->next;
            }
            else
            {
                // Get next
                prevTimer = srchTimer;
                srchTimer = srchTimer->next;
            }

            if ( freeTimer )
            {
                if ( freeTimer->timeout == 0 )
                {
                    if(freeTimer->taskPriority_flag)
                    {
                        freeTimer->taskPerform_flag = 0;
                        freeTimer->timeout_task();
                    }
                    else
                        freeTimer->taskPerform_flag = 1;
                }
                free( freeTimer );
            }
        }
    }
}

/*********************************************************************
 * @fn      timers_adjust
 *
 * @brief   Update the timer structures for elapsed ticks.
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
void timers_adjust( void )
{
    static uint32_t oldTime = 0;
    static uint8_t oldTime_flag = 0;  //设置oldTime标志
    uint32_t nowTime = 0;
    uint32_t eTime = 0;

    if ( timerHead != NULL )
    {
        if(!oldTime_flag)
        {
            oldTime = get_tim3_ticks();
            oldTime_flag = 1;
        }
        
        // Compute elapsed time (msec)
        nowTime = get_tim3_ticks();
        if(nowTime >= oldTime)
            eTime = nowTime - oldTime;
        else
            eTime = TIMERS_MAX_TIMEOUT - oldTime + nowTime;

        if ( eTime )
        {
            oldTime = nowTime;
            Timer_Update( eTime );
        }
    }
}

/*********************************************************************
 * @fn      get_timeout
 *
 * @brief
 *
 * @param   uint8 task_id - task id of timer to check
 * @param   uint16 event_id - identifier of timer to be checked
 *
 * @return  Return the timer's tick count if found, zero otherwise.
 */
uint32_t get_timeout( void (*timeout_task)(void) )
{
    uint32_t rtrn = 0;
    Timer_TaskTypeDef *tmr;

    tmr = FindTimer(timeout_task);

    if ( tmr )
    {
        rtrn = tmr->timeout;
    }

    return rtrn;
}

/*********************************************************************
 * @fn      timer_num_active
 *
 * @brief
 *
 *   This function counts the number of active timers.
 *
 * @return  uint8 - number of timers
 */
uint8_t timer_num_active( void )
{
    uint8_t num_timers = 0;
    Timer_TaskTypeDef *srchTimer;

    // Head of the timer list
    srchTimer = timerHead;

    // Count timers in the list
    while ( srchTimer != NULL )
    {
        num_timers++;
        srchTimer = srchTimer->next;
    }

    return num_timers;
}

/*********************************************************************
 * @fn      next_timeout
 *
 * @brief
 *
 *   Search timer table to return the lowest timeout value. If the
 *   timer list is empty, then the returned timeout will be zero.
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
uint32_t next_timeout( void )
{
    uint32_t nextTimeout;
    Timer_TaskTypeDef *srchTimer;

    if ( timerHead != NULL )
    {
        // Head of the timer list
        srchTimer = timerHead;
        nextTimeout = TIMERS_MAX_TIMEOUT;

        // Look for the next timeout timer
        while ( srchTimer != NULL )
        {
            if (srchTimer->timeout < nextTimeout)
            {
                nextTimeout = srchTimer->timeout;
            }
            // Check next timer
            srchTimer = srchTimer->next;
        }
    }
    else
    {
        // No timers
        nextTimeout = 0;
    }

    return ( nextTimeout );
}

/*********************************************************************
*********************************************************************/
