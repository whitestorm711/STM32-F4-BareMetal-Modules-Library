
#ifndef F411_GPIO_H
#define F411_GPIO_H

#include "f411_platform.h"

enum
{
    F411_GPIO_INPUT  = 0U,
    F411_GPIO_OUTPUT = 1U,
    F411_GPIO_AF     = 2U,
    F411_GPIO_ANALOG = 3U
};

enum
{
    F411_GPIO_PUSH_PULL  = 0U,
    F411_GPIO_OPEN_DRAIN = 1U
};

enum
{
    F411_GPIO_PULL_NONE = 0U,
    F411_GPIO_PULL_UP   = 1U,
    F411_GPIO_PULL_DOWN = 2U
};

typedef struct
{
    GPIO_TypeDef *port;
    uint8_t pin;
    uint8_t mode;
    uint8_t pull;
    uint8_t speed;
    uint8_t output_type;
    uint8_t alternate_function;
} F411_GPIO_PinConfig;

void F411_GPIO_Init(const F411_GPIO_PinConfig *cfg);
void F411_GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t high);
void F411_GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);
uint8_t F411_GPIO_Read(GPIO_TypeDef *port, uint8_t pin);
void F411_GPIO_Set(GPIO_TypeDef *port, uint8_t pin);
void F411_GPIO_Reset(GPIO_TypeDef *port, uint8_t pin);
void F411_GPIO_ConfigOutput(GPIO_TypeDef *port, uint8_t pin, uint8_t speed);
void F411_GPIO_ConfigInput(GPIO_TypeDef *port, uint8_t pin, uint8_t pull);
void F411_GPIO_ConfigAnalog(GPIO_TypeDef *port, uint8_t pin);
void F411_GPIO_ConfigAF(GPIO_TypeDef *port, uint8_t pin, uint8_t af, uint8_t speed, uint8_t open_drain);

#endif
