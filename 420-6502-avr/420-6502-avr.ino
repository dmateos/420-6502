#include <SoftwareSerial.h>

#define CLOCKSPEED 100
#define SERIALBAUD 115200
#define STARTOFFSET 0x0200
#define CPUENABLED 1
#define RAMTEST 1  // Run a complete RAM test before starting the CPU
#define NOPTEST 0  // Fill ram with NOPS instead of real program

enum control_pins {
  RESETPIN = 3,  // (out) CPU reset, hold HIGH
  CLOCKPIN = 4,  // (out) CPU clock pusle
  RWPIN = 5,     /* (in/out*) CPU wants to read or write, HIGH for read
                    We can turn this into an OUTPUT to controll RAM r/w if the CPU
                    bus is disabled. */
  CPUBEPIN = 6,  // (out) CPU bus, HIGH for enabled
};

enum address_pins {
  ADDRESSPIN_0 = 22,
  ADDRESSPIN_1,
  ADDRESSPIN_2,
  ADDRESSPIN_3,
  ADDRESSPIN_4,
  ADDRESSPIN_5,
  ADDRESSPIN_6,
  ADDRESSPIN_7,
  ADDRESSPIN_8,
  ADDRESSPIN_9,
  ADDRESSPIN_10,
  ADDRESSPIN_11,
  ADDRESSPIN_12,
  ADDRESSPIN_13,
  ADDRESSPIN_14,
  ADDRESSPIN_15,
};

enum data_pins {
  DATAPIN_0 = 40,
  DATAPIN_1,
  DATAPIN_2,
  DATAPIN_3,
  DATAPIN_4,
  DATAPIN_5,
  DATAPIN_6,
  DATAPIN_7,
};

static uint32_t ram_errors = 0;

// Program is generated with bin2c.py util which turns 420-6502-os
// binary into a C array of hex values to inject into the 6502's RAM
extern const uint8_t program[];
extern const uint32_t program_size;

void print_short(uint16_t d) {
  char msg[32];
  snprintf(msg, 32, "%#06x (%hu)\r\n", d, d);
  Serial.print(msg);
}

void print_byte(uint8_t b) {
  char msg[32];
  snprintf(msg, 32, "%#04x (%hhu)\r\n", b, b);
  Serial.print(msg);
}

int set_data_state(uint32_t state) {
  if (state != OUTPUT && state != INPUT) {
    return 1;
  }
  for (int i = 0; i < 8; i++) {
    pinMode(DATAPIN_0 + i, state);
  }
  return 0;
}

int set_address_state(uint32_t state) {
  if (state != OUTPUT && state != INPUT) {
    return 1;
  }
  for (int i = 0; i < 16; i++) {
    pinMode(ADDRESSPIN_0 + i, state);
  }
  return 0;
}

void write_address(uint16_t address) {
  for (int i = 0; i < 16; i++) {
    digitalWrite(ADDRESSPIN_0 + i, (address >> i) & 1);
  }
}

uint16_t read_address() {
  uint16_t data = 0;
  for (int i = 0; i < 16; i++) {
    byte d = digitalRead(ADDRESSPIN_15 - i);
    data = (data << 1) | d;
  }
  return data;
}

void write_byte(uint8_t data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(DATAPIN_0 + i, (data >> i) & 1);
  }
}

uint8_t read_byte() {
  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    uint8_t d = digitalRead(DATAPIN_7 - i);
    data = (data << 1) | d;
  }
  return data;
}

int write_ram(uint16_t address, uint8_t b) {
  write_address(address);
  write_byte(b);

  // Pulse to signfiy a write to ram
  digitalWrite(RWPIN, LOW);
  digitalWrite(RWPIN, HIGH);
  return 0;
}

uint8_t read_ram(uint16_t address) {
  write_address(address);
  return read_byte();
}

void write_program_to_ram() {
  // Make sure these are in a known state
  pinMode(RWPIN, OUTPUT);
  set_address_state(OUTPUT);
  digitalWrite(CPUBEPIN, LOW);  // CPU bus disabled
  digitalWrite(RWPIN, HIGH);
  set_data_state(OUTPUT);

  if (NOPTEST) {
    Serial.println("RAM: writing NOP program");
    for (uint32_t i = 0; i < 0x8000; i++) {
      write_ram(i, 0xEA);
    }
  } else {
    Serial.println("RAM: writing program");
    for (uint32_t i = 0; i < program_size; i++) {
      write_ram(i + STARTOFFSET, program[i]);

      print_short(i + STARTOFFSET);
      print_byte(program[i]);
    }
  }
  Serial.println("RAM: done writing program");
}

