#include <SoftwareSerial.h>

#define CLOCKSPEED 100
#define SERIALBAUD 115200
#define NOP 0xEA

#define RESETPIN 3
#define CLOCKPIN 4
#define RWPIN 5

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

unsigned short read_address_pins() {
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
  if (state != OUTPUT || state != INPUT) {
    return 1;
  }

  for (int i = 0; i < 8; i++) {
    pinMode(DATAPIN_0 + i, state);
  }
  return 0;
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
  // Reset the CPU
  digitalWrite(RESETPIN, LOW);
  clock_cycle();
  clock_cycle();
  digitalWrite(RESETPIN, HIGH);
  Serial.println("CPU Reset");
}

void setup() {
  // Setup initial state
  pinMode(RESETPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RWPIN, INPUT);

  for (int i = 0; i < 16; i++) {
    pinMode(ADDRESSPIN_0 + i, INPUT);
  }
  set_data_state(OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RESETPIN, HIGH);
  digitalWrite(CLOCKPIN, LOW);

  // Setup serial connection back to computer
  Serial.begin(SERIALBAUD);
  while (!Serial) {
    continue;
  }

  init_cpu();
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
      write_byte(0x00);
      break;
    case 0xFFFD:
      write_byte(0x00);
      break;
    default:
      write_byte(NOP);
      Serial.println("not implemented, sending NOP");
      break;
  }
}

void loop() {
  clock_cycle();

  unsigned short addr_data = read_address_pins();
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
