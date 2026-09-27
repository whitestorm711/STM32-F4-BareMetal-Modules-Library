
#ifndef F411_UART2_H
#define F411_UART2_H

#include "f411_platform.h"

typedef void (*F411_UART2_RxCallback)(uint8_t byte);

int F411_USART2_Init(uint32_t pclk1_hz, uint32_t baud);
void F411_USART2_SendByte(uint8_t byte);
void F411_USART2_Send(const uint8_t *data, size_t length);
void F411_USART2_SendString(const char *text);
int F411_USART2_ReceiveByte(uint8_t *byte, uint32_t timeout_loops);
void F411_USART2_EnableRxInterrupt(void);
void F411_USART2_DisableRxInterrupt(void);
void F411_USART2_SetRxCallback(F411_UART2_RxCallback callback);
uint8_t F411_USART2_HasRxData(void);

#endif
