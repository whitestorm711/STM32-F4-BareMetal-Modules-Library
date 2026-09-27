# Module status

## Stable / directly aligned with the current learning labs

- `f411_rcc` — peripheral clocks + HSI + APB timer clock helper
- `f411_gpio` — GPIO mode/pull/speed/AF + atomic set/reset
- `f411_systick` / `f411_delay` — 1 ms time base
- `f411_uart2` — USART2 polling + RX interrupt callback
- `f411_ringbuf` — byte ring buffer
- `f411_packet` — small framed packet parser/builder + CRC8
- `f411_tim2` — periodic TIM2 setup + update interrupt
- `f411_pwm_tim2` — TIM2 CH1..CH4 PWM helper
- `f411_exti` — EXTI line setup + callback dispatch

## Practical reference modules

- `f411_adc1` — single conversion ADC1 helper
- `f411_spi1` — blocking SPI1 master
- `f411_i2c1` — blocking I2C1 master + 8-bit register access
- `f411_crc` — hardware CRC32 on words
- `f411_iwdg` — independent watchdog setup/feed
- `f411_reset` — reset-cause flags + software reset
- `f411_uart2_dma` — USART2 DMA circular RX + TX
- `f411_motor` — generic signed motor output
- `f411_drv3wheel` — 3-wheel motor abstraction + user-supplied Q15 mixer
- `f411_filter` — fixed-point controller-support helpers
- `f411_debug` — small UART2 debug print helpers

## Advanced reference

- `f411_tim1_3phase` — TIM1 complementary 3-phase PWM + dead-time

Advanced code is intentionally kept separate in naming and should be bench-tested
with the exact clock tree, package pin mapping, gate driver, and power stage.
