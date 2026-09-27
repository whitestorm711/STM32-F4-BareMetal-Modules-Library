
#ifndef F411_IWDG_H
#define F411_IWDG_H

#include "f411_platform.h"

/* prescaler_code 0..7 maps to /4,/8,/16,/32,/64,/128,/256,/256. */
int F411_IWDG_Init(uint8_t prescaler_code, uint16_t reload, uint32_t timeout_loops);
void F411_IWDG_Feed(void);
uint8_t F411_IWDG_Ready(void);

#endif
