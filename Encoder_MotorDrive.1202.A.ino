#include <util/atomic.h> // For the ATOMIC_BLOCK macro

#define ENCA 2 // YELLOW
#define ENCB 4 // GREEN
#define IN2 6  // BROWN (must be PWM-capable)
#define IN1 3  // PURPLE (must be PWM-capable)

volatile int posi = 0; // encoder count

void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Stop motor initially
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
}

void loop() {
  int pos = 0;

  // Atomic read of encoder position
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }

  // Forward at speed 80
  setMotor(1, 80, IN1, IN2);
  delay(200);
  Serial.println(pos);

  // Reverse at speed 80
  setMotor(-1, 80, IN1, IN2);
  delay(200);
  Serial.println(pos);

  // Stop
  setMotor(0, 0, IN1, IN2);
  delay(20);
  Serial.println(pos);
}

void setMotor(int dir, int speed, int in1, int in2) {
  speed = constrain(speed, 0, 255);

  if (dir == 1) {
    // Forward: IN1 PWM, IN2 LOW
    analogWrite(in1, speed);
    digitalWrite(in2, LOW);
  }
  else if (dir == -1) {
    // Reverse: IN2 PWM, IN1 LOW
    analogWrite(in2, speed);
    digitalWrite(in1, LOW);
  }
  else {
    // Stop: both off
    analogWrite(in1, 0);
    analogWrite(in2, 0);
  }
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    posi++;
  } else {
    posi--;
  }
}
