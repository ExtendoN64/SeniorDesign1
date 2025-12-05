#include <EchoLib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define M1_ENCA 9
#define M1_ENCB 10
#define M2_ENCA 11
#define M2_ENCB 12
#define M3_ENCA 13
#define M3_ENCB 14
#define M4_ENCA 3
#define M4_ENCB 42

// ENCODER COUNTERS
volatile long enc1 = 0;
volatile long enc2 = 0;
volatile long enc3 = 0;
volatile long enc4 = 0;

volatile bool lastA1, lastA2, lastA3, lastA4;

// LCD / BLE / MOTORS

LiquidCrystal_I2C lcd(0x27, 20, 4);
MotorControllers motorControllers;
BLEServerWrapper ble("EchoBot");


// Function to print BLE messages on LCD
void displayMessage(String msg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
}

void IRAM_ATTR encoder1_ISR() {
  bool A = digitalRead(M1_ENCA);
  bool B = digitalRead(M1_ENCB);

  if (A != lastA1) {
    enc1 += (A == B) ? 1 : -1;
    lastA1 = A;
  }
}

void IRAM_ATTR encoder2_ISR() {
  bool A = digitalRead(M2_ENCA);
  bool B = digitalRead(M2_ENCB);

  if (A != lastA2) {
    enc2 += (A == B) ? 1 : -1;
    lastA2 = A;
  }
}

void IRAM_ATTR encoder3_ISR() {
  bool A = digitalRead(M3_ENCA);
  bool B = digitalRead(M3_ENCB);

  if (A != lastA3) {
    enc3 += (A == B) ? 1 : -1;
    lastA3 = A;
  }
}

void IRAM_ATTR encoder4_ISR() {
  bool A = digitalRead(M4_ENCA);
  bool B = digitalRead(M4_ENCB);

  if (A != lastA4) {
    enc4 += (A == B) ? 1 : -1;
    lastA4 = A;
  }
}

long getEnc1() {
  noInterrupts();
  long v = enc1;
  interrupts();
  return v;
}
long getEnc2() {
  noInterrupts();
  long v = enc2;
  interrupts();
  return v;
}
long getEnc3() {
  noInterrupts();
  long v = enc3;
  interrupts();
  return v;
}
long getEnc4() {
  noInterrupts();
  long v = enc4;
  interrupts();
  return v;
}


void setup() {
  Wire.begin();
  Serial.begin(115200);

  ble.begin();
  ble.connect();
  Serial.println("BLE + Motor + Encoder System Ready");


  // Setup encoder pins
  pinMode(M1_ENCA, INPUT_PULLUP);
  pinMode(M1_ENCB, INPUT_PULLUP);

  pinMode(M2_ENCA, INPUT_PULLUP);
  pinMode(M2_ENCB, INPUT_PULLUP);

  pinMode(M3_ENCA, INPUT_PULLUP);
  pinMode(M3_ENCB, INPUT_PULLUP);

  pinMode(M4_ENCA, INPUT_PULLUP);
  pinMode(M4_ENCB, INPUT_PULLUP);

  // Initial lastA reads
  lastA1 = digitalRead(M1_ENCA);
  lastA2 = digitalRead(M2_ENCA);
  lastA3 = digitalRead(M3_ENCA);
  lastA4 = digitalRead(M4_ENCA);

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(M1_ENCA), encoder1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M1_ENCB), encoder1_ISR, CHANGE);

  attachInterrupt(digitalPinToInterrupt(M2_ENCA), encoder2_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M2_ENCB), encoder2_ISR, CHANGE);

  attachInterrupt(digitalPinToInterrupt(M3_ENCA), encoder3_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M3_ENCB), encoder3_ISR, CHANGE);

  attachInterrupt(digitalPinToInterrupt(M4_ENCA), encoder4_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M4_ENCB), encoder4_ISR, CHANGE);

  Serial.println("4-Encoder Quadrature System Ready");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("BLE Robot Ready");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for cmds...");
}

void loop() {
  ble.loop();

  if (ble.getStatus()) {
    String cmd = ble.readData();

    // --- FORWARD ---
    if (cmd == "FORWARD") {
      String msg = "Motors forward";
      ble.sendData(msg);
      displayMessage(msg);

      motorControllers.set(1, 80);
      motorControllers.set(2, 80);
      motorControllers.set(3, 80);
      motorControllers.set(4, 80);
    }

    // --- REVERSE ---
    if (cmd == "REVERSE") {
      String msg = "Motors reverse";
      ble.sendData(msg);
      displayMessage(msg);

      motorControllers.set(1, -80);
      motorControllers.set(2, -80);
      motorControllers.set(3, -80);
      motorControllers.set(4, -80);
    }

    // --- STOP ---
    if (cmd == "STOP") {
      String msg = "STOP all motors";
      ble.sendData(msg);
      displayMessage(msg);

      motorControllers.set(1, 0);
      motorControllers.set(2, 0);
      motorControllers.set(3, 0);
      motorControllers.set(4, 0);
    }
    // Optional: encoder debug command
    else if (cmd == "ENC") {
      String msg = "E1:" + String(getEnc1()) + " E2:" + String(getEnc2()) + " E3:" + String(getEnc3()) + " E4:" + String(getEnc4());
      ble.sendData(msg);
      displayMessage("Enc on Serial");
      Serial.println(msg);
    }
  }

  delay(50);
}
