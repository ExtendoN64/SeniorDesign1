#include <Wire.h>

// ======== HARDWARE CONFIGURATION ======== //  
// Motor Driver Pins  
// ENABLE = PWM  
// IN1 = REVERSE must be HIGH if reversing (IN2 = LOW)  
// IN2 = FORWARD must be HIGH if forwarding (IN1 = LOW)  

// LEFT DRIVER  
// FRONT LEFT A  
#define ENABLEAFL 35  
#define IN1_FL     37   // REVERSE  
#define IN2_FL     39   // FORWARD  
// REAR LEFT B  
#define ENABLEBRL 34  
#define IN1_RL     36   // REVERSE  
#define IN2_RL     38   // FORWARD  

// RIGHT DRIVER  
// REAR RIGHT A  
#define ENABLEARR  43  
#define IN1_RR     45  
#define IN2_RR     47  

// FRONT RIGHT B  
#define ENABLEBFR 42  
#define IN1_FR     44  
#define IN2_FR     46  

// Motor base PWM values (you may override these dynamically)  
const int pwm_ENABLEBRL = 74;  
const int pwm_ENABLEARR = 80;  
const int pwm_ENABLEAFL = 79;  
const int pwm_ENABLEBFR = 79;

// ======== CONTROLLER INPUT PLACEHOLDERS ======== //  
// You will need to add / integrate a library to read the Xbox controller (via USB host or BLE)  
int16_t stickLX = 0;  // Left stick X  
int16_t stickLY = 0;  // Left stick Y  
int16_t stickRX = 0;  // Right stick X  
int16_t stickRY = 0;  // Right stick Y  

// Helper to write to a single motor  
void writeMotor(int enablePin, int in1Pin, int in2Pin, int value) {
  // value range: negative for reverse, positive for forward  
  if (value > 0) {
    digitalWrite(in2Pin, HIGH);
    digitalWrite(in1Pin, LOW);
    analogWrite(enablePin, value);
  } else if (value < 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    analogWrite(enablePin, -value);
  } else {
    // stop
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    analogWrite(enablePin, 0);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  // Configure motor pins  
  pinMode(ENABLEAFL, OUTPUT);  pinMode(IN1_FL, OUTPUT);  pinMode(IN2_FL, OUTPUT);  
  pinMode(ENABLEBRL, OUTPUT);  pinMode(IN1_RL, OUTPUT);  pinMode(IN2_RL, OUTPUT);  
  pinMode(ENABLEARR, OUTPUT);  pinMode(IN1_RR, OUTPUT);  pinMode(IN2_RR, OUTPUT);  
  pinMode(ENABLEBFR, OUTPUT);  pinMode(IN1_FR, OUTPUT);  pinMode(IN2_FR, OUTPUT);  

  // Set base PWM rights (you can scale / adjust later)  
  analogWrite(ENABLEAFL, pwm_ENABLEAFL);  
  analogWrite(ENABLEBRL, pwm_ENABLEBRL);  
  analogWrite(ENABLEARR, pwm_ENABLEARR);  
  analogWrite(ENABLEBFR, pwm_ENABLEBFR);

  Serial.println("Setup complete. Ready to read controller and drive wheels.");
}

void loop() {
  // 1) Read controller inputs  
  // Placeholder: replace with actual reading library code  
  // e.g., stickLX = readLeftStickX(); stickLY = readLeftStickY();  
  // stickRX = ...; stickRY = ...;

  // For now, print placeholder values  
  Serial.print("LX="); Serial.print(stickLX);
  Serial.print("  LY="); Serial.print(stickLY);
  Serial.print("  RX="); Serial.print(stickRX);
  Serial.print("  RY="); Serial.println(stickRY);

  // 2) Map controller sticks to wheel commands  
  // Example: assume standard differential 4-wheel drive (two left wheels, two right wheels)  
  int speedForward = map(stickLY, -32768, 32767, -255, 255);
  int speedTurn    = map(stickLX, -32768, 32767, -255, 255);

  int leftSpeed  = speedForward + speedTurn;
  int rightSpeed = speedForward - speedTurn;

  // Constrain speeds  
  leftSpeed  = constrain(leftSpeed,  -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // 3) Write speeds to each wheel  
  // FRONT LEFT  
  writeMotor(ENABLEAFL, IN1_FL, IN2_FL, leftSpeed);
  // REAR LEFT  
  writeMotor(ENABLEBRL, IN1_RL, IN2_RL,  leftSpeed);
  // FRONT RIGHT  
  writeMotor(ENABLEBFR, IN1_FR, IN2_FR,  rightSpeed);
  // REAR RIGHT  
  writeMotor(ENABLEARR, IN1_RR, IN2_RR,  rightSpeed);

  // 4) Loop delay  
  delay(20);  // ~50Hz update rate. Adjust as needed.
}