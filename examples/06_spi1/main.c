
#include "f411_spi1.h"

int main(void)
{
    F411_SPI1_Config cfg = {0U, 0U, 0U, 2U};
    F411_SPI1_Init(&cfg);
    F411_SPI1_CS_Init(GPIOB, 0U);

    while (1)
    {
        F411_SPI1_CS_Low(GPIOB, 0U);
        (void)F411_SPI1_Transfer(0x9FU);
        F411_SPI1_CS_High(GPIOB, 0U);
    }
}
