# 420-6502
Custom built 6502 based computer

System interface components

## Hardware Components
* WDC 65C02 CPU
* UM61256FK-15 32KB RAM 
* Arduino Mega 2560
* Breadboard
* Shitloads of wires and LEDS

## Software Components
* 420-6502 AVR controller (BIOS)
* 420-6502 OS 

## What it does
* Checks every RAM address for read/write correctness
* Starts the CPU 
* Provides the CPU clock 
* Injects a program into ram and controls the CPU start vectors
* Intercepts requests for high memory above the ram (0x7FFF)
* Provides a serial debugging interface in/out of the AVR

## What it might do 
* Provide a set of graphics/io registers
* Provide a ACPI like interface via RAM/Address mappings
