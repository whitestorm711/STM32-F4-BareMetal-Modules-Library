# Changelog

## 0.1.0

Initial daily-use register-level library for STM32F411.

Includes GPIO/RCC, SysTick/delay, USART2 polling + RX interrupt, ring buffer,
packet framing + CRC8, TIM2 periodic timer, TIM2 PWM, EXTI, ADC1, SPI1, I2C1,
hardware CRC32, IWDG, reset flags/software reset, USART2 DMA, generic motor,
3-wheel drive abstraction, TIM1 complementary 3-phase PWM/dead-time reference,
fixed-point filters, and lightweight UART debugging.

The code targets the STM32F411 CMSIS register structures and deliberately avoids
STM32 HAL/LL peripheral APIs and CMSIS bit-name macros.
