#include "f411_packet.h"

/* Wire format:
 * [0]   0xA5 start byte
 * [1]   payload length (0..64)
 * [2..] payload
 * [last] CRC8 over [length + payload], polynomial 0x07, init 0x00
 */

static uint8_t crc8_update(uint8_t crc, uint8_t data)
{
    crc ^= data;
    for (uint8_t b = 0U; b < 8U; b++)
    {
        if ((crc & 0x80U) != 0U) crc = (uint8_t)((crc << 1) ^ 0x07U);
        else crc <<= 1;
    }
    return crc;
}

uint8_t F411_Packet_CRC8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0U;
    if (data == NULL) return 0U;
    for (size_t i = 0U; i < length; i++) crc = crc8_update(crc, data[i]);
    return crc;
}

void F411_PacketParser_Init(F411_PacketParser *parser, F411_PacketCallback callback)
{
    if (parser == NULL) return;
    parser->state = 0U;
    parser->length = 0U;
    parser->index = 0U;
    parser->callback = callback;
}

int F411_PacketParser_Feed(F411_PacketParser *parser, uint8_t byte)
{
    uint8_t crc;
    if (parser == NULL) return -4;

    if (parser->state == 0U)
    {
        if (byte == 0xA5U) parser->state = 1U;
        return 0;
    }

    if (parser->state == 1U)
    {
        parser->length = byte;
        parser->index = 0U;
        if (parser->length > F411_PACKET_MAX_PAYLOAD)
        {
            parser->state = 0U;
            return -2;
        }
        parser->state = (parser->length == 0U) ? 3U : 2U;
        return 0;
    }

    if (parser->state == 2U)
    {
        parser->payload[parser->index++] = byte;
        if (parser->index >= parser->length) parser->state = 3U;
        return 0;
    }

    /* state 3 = CRC byte */
    crc = crc8_update(0U, parser->length);
    for (uint8_t i = 0U; i < parser->length; i++) crc = crc8_update(crc, parser->payload[i]);

    parser->state = 0U;
    if (crc != byte) return -3;
    if (parser->callback != NULL) parser->callback(parser->payload, parser->length);
    return 1;
}

size_t F411_Packet_Build(uint8_t *out, size_t out_capacity, const uint8_t *payload, uint8_t length)
{
    uint8_t crc;
    size_t needed = (size_t)length + 3U;

    if (length > F411_PACKET_MAX_PAYLOAD || out == NULL || payload == NULL || out_capacity < needed) return 0U;

    out[0] = 0xA5U;
    out[1] = length;
    crc = crc8_update(0U, length);
    for (uint8_t i = 0U; i < length; i++)
    {
        out[2U + i] = payload[i];
        crc = crc8_update(crc, payload[i]);
    }
    out[2U + length] = crc;
    return needed;
}
