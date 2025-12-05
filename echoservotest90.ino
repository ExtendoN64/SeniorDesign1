#include <EchoLib.h>

ServoDriver servoDriver;

void setup() {
    servoDriver.init();
    
    // Move servo on port 0 to 90 degrees
    servoDriver.setServoAngle(0, 90);
    servoDriver.setServoAngle(1, 90);
}

void loop() {
    // Servo holds position automatically
}
