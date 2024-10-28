#include <Wire.h> 

bool is_motion = false;
uint32_t last_motion_1;
uint32_t last_motion_2;
uint16_t motion_cooldown = 5000; // 5 sec
// cage motion sensors pins
const byte PIR_1 = 5; // D2 
const byte PIR_2 = 6; // D3

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("RAD Arduino Nano running");
  last_motion_1 = last_motion_2 = millis();
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
}

void loop() {
  // check cage #1
  is_motion = digitalRead(PIR_1);
  if (is_motion) {
    if ((millis() - last_motion_1) > motion_cooldown) {
      last_motion_1 = millis();
      Serial.println("MOTION CAGE 1");
    }
  }

  // check cage #2
  is_motion = digitalRead(PIR_2);
  if (is_motion) {
    if ((millis() - last_motion_2) > motion_cooldown) {
      last_motion_2 = millis();
      Serial.println("MOTION CAGE 2");
    }
  }
}