#include <EchoLib.h>

ServoDriver servoDriver;

void setup() {
  servoDriver.init();
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    servoDriver.setServoAngle(0, angle);
    servoDriver.setServoAngle(1, angle);
    delay(20);  // Small delay for smooth movement
  }

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    servoDriver.setServoAngle(0, angle);
    servoDriver.setServoAngle(1, angle);
    delay(20);
  }
}
