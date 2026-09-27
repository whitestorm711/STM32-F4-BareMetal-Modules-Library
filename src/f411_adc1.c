
#include "f411_adc1.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

int F411_ADC1_Init(void)
{
    F411_RCC_EnableADC1();

    /* ADC clock = PCLK2 / 2. */
    ADC->CCR &= ~(3U << 16);
    ADC1->CR1 &= ~(3U << 24); /* 12-bit resolution */
    ADC1->CR2 = 0U;
    ADC1->SQR1 = 0U;           /* one conversion */
    ADC1->CR2 |= (1U << 10);   /* EOCS: EOC after each conversion */
    ADC1->CR2 |= (1U << 0);    /* ADON */
    return 0;
}

void F411_ADC1_ConfigAnalogPin(GPIO_TypeDef *port, uint8_t pin)
{
    F411_GPIO_ConfigAnalog(port, pin);
}

int F411_ADC1_Read(uint8_t channel, uint8_t sample_time, uint16_t *value, uint32_t timeout_loops)
{
    uint32_t shift;
    if (value == NULL || channel > 18U || sample_time > 7U) return -2;

    if (channel <= 9U)
    {
        shift = (uint32_t)channel * 3U;
        ADC1->SMPR2 &= ~(7U << shift);
        ADC1->SMPR2 |= ((uint32_t)sample_time << shift);
    }
    else
    {
        shift = (uint32_t)(channel - 10U) * 3U;
        ADC1->SMPR1 &= ~(7U << shift);
        ADC1->SMPR1 |= ((uint32_t)sample_time << shift);
    }

    ADC1->SQR1 &= ~(0xFU << 20);
    ADC1->SQR3 &= ~(0x1FU << 0);
    ADC1->SQR3 |= (uint32_t)channel;

    ADC1->SR &= ~(1U << 1); /* EOC */
    ADC1->CR2 |= (1U << 30); /* SWSTART */

    while ((ADC1->SR & (1U << 1)) == 0U)
    {
        if (timeout_loops == 0U) return -1;
        timeout_loops--;
    }

    *value = (uint16_t)ADC1->DR;
    return 0;
}
