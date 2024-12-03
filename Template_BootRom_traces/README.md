---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}


## <b>Template Description</b>

This project provides a reference template based on the STM32Cube HAL API that can be used
to build any firmware application. Note that security is enabled by default on Cortex-M55.

- This project is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.  
- The reference template project configures the maximum system clock frequency at 600 MHz.
- The template print via UART the BootRom traces 
- Once the clock is set, the green LED (GPIO PO.01) toggles in an infinite loop with a 200-ms period.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
 

### <b>Keywords</b>

Reference, Template 

### <b>Directory contents</b>

      - Templates/Template_BootRom_traces/FSBL/Src/main.c                         Main program
      - Templates/Template_BootRom_traces/FSBL/Src/stm32n6xx_it.c                 Interrupt handlers
      - Templates/Template_BootRom_traces/FSBL/Inc/main.h                         Main program header file
      - Templates/Template_BootRom_traces/FSBL/Inc/stm32n6570_discovery_conf.h    BSP Configuration file
      - Templates/Template_BootRom_traces/FSBL/Inc/stm32n6xx_hal_conf.h           HAL Configuration file
      - Templates/Template_BootRom_traces/FSBL/Inc/stm32n6xx_it.h                 Interrupt handlers header file
      - Templates/Template_BootRom_traces/FSBL/Inc/partition_stm32n657xx.h        Partition file


### <b>Hardware and Software environment</b>

  - This template runs on STM32N657X0H3QU devices.
  - This template has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:
 
  - launch the script Sign_Load_FW.bat (Projects\STM32N6570-DK\ROT_Provisioning\BootRom_Traces)  to add a header & sign the FSBL and load the binary generated in DK board external Flash at address 0x7000'0000.
  or
  - launch the script Sign_Encrypt_Load_FW.bat (Projects\STM32N6570-DK\ROT_Provisioning\BootRom_Traces) to add a header & sign the FSBL , encrypt the FSBL and load the binary generated in DK board external Flash at address 0x7000'0000 and address 0x7004'0000.

 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


:::
:::


:::
:::

