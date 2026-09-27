
#include "f411_debug.h"
#include "f411_uart2.h"

void F411_Debug_PrintU32(uint32_t value)
{
    char buf[11];
    uint32_t i = 10U;
    buf[i] = '\0';
    do {
        buf[--i] = (char)('0' + (value % 10U));
        value /= 10U;
    } while (value != 0U);
    F411_USART2_SendString(&buf[i]);
}

void F411_Debug_PrintI32(int32_t value)
{
    if (value < 0)
    {
        F411_USART2_SendByte('-');
        F411_Debug_PrintU32((uint32_t)(-(int64_t)value));
    }
    else
    {
        F411_Debug_PrintU32((uint32_t)value);
    }
}

void F411_Debug_PrintHex32(uint32_t value)
{
    static const char digits[] = "0123456789ABCDEF";
    F411_USART2_SendString("0x");
    for (int8_t shift = 28; shift >= 0; shift -= 4)
        F411_USART2_SendByte((uint8_t)digits[(value >> shift) & 0xFU]);
}

void F411_Debug_PrintLine(const char *text)
{
    F411_USART2_SendString(text);
    F411_USART2_SendString("\r\n");
}
