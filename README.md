ATtiny AVR-1 Peripheral Drivers – Personal Driver Set

This repository contains my personal, evolving set of peripheral drivers for ATtiny AVR-1 series microcontrollers, developed and tested primarily on ATtiny817 hardware.

The intent of this repository is not to provide a generic framework or HAL, but to serve as a clean, understandable, register-level driver base that I can reuse, study, modify, and trust while building real embedded systems—especially sensor-centric and timing-sensitive applications.

All drivers are written in bare-metal C, with explicit register configuration and minimal abstraction.

Repository Structure Overview

The project is structured to clearly separate application logic from peripheral drivers, allowing drivers to be reused across different projects without entangling application-specific code.

Application/
    main.c        → Application logic and system integration

Drivers/
    AC/           → Analog Comparator
    ADC/          → Analog-to-Digital Converter
    DAC/          → Digital-to-Analog Converter (where applicable)
    EEPROM/       → Non-volatile memory access
    EVSYS/        → Event System configuration
    GPIO/         → Digital I/O abstraction
    TIMERS/       → Software timers, TCA, TCB
    USART/        → UART communication


Each peripheral is implemented as a self-contained module, consisting of:

<peripheral>.c → implementation

<peripheral>.h → interface and configuration macros

Design Philosophy

These drivers are written with the following principles:

Register-level clarity
Every configuration step maps directly to the datasheet. There are no hidden side effects or opaque helper layers.

Predictable behavior
Timing, clock dependencies, and peripheral interactions are explicit and intentional.

Minimal coupling
Drivers do not assume how the application is structured. Initialization and usage are controlled explicitly from main.c.

Readability over cleverness
Code is written so that future-me can understand why something works, not just that it works.

Experimental but disciplined
Drivers may evolve, but working configurations are preserved and not casually broken.

Driver Modules Description
🔹 GPIO

Provides basic digital I/O control with clear direction setting, output control, and pin-level abstraction. Designed to keep pin manipulation readable without hiding the underlying port registers.

🔹 ADC

Implements ADC configuration and sampling with attention to:

Reference selection

Prescaler and timing control

Repeatable sampling behavior

This module is designed to support sensor front-ends (e.g., photodiode + TIA circuits), where noise, timing, and consistency matter.

🔹 AC (Analog Comparator)

Provides access to the analog comparator for threshold-based detection and fast analog decisions without ADC involvement. Intended for low-latency or low-power comparisons.

🔹 DAC

Where supported, provides DAC output configuration for generating analog voltages or test signals, typically for calibration or debugging purposes.

🔹 EEPROM

Handles non-volatile storage with controlled read/write access. Intended for:

Calibration data

Configuration parameters

Persistent system state

Focus is on correctness and data integrity rather than speed.

🔹 EVSYS (Event System)

Configures internal peripheral-to-peripheral routing without CPU intervention. This module exists to explore and utilize hardware-level synchronization, especially between timers, ADC, and GPIO.

🔹 TIMERS

This folder contains multiple timing-related drivers:

sw_timer
Lightweight software timers built on hardware timers for scheduling non-critical tasks.

TCA_pwm
PWM generation using TCA for higher-resolution or multi-channel outputs.

TCB_pwm
PWM using TCB for simple, deterministic waveform generation or modulation.

Timer drivers are written with explicit clock assumptions and are suitable for signal generation, modulation, and control outputs.

🔹 USART

Implements UART communication primarily for:

Debug output

Calibration interfaces

Runtime inspection of internal variables

The driver favors simplicity and reliability over feature completeness.

Application Layer

Application/main.c acts as the integration point, where:

System clock is configured

Drivers are initialized

Peripheral interactions are coordinated

This separation ensures that drivers remain reusable and that application logic stays readable.

Intended Usage

This repository is meant to be:

A personal driver library

A reference for AVR-1 peripheral behavior

A safe base for experimenting with new configurations

A foundation for sensor and control projects

Drivers are reused selectively; not everything here is meant to be enabled at once.

Non-Goals

This is not a drop-in HAL

This is not Arduino-compatible

This is not optimized for smallest code size

This is not intended to hide hardware details

Understanding the hardware is the goal.
