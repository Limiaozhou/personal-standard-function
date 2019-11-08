#include "mid_multi_timer.h"

//timer handle list head
static struct Timer* head_handle = NULL;

//Timer ticks
static uint32_t _timer_ticks = 0;

/**
  * @brief  Initializes the timer struct handle.
  * @param  handle: the timer handle strcut.
  * @param  timeout_cb: timeout callback.
  * @param  repeat: repeat interval time.
  * @retval None
  */
void timer_init(struct Timer* handle, void(*timeout_cb)(), uint32_t timeout, uint32_t repeat)
{
	// memset(handle, sizeof(struct Timer), 0);
	handle->timeout_cb = timeout_cb;
	handle->timeout = _timer_ticks + timeout;
	handle->repeat = repeat;
}

/**
  * @brief  Start the timer work, add the handle into work list.
  * @param  btn: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int timer_start(struct Timer* handle)
{
	struct Timer* target = head_handle;
	while(target) {
		if(target == handle) return -1;	//already exist.
		target = target->next;  //遍历链表
	}
	handle->next = head_handle;
	head_handle = handle;  //插表头，头指针指向新节点
	return 0;
}

/**
  * @brief  Stop the timer work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void timer_stop(struct Timer* handle)
{
	struct Timer** curr;
	for(curr = &head_handle; *curr; ) {  //指向头指针的指针；判断当前节点的指针是否为空
		struct Timer* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;  //移除目标节点
//			free(entry);
		} else
			curr = &entry->next;  //指向下个节点指针的指针
	}
}

/**
  * @brief  main loop.
  * @param  None.
  * @retval None
  */
void timer_loop()
{
	struct Timer* target;
	for(target=head_handle; target; target=target->next) {
		if(_timer_ticks >= target->timeout) {
			if(target->repeat == 0) {
				timer_stop(target);  //不重复任务执行一次就从链表移除
			} else {
				target->timeout = _timer_ticks + target->repeat;
			}
			target->timeout_cb();
		}
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 1ms.
  * @param  None.
  * @retval None.
  */
void timer_ticks()
{
//	_timer_ticks++;
    _timer_ticks = get_tim3_ticks();
}

