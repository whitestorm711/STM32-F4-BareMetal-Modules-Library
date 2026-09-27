
#include "f411_rcc.h"
#include "f411_adc1.h"

int main(void)
{
    uint16_t sample;
    F411_RCC_UseHSI16();
    F411_ADC1_Init();
    F411_ADC1_ConfigAnalogPin(GPIOA, 0U); /* ADC1 channel 0 */

    while (1)
    {
        (void)F411_ADC1_Read(0U, 4U, &sample, 100000U);
        (void)sample;
    }
}
