
const byte ledPin = LED_BUILTIN;
const byte interruptPin = 2;
volatile byte state = LOW;
uint32_t counter = 0;
uint32_t counter_3 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), blink_3, FALLING);
}

void loop() {
  digitalWrite(ledPin, state);
  static uint32_t old = 0;
  static uint32_t old_3 = 0;
  if (counter != old || counter_3 != old_3) {
    Serial.print("2 = "); Serial.print(counter);
    Serial.print("3 = "); Serial.println(counter_3);
    old = counter;
    old_3 = counter_3;
  }
}

void blink() {
  state = !state;
  ++counter;
}

void blink_3() {
  state = !state;
  ++counter_3;
}