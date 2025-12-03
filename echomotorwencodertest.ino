#include <EchoLib.h>

// --- Encoder pins ---
#define ENCA_PIN 11
#define ENCB_PIN 12

volatile long encoderCount = 0;
volatile bool lastA = 0;

// Instance of your motor controllers (from EchoLib)
MotorControllers motorsControllers;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup encoder pins
  pinMode(ENCA_PIN, INPUT_PULLUP);
  pinMode(ENCB_PIN, INPUT_PULLUP);
  lastA = digitalRead(ENCA_PIN);
  attachInterrupt(digitalPinToInterrupt(ENCA_PIN), onEncoderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB_PIN), onEncoderChange, CHANGE);

  // Possibly some initialization for EchoLib / driver board
  // e.g. motorsControllers.begin(...) — if your library requires init
}

void loop() {
  // Example: Run motor forward
  motorsControllers.set(1, 80); // Motor 1, 80% forward — adjust if needed

  long startCount = getEncoderCount();
  long targetIncrement = 1000; // example: run until encoder increments this amount

  while ((getEncoderCount() - startCount) < targetIncrement) {
    // You might monitor encoderCount in real time
    // or add safety/timeouts
  }

  // Stop motor
  motorsControllers.set(1, 0);

  Serial.print("Final encoder count: ");
  Serial.println(getEncoderCount());

  delay(2000); // wait before next move — adjust as needed
}

long getEncoderCount() {
  noInterrupts();
    long c = encoderCount;
  interrupts();
  return c;
}

// Quadrature encoder ISR — minimal, fast
void onEncoderChange() {
  bool A = digitalRead(ENCA_PIN);
  bool B = digitalRead(ENCB_PIN);
  if (A != lastA) {
    if (A == B) encoderCount++;
    else encoderCount--;
  }
  lastA = A;
}
