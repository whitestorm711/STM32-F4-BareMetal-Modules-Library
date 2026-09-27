
#include "f411_gpio.h"
#include "f411_rcc.h"

static void set_2bit_field(volatile uint32_t *reg, uint8_t pin, uint32_t value)
{
    uint32_t shift = (uint32_t)pin * 2U;
    *reg &= ~(3U << shift);
    *reg |= ((value & 3U) << shift);
}

void F411_GPIO_Init(const F411_GPIO_PinConfig *cfg)
{
    if (cfg == NULL || cfg->port == NULL || cfg->pin > 15U) return;

    F411_RCC_EnableGPIO(cfg->port);

    set_2bit_field(&cfg->port->MODER, cfg->pin, cfg->mode);
    set_2bit_field(&cfg->port->PUPDR, cfg->pin, cfg->pull);
    set_2bit_field(&cfg->port->OSPEEDR, cfg->pin, cfg->speed);

    if (cfg->pin < 16U)
    {
        cfg->port->OTYPER &= ~(1U << cfg->pin);
        cfg->port->OTYPER |= ((uint32_t)(cfg->output_type & 1U) << cfg->pin);
    }

    if (cfg->mode == F411_GPIO_AF)
    {
        volatile uint32_t *afr = (cfg->pin < 8U) ? &cfg->port->AFR[0] : &cfg->port->AFR[1];
        uint32_t shift = (uint32_t)(cfg->pin & 7U) * 4U;
        *afr &= ~(0xFU << shift);
        *afr |= ((uint32_t)(cfg->alternate_function & 0xFU) << shift);
    }
}

void F411_GPIO_Set(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == NULL || pin > 15U) return;
    port->BSRR = (1U << pin);
}

void F411_GPIO_Reset(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == NULL || pin > 15U) return;
    port->BSRR = (1U << (pin + 16U));
}

void F411_GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t high)
{
    if (high) F411_GPIO_Set(port, pin);
    else F411_GPIO_Reset(port, pin);
}

void F411_GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == NULL || pin > 15U) return;
    port->ODR ^= (1U << pin);
}

uint8_t F411_GPIO_Read(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == NULL || pin > 15U) return 0U;
    return (uint8_t)((port->IDR >> pin) & 1U);
}

void F411_GPIO_ConfigOutput(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
{
    F411_GPIO_PinConfig cfg = {
        port, pin, F411_GPIO_OUTPUT, F411_GPIO_PULL_NONE,
        speed & 3U, F411_GPIO_PUSH_PULL, 0U
    };
    F411_GPIO_Init(&cfg);
}

void F411_GPIO_ConfigInput(GPIO_TypeDef *port, uint8_t pin, uint8_t pull)
{
    F411_GPIO_PinConfig cfg = {
        port, pin, F411_GPIO_INPUT, pull & 3U,
        0U, F411_GPIO_PUSH_PULL, 0U
    };
    F411_GPIO_Init(&cfg);
}

void F411_GPIO_ConfigAnalog(GPIO_TypeDef *port, uint8_t pin)
{
    F411_GPIO_PinConfig cfg = {
        port, pin, F411_GPIO_ANALOG, F411_GPIO_PULL_NONE,
        0U, F411_GPIO_PUSH_PULL, 0U
    };
    F411_GPIO_Init(&cfg);
}

void F411_GPIO_ConfigAF(GPIO_TypeDef *port, uint8_t pin, uint8_t af, uint8_t speed, uint8_t open_drain)
{
    F411_GPIO_PinConfig cfg = {
        port, pin, F411_GPIO_AF, F411_GPIO_PULL_NONE,
        speed & 3U, open_drain ? F411_GPIO_OPEN_DRAIN : F411_GPIO_PUSH_PULL, af & 0xFU
    };
    F411_GPIO_Init(&cfg);
}
