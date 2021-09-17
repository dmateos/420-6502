#include <SoftwareSerial.h>

#define CLOCKSPEED 100
#define RESETPIN 3
#define CLOCKPIN 4

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

const unsigned char program[] = {0xEA};
const unsigned int program_length = 1;

void print_short(unsigned short d) {
    char msg[32];
    snprintf(msg, 32, "%#06x (%hu)\r\n", d, d);
    Serial.print(msg);
}

void clock_cycle() {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(CLOCKPIN, HIGH);
    delay(CLOCKSPEED / 2);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(CLOCKPIN, LOW);
    delay(CLOCKSPEED / 2);
}

unsigned short read_address_pins() {
    unsigned short data;
    for (int i = 0; i < 16; i++) {
        byte d = digitalRead(ADDRESSPIN_15 - i);
        data = (data << 1) | d;
    }
    return data;
}

void write_program(unsigned char *pgrm, unsigned int length) {
    static int pc = 0;

    for (int i = 0; i < 8; i++) {
        digitalWrite(DATAPIN_0 + i, (pgrm[pc] >> i) & 1);
    }

    pc++;
    if (pc == length) {
        pc = 0;
    }
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

    for (int i = 0; i < 16; i++) {
        pinMode(ADDRESSPIN_0 + i, INPUT);
    }

    for (int i = 0; i < 8; i++) {
        pinMode(DATAPIN_0 + i, OUTPUT);
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
    write_program(program, program_length);
    clock_cycle();

    unsigned short addr_data = read_address_pins();
    print_short(addr_data);
}
