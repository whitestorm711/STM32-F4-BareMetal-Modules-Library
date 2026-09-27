
#ifndef F411_RESET_H
#define F411_RESET_H

#include "f411_platform.h"

enum
{
    F411_RESET_BOR = 25U,
    F411_RESET_PIN = 26U,
    F411_RESET_POR = 27U,
    F411_RESET_SOFTWARE = 28U,
    F411_RESET_IWDG = 29U,
    F411_RESET_WWDG = 30U,
    F411_RESET_LOWPOWER = 31U
};

uint32_t F411_ResetFlags(void);
uint8_t F411_ResetWas(uint8_t flag_bit);
void F411_ResetFlagsClear(void);
void F411_SoftwareReset(void);

#endif
