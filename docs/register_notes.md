# Register notes / assumptions

This repository targets STM32F411 using ST's RM0383 reference manual and the F411
datasheets. Register names and peripheral behavior should be checked against the
exact F411 package/device variant used in a project.

## Important pin mappings used by examples/modules

- PA0 AF1: TIM2_CH1
- PA1 AF1: TIM2_CH2
- PA2 AF1: TIM2_CH3 / AF7 USART2_TX
- PA3 AF1: TIM2_CH4 / AF7 USART2_RX
- PA5/PA6/PA7 AF5: SPI1 SCK/MISO/MOSI
- PB6/PB7 AF4: I2C1 SCL/SDA
- PA8/PA9/PA10 AF1: TIM1 CH1/CH2/CH3
- PB13/PB14/PB15 AF1: TIM1 CH1N/CH2N/CH3N

Always check the exact package datasheet before committing pins on a PCB.

## USART2

`USART2` is on APB1. The DMA module uses:

- DMA1 Stream5 Channel4 = USART2_RX
- DMA1 Stream6 Channel4 = USART2_TX

The DMA module intentionally uses direct mode and byte-sized transfers for a
simple circular RX / non-interrupting TX starting point.

## TIM2

TIM2 is a 32-bit general-purpose timer on STM32F411. The PWM helper configures
PWM mode 1 and preload on the selected output-compare channel.

## TIM1 dead-time

RM0383 encodes BDTR.DTG non-linearly. The helper in `f411_tim1_3phase.c`
brute-force searches all 256 codes and picks the closest code for the requested
nanoseconds. This avoids accidentally treating DTG as a linear 0..255 scale.

For the six-output reference driver:

- PA8/PA9/PA10 = CH1/CH2/CH3
- PB13/PB14/PB15 = CH1N/CH2N/CH3N
- BDTR.DTG = bits 7:0
- BDTR.OSSI = bit 10
- BDTR.OSSR = bit 11
- BDTR.BKE = bit 12
- BDTR.BKP = bit 13
- BDTR.AOE = bit 14
- BDTR.MOE = bit 15

No break-input protection is enabled by the default reference driver.

## RCC timer clocks

The helper `F411_RCC_APB1TimerClockHz()` / `F411_RCC_APB2TimerClockHz()`
accounts for the APB prescaler and the F411 `TIMPRE` option in `RCC->DCKCFGR`
when calculating the timer kernel clock.

## Watchdog / reset

`f411_iwdg` enables LSI, unlocks PR/RLR, waits for synchronization, then starts
the independent watchdog with the `0xCCCC` key. Once started, the IWDG cannot be
stopped by software; it runs until reset.

`f411_reset` exposes RCC reset-cause flags and the Cortex-M4 `SYSRESETREQ`
software-reset mechanism.

## STM32F411-specific reference notes

This port is for STM32F411xC/xE and is based on ST RM0383 and the F411 datasheet.
The F411 device family includes a Cortex-M4F up to 100 MHz, up to 512 KB Flash,
128 KB SRAM, USB OTG FS, and additional communications/timer resources compared
with the F401. The modules in this repository intentionally keep the same daily-use
surface as the F401 library rather than expanding the API for every F411-specific
peripheral.

For USART2 DMA, RM0383 maps USART2_RX to DMA1 Stream 5 Channel 4 and USART2_TX to
DMA1 Stream 6 Channel 4; these are the mappings used by `f411_uart2_dma`.
