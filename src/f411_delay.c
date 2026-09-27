
#include "f411_delay.h"
#include "f411_systick.h"

void F411_DelayMs(uint32_t ms)
{
    F411_SysTick_DelayMs(ms);
}
