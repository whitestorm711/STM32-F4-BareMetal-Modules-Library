
#include "f411_i2c1.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

static int wait_sr1(uint32_t mask, uint32_t timeout)
{
    while ((I2C1->SR1 & mask) == 0U)
    {
        if ((I2C1->SR1 & ((1U << 8) | (1U << 9) | (1U << 10) | (1U << 11))) != 0U)
            return -2;
        if (timeout == 0U) return -1;
        timeout--;
    }
    return 0;
}

static int wait_not_busy(uint32_t timeout)
{
    while ((I2C1->SR2 & (1U << 1)) != 0U)
    {
        if (timeout == 0U) return -1;
        timeout--;
    }
    return 0;
}

static void clear_addr(void)
{
    volatile uint32_t sr1 = I2C1->SR1;
    volatile uint32_t sr2 = I2C1->SR2;
    (void)sr1;
    (void)sr2;
}

static void send_start(void)
{
    I2C1->CR1 |= (1U << 8);
}

static void send_stop(void)
{
    I2C1->CR1 |= (1U << 9);
}

int F411_I2C1_Init(uint32_t pclk1_hz, uint32_t scl_hz)
{
    uint32_t freq_mhz;
    uint32_t ccr;
    if (pclk1_hz < 2000000U || pclk1_hz > 50000000U || scl_hz == 0U || scl_hz > 100000U) return -1;

    F411_RCC_EnableI2C1();
    F411_GPIO_ConfigAF(GPIOB, 6U, 4U, 3U, 1U); /* PB6 SCL */
    F411_GPIO_ConfigAF(GPIOB, 7U, 4U, 3U, 1U); /* PB7 SDA */

    I2C1->CR1 = (1U << 15); /* software reset */
    I2C1->CR1 = 0U;

    freq_mhz = pclk1_hz / 1000000U;
    I2C1->CR2 = (freq_mhz & 0x3FU);
    I2C1->OAR1 = (1U << 14); /* required fixed bit */
    ccr = pclk1_hz / (2U * scl_hz);
    if (ccr < 4U) ccr = 4U;
    I2C1->CCR = ccr & 0x0FFFU;
    I2C1->TRISE = (freq_mhz + 1U) & 0x3FU;
    I2C1->CR1 = (1U << 10) | (1U << 0); /* ACK + PE */
    return 0;
}

int F411_I2C1_Write7(uint8_t address7, const uint8_t *data, size_t length, uint32_t timeout_loops)
{
    if (address7 > 0x7FU || (data == NULL && length != 0U)) return -4;
    if (wait_not_busy(timeout_loops) != 0) return -1;

    send_start();
    if (wait_sr1((1U << 0), timeout_loops) != 0) return -2; /* SB */
    I2C1->DR = ((uint32_t)address7 << 1);
    if (wait_sr1((1U << 1), timeout_loops) != 0) return -3; /* ADDR */
    clear_addr();

    for (size_t i = 0U; i < length; i++)
    {
        if (wait_sr1((1U << 7), timeout_loops) != 0) { send_stop(); return -4; }
        I2C1->DR = data[i];
    }
    if (wait_sr1((1U << 2), timeout_loops) != 0) { send_stop(); return -5; } /* BTF */
    send_stop();
    return 0;
}

