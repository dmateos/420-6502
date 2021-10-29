# 420-6502
Custom built 6502 based computer + system interface controller

## Hardware Components
* W65C02S 8-bit Microprocessor
* UM61256FK-15 32KB RAM 
* Arduino Mega 2560 AVR 
* Breadboard
* Shitloads of wires and LEDS
* Resistance (in the form of resistors)

## Software Components
* 420-6502 AVR controller (Controller system)
  * Power on self test (write/read all RAM addresses)
  * Inject Kernel or NOP test program into RAM 
  * Sets the CPU reset vectors (0x0200)
  * Starts the CPU
  * Provides a clock for the CPU
  * Provides some crude bus management via the 6502 BE pin
  * Intercept/monitor CPU address and data lines
  * Provides special high memory "registers" and "features"
  * Provides a serial debugging interface to PC over AVR USB
* 420-6502 OS 
  * Basic Kernel
  * Tests the CPU stack is working 
  * Tests basic logic and RAM reading/writting is working
  * Writes to special high mem registers which get propogated over the USB serial debugging interface

## Memory Layout
### Low (0x0000:0x7FFF)
| Range  | Purpose |
| ------------- | ------------- |
| 0x0000:0x00FF | Zero Page     |
| 0x0100:0x01FF | Stack         |
| 0x0200:0x7FFF | Kernel/Application code |

### High (0x8000:0xFFFF)
| Range  | Purpose |
| ------------- | ------------- |
| 0xFFFC:0xFFFD | Reset Vectors |
| 0xFFFA:0xFFFB | NM Interrupt service routine (not implemented) |
| 0xFFFE:0xFFFD | Interupt service routine (not implemented0 |
| 0xF420:0xF420 | Magic debug register |

## TODO
* Problems with CPU clock timing and read/write IO to high mem?
* Some sort of graphical output
  * VIA GPIO?
  * AVR Special registers + Display driver?
  * OLED or 120x20 LCD?
* Automate hexdump -> c array of kernel
  * Possibly send kernel over AVR USB?
* Run faster than 20hz
* Flesh out OS (mateOS?) 

## Pics
![Board pic](/pic.png)

