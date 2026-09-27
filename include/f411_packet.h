
#ifndef F411_PACKET_H
#define F411_PACKET_H

#include "f411_platform.h"

#define F411_PACKET_MAX_PAYLOAD 64U

typedef void (*F411_PacketCallback)(const uint8_t *payload, uint8_t length);

typedef struct
{
    uint8_t state;
    uint8_t length;
    uint8_t index;
    uint8_t payload[F411_PACKET_MAX_PAYLOAD];
    F411_PacketCallback callback;
} F411_PacketParser;

void F411_PacketParser_Init(F411_PacketParser *parser, F411_PacketCallback callback);
int F411_PacketParser_Feed(F411_PacketParser *parser, uint8_t byte);
uint8_t F411_Packet_CRC8(const uint8_t *data, size_t length);
size_t F411_Packet_Build(uint8_t *out, size_t out_capacity, const uint8_t *payload, uint8_t length);

#endif
