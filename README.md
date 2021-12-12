# 420-6502
Custom built 6502 based computer + system interface controller

## Hardware Components
* W65C02S 8-bit Microprocessor
* UM61256FK-15 32KB RAM 
  * Chip enable is tied to addr line 15 of CPU
* Arduino Mega 2560 AVR 
* Breadboard
* Shitloads of wires and LEDS
* Resistance (in the form of resistors)
* 128x64 i2c compadible OLED. 

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
  * Provides a basic graphics interface  
* 420-6502 OS 
  * Basic Kernel
  * Tests the CPU stack is working 
  * Tests basic logic and RAM reading/writting is working
  * Prints Hello World via the AVR "Graphics Driver"

## Memory Layout
### Low, Physical 32kb RAM (0x0000:0x7FFF)
| Start | End  | Purpose |
| ------| ------- | ------------- |
| 0x0000 | 0x00FF | Zero Page     |
| 0x0100 | 0x01FF | Stack         |
| 0x0200 | 0x7FFF | Kernel/Application code |

### High, AVR Simulated RAM/Registers (0x8000:0xFFFF)
| Start |  End  | Purpose |
| ------|------- | ------------- |
| 0xF420 | - | GFX Register, ASCII char or 0xFF to clear screen |
| 0xFFFA | 0xFFFB | NM Interrupt service routine (not implemented) |
| 0xFFFC | 0xFFFD | Reset Vectors (set to 0x0200) |
| 0xFFFE | 0xFFFF | Interrupt service routine (not implemented) |

## TODO
* ~~Problems with CPU clock timing and read/write IO to high mem?~~
* ~~Some sort of graphical output
  * ~~AVR Special registers + Display driver~~
  * ~~OLED or 120x20 LCD?~~
  * Pixel GFX mode (x,y,n)
* ~~Automate hexdump -> c array of kernel~~
  * Possibly send kernel over AVR USB?
* Run faster than 20hz
* Flesh out OS (mateOS?) 
  * IRQ Support 
  * String libs 
* Cool registers?
  * CPU defined clock rate/scaling?
  * CPU auto reset

## Pics
![Board pic](/pic2.png)
![Board pic](/pic.png)

