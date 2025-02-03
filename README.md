# How to install a trusted application on STM32N6 MCUs

> [!NOTE]
> This repository contains the supporting material for the Wiki article [How to install a trusted application on STM32N6 MCUs](https://wiki.st.com/stm32mcu/wiki/Security:How_to_install_a_trusted_application_on_STM32N6_MCUs).

## Description

The purpose of this project is to explain:
* How to sign/encrypt a basic application (FSBL) loaded by BootRom,
* Which OTPs are involved in BootRom authenticity and integrity checks,
* How to obtain BootRom traces,
* How to set the device to secured-locked state,
* How to load a new FSBL whith the device in the secured-locked state.

## Content

This project consists of three folders:
* The `./BootRom_Traces` folder, containing scripts for performing security operations on an STM32N6 MCU.
* The `./Template_BootRom_traces` folder, a template based on those delivered in the STM32CubeN6 firmware, with added capability to obtain BootRom traces. This template is intended for STM32N657X0H3QU devices.
* The `./Load_new_FSBL_locked_state` folder, containing the material to load a new FSBL is secured-locked state.

## Supported toolchains

This project has been created using version **9.40.1** of IAR's EWARM IDE.

## Questions & Feedback

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
