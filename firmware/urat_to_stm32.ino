#include <Wire.h>
#include <VL53L0X.h>

#define XSHUT1 4
#define XSHUT2 5

#define TRIG_PIN 8
#define ECHO_PIN 9

VL53L0X s1;
VL53L0X s2;

static uint16_t readUltrasonicMM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long t = pulseIn(ECHO_PIN, HIGH, 25000UL); // 25 ms timeout
  if (t == 0) return 0; // no echo
  // distance (mm) ≈ t / 5.8  (since cm = t/58, mm = t/5.8)
  return (uint16_t)(t / 5.8);
}

void setup() {
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  delay(10);

  Wire.begin();
  Wire.setClock(100000); // stable I2C (safe on breadboard)

  // Bring up sensor 1, set new address
  digitalWrite(XSHUT1, HIGH);
  delay(10);
  s1.init();
  s1.setAddress(0x30);

  // Bring up sensor 2, set new address
  digitalWrite(XSHUT2, HIGH);
  delay(10);
  s2.init();
  s2.setAddress(0x31);

  // Optional: reduce measurement timing (don’t go crazy on UNO)
  s1.setMeasurementTimingBudget(20000);
  s2.setMeasurementTimingBudget(20000);

  Serial.begin(115200);  // match STM32
  delay(200);
  Serial.println(F("BOOT"));
}

void loop() {
  uint16_t v1 = s1.readRangeSingleMillimeters();
  uint16_t v2 = s2.readRangeSingleMillimeters();
  uint16_t us = readUltrasonicMM();

  // transmit with minimal SRAM use
  Serial.print(F("V1=")); Serial.print(v1);
  Serial.print(F(",V2=")); Serial.print(v2);
  Serial.print(F(",US=")); Serial.println(us);

  delay(100);
}
