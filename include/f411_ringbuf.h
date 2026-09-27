
#ifndef F411_RINGBUF_H
#define F411_RINGBUF_H

#include "f411_platform.h"

typedef struct
{
    uint8_t *data;
    uint16_t size;
    volatile uint16_t head;
    volatile uint16_t tail;
} F411_RingBuffer;

void F411_RingBuffer_Init(F411_RingBuffer *rb, uint8_t *storage, uint16_t size);
int F411_RingBuffer_Push(F411_RingBuffer *rb, uint8_t byte);
int F411_RingBuffer_Pop(F411_RingBuffer *rb, uint8_t *byte);
uint16_t F411_RingBuffer_Available(const F411_RingBuffer *rb);
uint16_t F411_RingBuffer_Free(const F411_RingBuffer *rb);
void F411_RingBuffer_Clear(F411_RingBuffer *rb);

#endif
