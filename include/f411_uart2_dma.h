
#ifndef F411_UART2_DMA_H
#define F411_UART2_DMA_H

#include "f411_platform.h"

int F411_USART2_DMA_RX_Start(uint8_t *buffer, uint16_t length);
void F411_USART2_DMA_RX_Stop(void);
uint16_t F411_USART2_DMA_RX_Position(uint16_t buffer_length);
int F411_USART2_DMA_TX_Start(const uint8_t *buffer, uint16_t length);
uint8_t F411_USART2_DMA_TX_Busy(void);

#endif
