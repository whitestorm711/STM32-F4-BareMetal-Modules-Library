#include "f411_rcc.h"
#include "f411_uart2.h"
#include "f411_packet.h"

static F411_PacketParser parser;
static volatile uint8_t packet_ready;
static volatile uint8_t packet_length;
static volatile uint8_t packet_copy[F411_PACKET_MAX_PAYLOAD];

static void on_packet(const uint8_t *payload, uint8_t length)
{
    for (uint8_t i = 0U; i < length; i++) packet_copy[i] = payload[i];
    packet_length = length;
    packet_ready = 1U;
}

static void rx_byte(uint8_t byte)
{
    (void)F411_PacketParser_Feed(&parser, byte);
}

int main(void)
{
    F411_RCC_UseHSI16();
    F411_USART2_Init(16000000U, 115200U);
    F411_PacketParser_Init(&parser, on_packet);
    F411_USART2_SetRxCallback(rx_byte);
    F411_USART2_EnableRxInterrupt();

    while (1)
    {
        if (packet_ready)
        {
            packet_ready = 0U;
            F411_USART2_SendString("RX packet: ");
            F411_USART2_Send((const uint8_t *)packet_copy, packet_length);
            F411_USART2_SendString("\r\n");
        }
    }
}
