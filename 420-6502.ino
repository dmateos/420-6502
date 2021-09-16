#include <SoftwareSerial.h>

#define RESETPIN 3
#define CLOCKPIN 4
#define CLOCKSPEED 100

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

// Keep track of current clock state
static unsigned char clock_state = LOW;


void print_short(unsigned short d) {
    char msg[32];
    snprintf(msg, 32, "%#06x (%hu)\r\n", d, d);
    Serial.print(msg);
}

void clock_cycle() {
    if(clock_state == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(CLOCKPIN, HIGH);
      clock_state = HIGH;
    } else if(clock_state == HIGH) {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(CLOCKPIN, LOW);
      clock_state = LOW;
    }
    delay(CLOCKSPEED);
}

unsigned short read_address_pins() {
    unsigned short data;
    for (int i = 0; i < 16; i++) {
        byte d = digitalRead(ADDRESSPIN_0 + i);
        data = (data << 1) | d;
    }
    return data;
}

void init_cpu() {
    // Reset the CPU
    digitalWrite(RESETPIN, LOW);
    clock_cycle();
    clock_cycle();
    digitalWrite(RESETPIN, HIGH);
}

void setup() {
    // Setup initial state
    pinMode(RESETPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 16; i++) {
      pinMode(ADDRESSPIN_0 + i, INPUT);
    }

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RESETPIN, HIGH);
    digitalWrite(CLOCKPIN, LOW);

    // Setup serial connection back to computer
    Serial.begin(115200);
    while (!Serial) {
        continue;
    }

    init_cpu();
}

void loop() {
    clock_cycle();
    unsigned short addr_data = read_address_pins();
    print_short(addr_data);
}

