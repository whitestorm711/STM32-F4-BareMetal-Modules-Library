
#ifndef F411_ADC1_H
#define F411_ADC1_H

#include "f411_platform.h"

int F411_ADC1_Init(void);
void F411_ADC1_ConfigAnalogPin(GPIO_TypeDef *port, uint8_t pin);
int F411_ADC1_Read(uint8_t channel, uint8_t sample_time, uint16_t *value, uint32_t timeout_loops);

#endif