int F411_I2C1_Read7(uint8_t address7, uint8_t *data, size_t length, uint32_t timeout_loops)
{
    size_t i = 0U;
    if (address7 > 0x7FU || data == NULL || length == 0U) return -4;
    if (wait_not_busy(timeout_loops) != 0) return -1;

    send_start();
    if (wait_sr1((1U << 0), timeout_loops) != 0) return -2;
    I2C1->DR = ((uint32_t)address7 << 1) | 1U;
    if (wait_sr1((1U << 1), timeout_loops) != 0) return -3;

    if (length == 1U)
    {
        I2C1->CR1 &= ~(1U << 10); /* ACK off */
        clear_addr();
        send_stop();
        if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -5; }
        data[0] = (uint8_t)I2C1->DR;
        I2C1->CR1 |= (1U << 10);
        return 0;
    }

    if (length == 2U)
    {
        I2C1->CR1 |= (1U << 11);  /* POS */
        I2C1->CR1 &= ~(1U << 10);  /* ACK off */
        clear_addr();
        if (wait_sr1((1U << 2), timeout_loops) != 0) { I2C1->CR1 &= ~(1U << 11); I2C1->CR1 |= (1U << 10); return -5; }
        send_stop();
        data[0] = (uint8_t)I2C1->DR;
        data[1] = (uint8_t)I2C1->DR;
        I2C1->CR1 &= ~(1U << 11);
        I2C1->CR1 |= (1U << 10);
        return 0;
    }

    /* Three or more bytes. */
    clear_addr();
    while (length - i > 3U)
    {
        if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -5; }
        data[i++] = (uint8_t)I2C1->DR;
    }

    if (wait_sr1((1U << 2), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -6; } /* BTF */
    I2C1->CR1 &= ~(1U << 10); /* NACK next */
    data[i++] = (uint8_t)I2C1->DR;
    send_stop();
    data[i++] = (uint8_t)I2C1->DR;
    if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -7; }
    data[i++] = (uint8_t)I2C1->DR;
    I2C1->CR1 |= (1U << 10);
    return 0;
}

int F411_I2C1_MemWrite8(uint8_t address7, uint8_t reg, const uint8_t *data, size_t length, uint32_t timeout_loops)
{
    uint8_t buf[1 + 32U];
    if (address7 > 0x7FU || length > 32U || (data == NULL && length != 0U)) return -6;
    buf[0] = reg;
    for (size_t i = 0U; i < length; i++) buf[1U + i] = data[i];
    return F411_I2C1_Write7(address7, buf, length + 1U, timeout_loops);
}

int F411_I2C1_MemRead8(uint8_t address7, uint8_t reg, uint8_t *data, size_t length, uint32_t timeout_loops)
{
    if (address7 > 0x7FU || data == NULL || length == 0U) return -6;
    if (wait_not_busy(timeout_loops) != 0) return -1;

    send_start();
    if (wait_sr1((1U << 0), timeout_loops) != 0) return -2;
    I2C1->DR = ((uint32_t)address7 << 1);
    if (wait_sr1((1U << 1), timeout_loops) != 0) return -3;
    clear_addr();
    if (wait_sr1((1U << 7), timeout_loops) != 0) { send_stop(); return -4; }
    I2C1->DR = reg;
    if (wait_sr1((1U << 2), timeout_loops) != 0) { send_stop(); return -5; }

    send_start();
    if (wait_sr1((1U << 0), timeout_loops) != 0) return -2;
    I2C1->DR = ((uint32_t)address7 << 1) | 1U;
    if (wait_sr1((1U << 1), timeout_loops) != 0) return -3;

    /* Reuse the read sequence by performing the sequence inline. */
    if (length == 1U)
    {
        I2C1->CR1 &= ~(1U << 10);
        clear_addr();
        send_stop();
        if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -7; }
        data[0] = (uint8_t)I2C1->DR;
        I2C1->CR1 |= (1U << 10);
        return 0;
    }

    if (length == 2U)
    {
        I2C1->CR1 |= (1U << 11);
        I2C1->CR1 &= ~(1U << 10);
        clear_addr();
        if (wait_sr1((1U << 2), timeout_loops) != 0) { I2C1->CR1 &= ~(1U << 11); I2C1->CR1 |= (1U << 10); return -8; }
        send_stop();
        data[0] = (uint8_t)I2C1->DR;
        data[1] = (uint8_t)I2C1->DR;
        I2C1->CR1 &= ~(1U << 11);
        I2C1->CR1 |= (1U << 10);
        return 0;
    }

    clear_addr();
    size_t i = 0U;
    while (length - i > 3U)
    {
        if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -9; }
        data[i++] = (uint8_t)I2C1->DR;
    }
    if (wait_sr1((1U << 2), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -10; }
    I2C1->CR1 &= ~(1U << 10);
    data[i++] = (uint8_t)I2C1->DR;
    send_stop();
    data[i++] = (uint8_t)I2C1->DR;
    if (wait_sr1((1U << 6), timeout_loops) != 0) { I2C1->CR1 |= (1U << 10); return -11; }
    data[i++] = (uint8_t)I2C1->DR;
    I2C1->CR1 |= (1U << 10);
    return 0;
}
