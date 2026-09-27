
#include "f411_rcc.h"
#include "f411_tim1_3phase.h"

int main(void)
{
    uint8_t dtg;
    F411_RCC_UseHSI16();
    dtg = F411_TIM1_EncodeDeadtime(16000000U, 1U, 1000U);
    F411_TIM1_3Phase_Init(16000000U, 20000U, 0U, dtg);
    F411_TIM1_3Phase_SetDutyPermille(100U, 200U, 300U);

    while (1) {}
}
