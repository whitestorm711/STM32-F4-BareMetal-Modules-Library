# STM32F411 Bare-Metal Daily Library

A small, composable register-level utility library for STM32F411 projects.
The intent is simple: copy the modules you actually need into a project and
start using them immediately without rebuilding the same low-level peripheral
setup every time.

## Rules this repository follows

- No STM32 HAL peripheral calls.
- No STM32 LL peripheral calls.
- Peripheral configuration is performed through CMSIS register structures
  (`GPIOA->...`, `RCC->...`, `TIM2->...`, etc.).
- Register bitfields are written with explicit shifts/masks such as `(1U << n)`;
  STM32 CMSIS bit-name macros are intentionally not used.
- The exact F411 device header is selected through `STM32F411xC` or
  `STM32F411xE`.
- The library is small and composable instead of being a monolithic framework.

## Version

`0.1.0`

## Contents

| Module | Purpose |
|---|---|
| `f411_rcc` | Peripheral clocks, HSI 16 MHz, APB timer-clock helper |
| `f411_gpio` | GPIO modes, pulls, AF, atomic set/reset, read/toggle |
| `f411_systick` | 1 ms time base |
| `f411_delay` | Delay wrapper |
| `f411_uart2` | USART2 TX/RX, strings, RX interrupt callback |
| `f411_ringbuf` | ISR-friendly single-producer/single-consumer byte ring buffer |
| `f411_packet` | Framed packet builder/parser with CRC8 |
| `f411_tim2` | Basic periodic TIM2 setup and update interrupt |
| `f411_pwm_tim2` | TIM2 CH1..CH4 PWM helper |
| `f411_exti` | EXTI line configuration and callback dispatch |
| `f411_adc1` | ADC1 single-conversion reads |
| `f411_spi1` | SPI1 blocking master transfers |
| `f411_i2c1` | I2C1 standard-mode blocking master + 8-bit register access |
| `f411_crc` | Hardware CRC32 on 32-bit words |
| `f411_iwdg` | Independent watchdog setup/feed |
| `f411_reset` | Reset-cause flags + software reset |
| `f411_uart2_dma` | USART2 DMA RX circular + TX helpers |
| `f411_motor` | Generic signed-speed motor output on a timer CCR + DIR |
| `f411_drv3wheel` | Three-motor driver + configurable Q15 kinematics matrix |
| `f411_tim1_3phase` | TIM1 complementary 3-phase PWM + dead-time reference |
| `f411_filter` | Fixed-point EMA, moving average, clamp |
| `f411_debug` | Lightweight UART2 integer/hex logging |

## Repository layout

```text
stm32f411-baremetal-library/
  include/          paired public headers
  src/              paired implementations
  examples/         small starting points
  docs/             register and usage notes
  README.md
  CHANGELOG.md
  VERSION
```

## Getting started

Add `include/` to the compiler include path and compile the `.c` files for the
modules you use from `src/`.

A CubeIDE project may keep the CMSIS startup/system files supplied by CubeIDE,
but do not let generated HAL/LL initialization rewrite the same peripheral
registers after these modules have configured them.

For one-file convenience, `f411.h` includes every module header.

## Device-clock rule

The library does not guess the clock tree. Pass the actual peripheral/core
clock frequency to helpers that need it. This is deliberate because APB timer
clocks can differ from the raw APB clocks when prescalers and `TIMPRE` are used.

For a simple 16 MHz HSI-only project:

```c
#include "f411_rcc.h"
#include "f411_systick.h"

int main(void)
{
    F411_RCC_UseHSI16();
    F411_SysTick_Init(16000000U);

    while (1) {}
}
```

For Cube-generated clocks, call the peripheral modules after
`SystemClock_Config()` and pass the resulting clock values.

## UART + packet starting point

```c
static F411_PacketParser parser;
static volatile uint8_t got_packet;

static void on_packet(const uint8_t *payload, uint8_t length)
{
    (void)payload;
    (void)length;
    got_packet = 1U;
}

static void on_rx_byte(uint8_t byte)
{
    (void)F411_PacketParser_Feed(&parser, byte);
}

int main(void)
{
    F411_RCC_UseHSI16();
    F411_USART2_Init(16000000U, 115200U);
    F411_PacketParser_Init(&parser, on_packet);
    F411_USART2_SetRxCallback(on_rx_byte);
    F411_USART2_EnableRxInterrupt();

    while (1)
    {
        if (got_packet)
        {
            got_packet = 0U;
            /* Process the payload outside the ISR. */
        }
    }
}
```

`f411_uart2.c` provides the `USART2_IRQHandler()`. Do not define a second
handler in the application when using that module. The packet parser callback
can execute from the USART interrupt context, so keep that callback tiny; use
a ring buffer or copy the payload into an application queue for heavier work.

## 3-wheel driver

The wheel layer deliberately does not assume an L298, BTS7960, DRV8833, or any
other motor-driver IC. Each `F411_Motor` gives the library a timer/CCR for PWM,
a digital direction pin, and an optional enable pin.

`F411_Drive3Wheel_SetWheelSpeeds()` is the direct interface. The vector API
uses a user-supplied 3x3 Q15 matrix because wheel angle/sign conventions are
mechanical decisions, not universal STM32 settings.

## TIM1 / ESC note

`f411_tim1_3phase` is the advanced reference module. It targets:

- PA8/PA9/PA10 = CH1/CH2/CH3
- PB13/PB14/PB15 = CH1N/CH2N/CH3N

It demonstrates complementary PWM, preload, main-output enable, and the
non-linear BDTR dead-time encoding from RM0383. Use the module with a real gate
driver/power stage only after checking polarity, bootstrap requirements, dead
time, break behavior, and the external hardware on the bench.

## Source of truth

Peripheral definitions and behavior should be checked against ST RM0383 for
STM32F411 and the datasheet for the exact package/device variant.

## F411 scope

The register/peripheral behavior for this port should be checked against ST RM0383 and the exact STM32F411 datasheet. ST documents the F411xC/xE family as using the STM32F411xE CMSIS device header selection.


This repository is an intentionally API-matched STM32F411 daily library, mirrored from the STM32F401 module set.
It targets STM32F411xC/xE and the ST CMSIS device header `stm32f411xe.h`.
The code remains register-level and bit-shift based, so the corresponding F401 and F411
modules can be moved between projects with minimal application-level changes when the
selected pins, clock tree, and peripheral instances are the same.

The F411 supports a higher maximum CPU clock and has additional peripherals, but this
first port keeps the same small, reusable module set so the two repositories remain
interchangeable for the workflows covered by the library.
