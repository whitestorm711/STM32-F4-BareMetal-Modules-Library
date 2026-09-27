
#include "f411_uart2_dma.h"
#include "f411_rcc.h"

static void dma1_stream5_clear_flags(void)
{
    DMA1->HIFCR = (1U << 6) | (1U << 8) | (1U << 9) | (1U << 10) | (1U << 11);
}

static void dma1_stream6_clear_flags(void)
{
    DMA1->HIFCR = (1U << 16) | (1U << 18) | (1U << 19) | (1U << 20) | (1U << 21);
}

int F411_USART2_DMA_RX_Start(uint8_t *buffer, uint16_t length)
{
    if (buffer == NULL || length == 0U) return -1;
    F411_RCC_EnableDMA1();
    USART2->CR3 &= ~(1U << 6); /* stop DMA requests while reconfiguring */

    DMA1_Stream5->CR &= ~(1U << 0);
    while ((DMA1_Stream5->CR & (1U << 0)) != 0U) {}
    dma1_stream5_clear_flags();
    DMA1_Stream5->FCR = 0U;
    DMA1_Stream5->PAR = (uint32_t)(uintptr_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)(uintptr_t)buffer;
    DMA1_Stream5->M1AR = 0U;
    DMA1_Stream5->NDTR = length;

    DMA1_Stream5->CR = 0U;
    DMA1_Stream5->CR |= (4U << 25); /* Channel 4 */
    DMA1_Stream5->CR |= (1U << 8);  /* circular */
    DMA1_Stream5->CR |= (1U << 10); /* memory increment */
    DMA1_Stream5->CR |= (2U << 16); /* high priority */
    /* DIR=00, PSIZE=00, MSIZE=00: peripheral->memory, 8-bit */

    USART2->CR3 |= (1U << 6); /* DMAR */
    DMA1_Stream5->CR |= (1U << 0);
    return 0;
}

void F411_USART2_DMA_RX_Stop(void)
{
    USART2->CR3 &= ~(1U << 6);
    DMA1_Stream5->CR &= ~(1U << 0);
    while ((DMA1_Stream5->CR & (1U << 0)) != 0U) {}
}

uint16_t F411_USART2_DMA_RX_Position(uint16_t buffer_length)
{
    uint32_t remaining;
    if (buffer_length == 0U) return 0U;
    remaining = DMA1_Stream5->NDTR;
    return (uint16_t)((buffer_length - (remaining % buffer_length)) % buffer_length);
}

int F411_USART2_DMA_TX_Start(const uint8_t *buffer, uint16_t length)
{
    if (buffer == NULL || length == 0U) return -1;
    F411_RCC_EnableDMA1();
    if ((DMA1_Stream6->CR & (1U << 0)) != 0U) return -2;
    USART2->CR3 &= ~(1U << 7); /* stop DMA requests while reconfiguring */

    dma1_stream6_clear_flags();
    DMA1_Stream6->CR = 0U;
    DMA1_Stream6->FCR = 0U;
    DMA1_Stream6->PAR = (uint32_t)(uintptr_t)&USART2->DR;
    DMA1_Stream6->M0AR = (uint32_t)(uintptr_t)buffer;
    DMA1_Stream6->M1AR = 0U;
    DMA1_Stream6->NDTR = length;

    DMA1_Stream6->CR |= (4U << 25); /* Channel 4 */
    DMA1_Stream6->CR |= (1U << 6);  /* DIR=01, memory->peripheral */
    DMA1_Stream6->CR |= (1U << 10); /* MINC */
    DMA1_Stream6->CR |= (2U << 16); /* high priority */

    USART2->CR3 |= (1U << 7); /* DMAT */
    DMA1_Stream6->CR |= (1U << 0);
    return 0;
}

uint8_t F411_USART2_DMA_TX_Busy(void)
{
    return (uint8_t)((DMA1_Stream6->CR >> 0) & 1U);
}
