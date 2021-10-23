#include <SoftwareSerial.h>

#define CLOCKSPEED 100
#define SERIALBAUD 115200
#define NOP 0xEA
#define STARTOFFSET 0xFF00

#define RESETPIN 3
#define CLOCKPIN 4
#define RWPIN 5
#define CPUBEPIN 6

#define RAMTEST true

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

// Our hacky pretend ROM
const byte program[] = {
    NOP,
    NOP,
    NOP,
};

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

int ram_test() {
  unsigned short testaddr[] = {0x00ff, 0x00aa, 0x0055, 0x0fff};  // fails....
  // unsigned short testaddr[] = {0x0022, 0x00AA, 0x0055, 0x0FFF}; //passes
  byte data[] = {0xff, 0xaa, 0xbb, 0xcc};
  int error = 0;

  set_address_state(OUTPUT);
  pinMode(RWPIN, OUTPUT);

  set_data_state(OUTPUT);
  digitalWrite(RWPIN, LOW);  // low to tell the ram we want to write
  for (int i = 0; i < 4; i++) {
    write_address(testaddr[i]);
    write_byte(data[i]);
    delay(1000);
  }

  digitalWrite(RWPIN, HIGH);  // high to tell the ram we want to read
  set_data_state(INPUT);
  for (int i = 0; i < 4; i++) {
    write_address(testaddr[i]);
    byte b = read_byte();

    if (b == data[i]) {
      Serial.println("RAM Test Passed");
      print_byte(b);
    } else {
      Serial.println("RAM Test Failed");
      print_byte(b);
      print_byte(data[i]);
      error++;
    }
    delay(1000);
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
  // Enable the CPU Bus
  digitalWrite(CPUBEPIN, HIGH);

  // Reset the CPU
  digitalWrite(RESETPIN, LOW);
  for (int i = 0; i < 4; i++) {
    clock_cycle();
  }
  digitalWrite(RESETPIN, HIGH);
  Serial.println("CPU Reset");
}

void handle_write_request(unsigned short addr) {
  byte data = read_byte();
  print_byte(data);
  Serial.println("not implemented");
}

void handle_read_request(unsigned short addr) {
  switch (addr) {
    // These are the addresses the CPU first requests data from to
    // determine where to start execution
    case 0xFFFC:
      write_byte(lowByte(STARTOFFSET));
      break;
    case 0xFFFD:
      write_byte(highByte(STARTOFFSET));
      break;
    default:
      write_byte(NOP);
      break;

      // TODO Fix this
      if ((addr - STARTOFFSET) < sizeof(program)) {
        write_byte(program[addr - STARTOFFSET]);
        Serial.println("not implemented, sending NOP");
      } else {
        Serial.println("CPU Requested out of bounds location");
      }
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
  set_data_state(OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RESETPIN, HIGH);
  digitalWrite(CLOCKPIN, LOW);
  digitalWrite(CPUBEPIN, LOW);  // Disable the CPU bus for now

  // Setup serial connection back to computer
  Serial.begin(SERIALBAUD);
  while (!Serial) {
    continue;
  }

  init_cpu();
}

void loop() {
  if (RAMTEST) {
    digitalWrite(CPUBEPIN, LOW);  // Disable CPU bus so no ram conflicts
    // This doesnt clean up state so we cant do normal stuff after this.
    ram_test();
    return;
  }

  clock_cycle();

  unsigned short addr_data = read_address();
  print_short(addr_data);

  // High is a read request from the CPU
  if (digitalRead(RWPIN) == HIGH) {
    Serial.println("CPU wants to read");
    handle_read_request(addr_data);
  } else {
    Serial.println("CPU wants to write");
    handle_write_request(addr_data);
  }
}
