#define CLOCKSPEED 100

#define RESETPIN 3
#define CLOCKPIN 4

void clock_cycle() {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(CLOCKPIN, HIGH);
  delay(CLOCKSPEED / 2);
  digitalWrite(CLOCKPIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(CLOCKSPEED / 2);
}

void init_cpu() {
  // Reset the CPU
  digitalWrite(RESETPIN, LOW);
  for (int i = 0; i < 4; i++) {
    clock_cycle();
  }
  digitalWrite(RESETPIN, HIGH);
}

void setup() {
  // Setup initial state
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RESETPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RESETPIN, HIGH);
  digitalWrite(CLOCKPIN, LOW);

  init_cpu();
}

void loop() {
  clock_cycle();
}
