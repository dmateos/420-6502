# 420-6502
How much can we abuse a WD 6502 with a Arduino Mega? 

## Components
* WDC 6502 CPU
* UM61256FK-15 32KB RAM 
* Arduino Mega 2560

## What it does
* Checks every RAM address for read/write correctness
* Starts the CPU 
* Provides the CPU clock 
* Injects a program into ram and controls the CPU start vectors
* Intercepts requests for high memory above the ram (0x7FFF)

## What it might do 
* Provide a set of graphics/io registers
* Provide a ACPI like interface via RAM/Address mappings
