
#ifndef F411_RCC_H
#define F411_RCC_H

#include "f411_platform.h"

void F411_RCC_EnableGPIO(GPIO_TypeDef *gpio);
void F411_RCC_EnableDMA1(void);
void F411_RCC_EnableDMA2(void);
void F411_RCC_EnableCRC(void);
void F411_RCC_EnableTIM1(void);
void F411_RCC_EnableTIM2(void);
void F411_RCC_EnableUSART2(void);
void F411_RCC_EnableADC1(void);
void F411_RCC_EnableSPI1(void);
void F411_RCC_EnableI2C1(void);
void F411_RCC_EnableSYSCFG(void);

/* Force SYSCLK to the internal 16 MHz HSI oscillator. */
int F411_RCC_UseHSI16(void);

/* Calculate timer kernel clocks using both HCLK and PCLK because STM32F411
 * also exposes the RCC_DCKCFGR TIMPRE option. */
uint32_t F411_RCC_APB1TimerClockHz(uint32_t hclk_hz, uint32_t pclk1_hz);
uint32_t F411_RCC_APB2TimerClockHz(uint32_t hclk_hz, uint32_t pclk2_hz);

#endif
