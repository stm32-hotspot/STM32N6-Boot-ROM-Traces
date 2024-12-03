# How to install a trusted application on STM32N6 MCUs

> [!NOTE]
> This repository contains the supporting material for the Wiki article [How to install a trusted application on STM32N6 MCUs](https://wiki.st.com/stm32mcu/wiki/Security:How_to_install_a_trusted_application_on_STM32N6_MCUs).

## Description

The purpose of this project is to explain:
* How to sign/encrypt a basic application (FSBL) loaded by BootRom
* Which OTPs are involved in BootRom authenticity and integrity checks
* How to obtain BootRom traces
* How to set the device to Secured-locked state

## Content

This project consists of two folders:
* BootRom_Traces: Contains scripts for performing security operations on an STM32N6 MCU
* Template_BootRom_traces: A template based on those delivered in the STM32CubeN6 firmware, with added capability to obtain BootRom traces. This template is intended for STM32N657X0H3QU devices.

## Supported toolchains

This project has been created using version **9.40.1** of IAR's EWARM IDE.

## Questions & Feedback

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
