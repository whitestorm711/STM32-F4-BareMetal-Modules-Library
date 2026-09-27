
# Using the library in CubeIDE

1. Keep the CubeIDE project selected for the correct F411 part/package. ST CMSIS uses the device macro `STM32F411xE` for the STM32F411xC/xE family.
2. Add `include/` to the compiler include paths.
3. Add the `.c` files you need from `src/` to the build.
4. Keep the normal CMSIS device header/startup files supplied by your CubeIDE project.
5. Do not include the STM32 HAL peripheral initialization for the same peripheral if this library is going to configure it.
6. If Cube generates `SysTick_Handler()` or `USART2_IRQHandler()`, remove/rename the duplicate handlers when using the library versions.

The library does not ship the ST CMSIS device header itself. Use the one installed with your STM32CubeF4/CubeIDE project.

## IRQ ownership

The modules that install vector handlers are intentionally simple and own those handlers:

- `f411_systick.c` provides `SysTick_Handler()`
- `f411_uart2.c` provides `USART2_IRQHandler()`
- `f411_tim2.c` provides `TIM2_IRQHandler()`
- `f411_exti.c` provides the EXTI handlers

Do not compile duplicate application handlers for the same vector. For larger projects,
replace the module handler with your own dispatcher if you need to combine multiple
frameworks on the same interrupt.
