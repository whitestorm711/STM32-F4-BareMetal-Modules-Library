
#include "f411_ringbuf.h"

void F411_RingBuffer_Init(F411_RingBuffer *rb, uint8_t *storage, uint16_t size)
{
    if (rb == NULL) return;
    rb->data = storage;
    rb->size = size;
    rb->head = 0U;
    rb->tail = 0U;
}

static uint16_t next_index(uint16_t index, uint16_t size)
{
    index++;
    if (index >= size) index = 0U;
    return index;
}

int F411_RingBuffer_Push(F411_RingBuffer *rb, uint8_t byte)
{
    uint16_t next;
    if (rb == NULL || rb->data == NULL || rb->size < 2U) return -2;

    next = next_index(rb->head, rb->size);
    if (next == rb->tail) return -1;

    rb->data[rb->head] = byte;
    rb->head = next;
    return 0;
}

int F411_RingBuffer_Pop(F411_RingBuffer *rb, uint8_t *byte)
{
    if (rb == NULL || rb->data == NULL || byte == NULL || rb->size < 2U) return -2;
    if (rb->head == rb->tail) return -1;

    *byte = rb->data[rb->tail];
    rb->tail = next_index(rb->tail, rb->size);
    return 0;
}

uint16_t F411_RingBuffer_Available(const F411_RingBuffer *rb)
{
    if (rb == NULL || rb->size == 0U) return 0U;
    if (rb->head >= rb->tail) return rb->head - rb->tail;
    return (uint16_t)(rb->size - rb->tail + rb->head);
}

uint16_t F411_RingBuffer_Free(const F411_RingBuffer *rb)
{
    if (rb == NULL || rb->size < 2U) return 0U;
    return (uint16_t)(rb->size - 1U - F411_RingBuffer_Available(rb));
}

void F411_RingBuffer_Clear(F411_RingBuffer *rb)
{
    if (rb == NULL) return;
    rb->head = 0U;
    rb->tail = 0U;
}