unsigned int ram_test() {
  uint32_t error = 0;

  // Make sure these are in a known state
  pinMode(RWPIN, OUTPUT);
  set_address_state(OUTPUT);
  digitalWrite(CPUBEPIN, LOW);  // CPU bus disabled
  digitalWrite(RWPIN, HIGH);
  set_data_state(OUTPUT);

  Serial.println("RAM test: writing values 0x0:0x7FFF");
  // Write a value to each memory address
  for (uint32_t i = 0; i < 0x8000; i++) {
    write_ram(i, i % 256);
  }

  Serial.println("RAM test: reading values 0x0:0x7FFF");
  // Read back each value from each address and verify
  digitalWrite(RWPIN, HIGH);  // high to tell the ram we want to read
  set_data_state(INPUT);
  for (uint32_t i = 0; i < 0x8000; i++) {
    byte b = read_ram(i);

    if (b != (i % 256)) {
      Serial.println("RAM test: error");
      print_byte(b);
      print_byte(i % 256);
      print_short(i);
      error++;
    }
  }

  if (error == 0) {
    Serial.println("RAM test: passed");
  } else {
    Serial.println("RAM test: failed");
    print_short(error);
  }

  return error;
}

void clock_cycle() {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(CLOCKPIN, HIGH);
  delay(CLOCKSPEED / 2);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(CLOCKPIN, LOW);
  delay(CLOCKSPEED / 2);
}

void init_cpu() {
  // Make sure these are in a known state
  pinMode(RWPIN, INPUT);
  set_address_state(INPUT);
  set_data_state(INPUT);

  digitalWrite(CPUBEPIN, HIGH);  // CPU bus enabled

  // Reset the CPU
  digitalWrite(RESETPIN, LOW);
  for (int i = 0; i < 4; i++) {
    clock_cycle();
  }
  digitalWrite(RESETPIN, HIGH);
  Serial.println("CPU: reset");
}

void handle_read_request(uint16_t addr) {
  switch (addr) {
    // These are the addresses the CPU first requests data from to
    // determine where to start execution
    case 0xFFFC:
      set_data_state(OUTPUT);
      write_byte(lowByte(STARTOFFSET));
      Serial.println("CPU: requested 0xFFFC start byte");
      print_byte(lowByte(STARTOFFSET));
      break;
    case 0xFFFD:
      set_data_state(OUTPUT);
      write_byte(highByte(STARTOFFSET));
      Serial.println("CPU: requested 0xFFFD start byte");
      print_byte(highByte(STARTOFFSET));
      break;
    case STARTOFFSET:
      // At this point the CPU is running, so we will switch the datastate
      // back to input so we dont mess with the RAM on the bus.
      set_data_state(INPUT);
      Serial.println("CPU: running");
      print_short(addr);
      print_byte(read_byte());
      break;
    case 0xF420:
      Serial.println("CPU: read from magic register!!!");
      print_short(addr);
      print_byte(read_byte());
      break;
    default:
      // Serial.println("CPU: read request");
      // print_short(addr);
      // print_byte(read_byte());
      break;
  }
}

void handle_write_request(uint16_t addr) {
  switch (addr) {
    case 0xF420:
      Serial.println("CPU: write to magic register!!!");
      print_short(addr);
      print_byte(read_byte());
      break;
    default:
      // Serial.println("CPU: write request");
      // print_short(addr);
      // print_byte(read_byte());
      break;
  }
}

void setup() {
  // Setup initial state
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RESETPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(RWPIN, INPUT);
  pinMode(CPUBEPIN, OUTPUT);

  set_address_state(INPUT);
  set_data_state(INPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RESETPIN, HIGH);
  digitalWrite(CLOCKPIN, LOW);
  digitalWrite(CPUBEPIN, LOW);  // Disable the CPU bus for now

  // Setup serial connection back to computer
  Serial.begin(SERIALBAUD);
  while (!Serial) {
    continue;
  }
  Serial.println("420-6502-001");

  if (RAMTEST) {
    ram_errors = ram_test();
  }

  if (CPUENABLED && ram_errors == 0) {
    write_program_to_ram();
    init_cpu();
  }
}

void loop() {
  if (!CPUENABLED || ram_errors > 0) {
    return;
  }

  clock_cycle();
  uint16_t addr_data = read_address();

  // High is a read request from the CPU
  if (digitalRead(RWPIN) == HIGH) {
    handle_read_request(addr_data);
  } else {
    handle_write_request(addr_data);
  }
}
