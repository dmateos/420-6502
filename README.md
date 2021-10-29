# 420-6502
Custom built 6502 based computer + system interface controller

![Board pic](/pic.png)

## Hardware Components
* W65C02S 8-bit Microprocessor
* UM61256FK-15 32KB RAM 
* Arduino Mega 2560 AVR 
* Breadboard
* Shitloads of wires and LEDS

## Software Components
* 420-6502 AVR controller (Controller system)
  * RAM Test on POST 
  * Inject Kernel or NOP test program into RAM 
  * Sets the CPU reset vectors
  * Starts the CPU
  * Provides a clock for the CPU
  * Provides special high memory "registers" and "features"
  * Provides a serial debugging interface to get output over USB
* 420-6502 OS 
  * Basic Kernel
  * Tests the CPU stack is working 
  * Tests basic logic and RAM reading/writting is working
  * Writes to special high mem registers which get propogated over the USB serial debugging interface
