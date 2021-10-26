#include <SoftwareSerial.h>

#define CLOCKSPEED 100
#define SERIALBAUD 115200
#define STARTOFFSET 0x0200
#define RAMTEST 1
#define CPUENABLED 1
#define NOPTEST 0

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

static unsigned int ram_errors = 0;

const byte[] program = {
   0x20, 0x06, 0x02, 0x4C, 0x00, 0x02, 
   0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA,  
   0xEA, 0xEA, 0xEA, 0xEA,  0x60,
}

void print_short(unsigned short d) {
  char msg[32];
  snprintf(msg, 32, "%#06x (%hu)\r\n", d, d);
  Serial.print(msg);
}

void print_byte(byte b) {
  char msg[32];
  snprintf(msg, 32, "%#04x (%hhu)\r\n", b, b);
  Serial.print(msg);
}

void write_address(unsigned short address) {
  for (int i = 0; i < 16; i++) {
    digitalWrite(ADDRESSPIN_0 + i, (address >> i) & 1);
  }
}

unsigned short read_address() {
  unsigned short data = 0;
  for (int i = 0; i < 16; i++) {
    byte d = digitalRead(ADDRESSPIN_15 - i);
    data = (data << 1) | d;
  }
  return data;
}

void write_byte(byte data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(DATAPIN_0 + i, (data >> i) & 1);
  }
}

byte read_byte() {
  byte data = 0;
  for (int i = 0; i < 8; i++) {
    byte d = digitalRead(DATAPIN_7 - i);
    data = (data << 1) | d;
  }
  return data;
}

int set_data_state(int state) {
  if (state != OUTPUT && state != INPUT) {
    return 1;
  }
  for (int i = 0; i < 8; i++) {
    pinMode(DATAPIN_0 + i, state);
  }
  return 0;
}

int set_address_state(int state) {
  if (state != OUTPUT && state != INPUT) {
    return 1;
  }
  for (int i = 0; i < 16; i++) {
    pinMode(ADDRESSPIN_0 + i, state);
  }
  return 0;
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
    for (unsigned int i = 0; i < 0x8000; i++) {
      write_address(i);
      write_byte(0xEA);

      // Pulse to signfiy a write to ram
      digitalWrite(RWPIN, LOW);
      digitalWrite(RWPIN, HIGH);
    }
    Serial.println("RAM: done writing NOP program");
  } else {
    unsigned int program_size = sizeof(program) + STARTOFFSET;
    for (unsigned int i = STARTOFFSET; i < program_size; i++) {
      write_address(i);
      write_byte(program[i - STARTOFFSET]);
      // Pulse to signfiy a write to ram
      digitalWrite(RWPIN, LOW);
      digitalWrite(RWPIN, HIGH);
    }
  }
}

unsigned int ram_test() {
  unsigned int error = 0;

  // Make sure these are in a known state
  pinMode(RWPIN, OUTPUT);
  set_address_state(OUTPUT);
  digitalWrite(CPUBEPIN, LOW);  // CPU bus disabled
  digitalWrite(RWPIN, HIGH);
  set_data_state(OUTPUT);

  Serial.println("RAM test: writing values 0x0:0x7FFF");
  // Write a value to each memory address
  for (unsigned int i = 0; i < 0x8000; i++) {
    write_address(i);
    write_byte(i % 256);

    // Pulse to signfiy a write to ram
    digitalWrite(RWPIN, LOW);
    digitalWrite(RWPIN, HIGH);
  }

  Serial.println("RAM test: reading values 0x0:0x7FFF");
  // Read back each value from each address and verify
  digitalWrite(RWPIN, HIGH);  // high to tell the ram we want to read
  set_data_state(INPUT);
  for (unsigned int i = 0; i < 0x8000; i++) {
    write_address(i);
    byte b = read_byte();

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

void handle_read_request(unsigned short addr) {
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
      break;
    default:
      print_short(addr);
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

  // High is a read request from the CPU
  if (digitalRead(RWPIN) == HIGH) {
    unsigned short addr_data = read_address();
    handle_read_request(addr_data);
  }
}
