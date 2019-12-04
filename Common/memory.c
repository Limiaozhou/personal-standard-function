#include "memory.h"

//逆序复制
void mem_copy_reverse(void * pdest, void * psrc, uint32_t len)
{
    uint32_t index;
    uint8_t * ptemp = (uint8_t*)psrc + (len - 1);

    for( index = 0; index < len; index++ )
        *((uint8_t*)pdest++) = *(temp--);
}