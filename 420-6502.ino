#define RESETPIN 3
#define CLOCKPIN 4
#define CLOCKSPEED 50

void clock_cycle() {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(CLOCKPIN, HIGH);
        delay(CLOCKSPEED/2);
        digitalWrite(CLOCKPIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        delay(CLOCKSPEED/2);
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

        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RESETPIN, HIGH);
        digitalWrite(CLOCKPIN, LOW);

	init_cpu();
}

void loop() {
	clock_cycle();
}
