
#include "f411_uart2.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

static F411_UART2_RxCallback g_rx_callback;

int F411_USART2_Init(uint32_t pclk1_hz, uint32_t baud)
{
    if (pclk1_hz == 0U || baud == 0U) return -1;

    F411_RCC_EnableGPIO(GPIOA);
    F411_RCC_EnableUSART2();

    /* PA2 = USART2_TX, PA3 = USART2_RX, AF7. */
    F411_GPIO_ConfigAF(GPIOA, 2U, 7U, 2U, 0U);
    F411_GPIO_ConfigAF(GPIOA, 3U, 7U, 2U, 0U);

    USART2->CR1 = 0U;
    USART2->CR2 = 0U; /* 1 stop bit */
    USART2->CR3 = 0U;

    /* OVER8=0: BRR is the rounded value of fPCLK / baud. */
    {
        uint32_t brr = (pclk1_hz + (baud / 2U)) / baud;
        if (brr == 0U || brr > 0xFFFFU) return -2;
        USART2->BRR = brr;
    }

    USART2->CR1 |= (1U << 3);  /* TE */
    USART2->CR1 |= (1U << 2);  /* RE */
    USART2->CR1 |= (1U << 13); /* UE */
    return 0;
}

void F411_USART2_SendByte(uint8_t byte)
{
    while ((USART2->SR & (1U << 7)) == 0U) {}
    USART2->DR = byte;
}

void F411_USART2_Send(const uint8_t *data, size_t length)
{
    if (data == NULL) return;
    for (size_t i = 0U; i < length; i++) F411_USART2_SendByte(data[i]);
}

void F411_USART2_SendString(const char *text)
{
    if (text == NULL) return;
    while (*text != '\0') F411_USART2_SendByte((uint8_t)*text++);
}

int F411_USART2_ReceiveByte(uint8_t *byte, uint32_t timeout_loops)
{
    if (byte == NULL) return -2;

    while ((USART2->SR & (1U << 5)) == 0U)
    {
        if (timeout_loops == 0U) return -1;
        timeout_loops--;
    }

    *byte = (uint8_t)USART2->DR;
    return 0;
}

void F411_USART2_EnableRxInterrupt(void)
{
    USART2->CR1 |= (1U << 5); /* RXNEIE */
    /* USART2 IRQ = 38 -> NVIC ISER[1], bit 6. */
    NVIC->ISER[1] = (1U << 6);
}

void F411_USART2_DisableRxInterrupt(void)
{
    USART2->CR1 &= ~(1U << 5);
}

void F411_USART2_SetRxCallback(F411_UART2_RxCallback callback)
{
    g_rx_callback = callback;
}

uint8_t F411_USART2_HasRxData(void)
{
    return (uint8_t)((USART2->SR >> 5) & 1U);
}

void USART2_IRQHandler(void)
{
    uint32_t sr = USART2->SR;
    uint32_t clear_dr = sr & ((1U << 4) | (1U << 3) | (1U << 2) | (1U << 1) | (1U << 5));

    /* Reading SR followed by one DR read clears the relevant legacy USART
     * status conditions. Reuse that same DR read for RXNE so an incoming
     * byte is not accidentally consumed twice. */
    if (clear_dr != 0U)
    {
        uint8_t byte = (uint8_t)USART2->DR;
        if ((sr & (1U << 5)) != 0U && g_rx_callback != NULL)
            g_rx_callback(byte);
    }
}
