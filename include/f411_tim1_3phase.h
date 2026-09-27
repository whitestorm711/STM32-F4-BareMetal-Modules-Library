
#ifndef F411_TIM1_3PHASE_H
#define F411_TIM1_3PHASE_H

#include "f411_platform.h"

/* Advanced reference driver for the six TIM1 outputs:
 * PA8/PA9/PA10 = CH1/CH2/CH3
 * PB13/PB14/PB15 = CH1N/CH2N/CH3N
 * All six use AF1. */
int F411_TIM1_3Phase_Init(uint32_t timer_clock_hz,
                          uint32_t pwm_hz,
                          uint16_t prescaler,
                          uint8_t deadtime_dtg);
void F411_TIM1_3Phase_SetDutyPermille(uint16_t phase1, uint16_t phase2, uint16_t phase3);
void F411_TIM1_3Phase_EnableOutputs(void);
void F411_TIM1_3Phase_DisableOutputs(void);
void F411_TIM1_3Phase_Stop(void);
uint8_t F411_TIM1_EncodeDeadtime(uint32_t timer_clock_hz, uint8_t ckd_div, uint32_t deadtime_ns);

#endif
