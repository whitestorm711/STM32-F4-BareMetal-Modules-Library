
#include "f411_rcc.h"
#include "f411_uart2.h"
#include "f411_uart2_dma.h"

static uint8_t rx_buffer[128];
static const uint8_t message[] = "DMA hello\r\n";

int main(void)
{
    F411_RCC_UseHSI16();
    F411_USART2_Init(16000000U, 115200U);
    F411_USART2_DMA_RX_Start(rx_buffer, sizeof(rx_buffer));
    F411_USART2_DMA_TX_Start(message, sizeof(message) - 1U);

    while (1)
    {
        uint16_t write_pos = F411_USART2_DMA_RX_Position(sizeof(rx_buffer));
        (void)write_pos;
    }
}
