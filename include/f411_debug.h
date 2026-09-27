
#ifndef F411_DEBUG_H
#define F411_DEBUG_H

#include "f411_platform.h"

void F411_Debug_PrintU32(uint32_t value);
void F411_Debug_PrintI32(int32_t value);
void F411_Debug_PrintHex32(uint32_t value);
void F411_Debug_PrintLine(const char *text);

#endif
