
#ifndef F411_SPI1_H
#define F411_SPI1_H

#include "f411_platform.h"

typedef struct
{
    uint8_t cpol;
    uint8_t cpha;
    uint8_t lsb_first;
    uint8_t baud_prescaler_code; /* 0..7 -> SPI_CR1 BR[2:0] */
} F411_SPI1_Config;

int F411_SPI1_Init(const F411_SPI1_Config *config);
uint8_t F411_SPI1_Transfer(uint8_t tx);
void F411_SPI1_CS_Init(GPIO_TypeDef *port, uint8_t pin);
void F411_SPI1_CS_Low(GPIO_TypeDef *port, uint8_t pin);
void F411_SPI1_CS_High(GPIO_TypeDef *port, uint8_t pin);

#endif
