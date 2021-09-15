#define RESETPIN 3
#define CLOCKPIN 4
#define CLOCKSPEED 50

void setup() {
        pinMode(RESETPIN, OUTPUT);
        pinMode(CLOCKPIN, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);

        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RESETPIN, HIGH);
        digitalWrite(CLOCKPIN, LOW);
        delay(1000);

        digitalWrite(RESETPIN, LOW);
        digitalWrite(CLOCKPIN, HIGH);
        digitalWrite(CLOCKPIN, LOW);
        delay(100);
        digitalWrite(CLOCKPIN, HIGH);
        digitalWrite(CLOCKPIN, LOW);
        delay(2000);
        digitalWrite(RESETPIN, HIGH);
}

void loop() {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(CLOCKPIN, HIGH);
        delay(CLOCKSPEED/2);
        digitalWrite(CLOCKPIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        delay(CLOCKSPEED/2);
}
