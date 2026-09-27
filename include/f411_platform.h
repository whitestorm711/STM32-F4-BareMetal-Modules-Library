#ifndef F411_PLATFORM_H
#define F411_PLATFORM_H

#include <stdint.h>
#include <stddef.h>

/*
 * Select the STM32F411 CMSIS device header from the compiler symbol.
 * STM32F411xC and STM32F411xE use the common STM32F411xE device header
 * family in current ST CMSIS packages.
 *
 * This library intentionally uses CMSIS register structures only. It does
 * not use STM32 HAL/LL peripheral drivers.
 */
#if defined(STM32F411xE)
#include "stm32f411xe.h"
#else
#error "Define STM32F411xE before including f411_platform.h"
#endif

#endif
