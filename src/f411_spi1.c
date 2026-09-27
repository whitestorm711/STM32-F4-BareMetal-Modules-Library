
#include "f411_spi1.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

int F411_SPI1_Init(const F411_SPI1_Config *config)
{
    uint32_t cr1 = 0U;
    if (config == NULL || config->baud_prescaler_code > 7U) return -1;

    F411_RCC_EnableSPI1();
    F411_GPIO_ConfigAF(GPIOA, 5U, 5U, 3U, 0U); /* SCK */
    F411_GPIO_ConfigAF(GPIOA, 6U, 5U, 3U, 0U); /* MISO */
    F411_GPIO_ConfigAF(GPIOA, 7U, 5U, 3U, 0U); /* MOSI */

    cr1 |= (1U << 2); /* MSTR */
    cr1 |= (1U << 8); /* SSI */
    cr1 |= (1U << 9); /* SSM */
    cr1 |= ((uint32_t)(config->baud_prescaler_code & 7U) << 3);
    if (config->cpol) cr1 |= (1U << 1);
    if (config->cpha) cr1 |= (1U << 0);
    if (config->lsb_first) cr1 |= (1U << 7);

    SPI1->CR1 = cr1;
    SPI1->CR2 = 0U;
    SPI1->CR1 |= (1U << 6); /* SPE */
    return 0;
}

uint8_t F411_SPI1_Transfer(uint8_t tx)
{
    while ((SPI1->SR & (1U << 1)) == 0U) {} /* TXE */
    *((volatile uint8_t *)&SPI1->DR) = tx;
    while ((SPI1->SR & (1U << 0)) == 0U) {} /* RXNE */
    return *((volatile uint8_t *)&SPI1->DR);
}

void F411_SPI1_CS_Init(GPIO_TypeDef *port, uint8_t pin)
{
    F411_GPIO_ConfigOutput(port, pin, 2U);
    F411_GPIO_Set(port, pin); /* deselect before the first transfer */
}

void F411_SPI1_CS_Low(GPIO_TypeDef *port, uint8_t pin)
{
    F411_GPIO_Reset(port, pin);
}

void F411_SPI1_CS_High(GPIO_TypeDef *port, uint8_t pin)
{
    F411_GPIO_Set(port, pin);
}
