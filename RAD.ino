#include <Wire.h> 

bool is_motion = false;
uint16_t motion_cooldown = 5000; // 5 sec
uint32_t last_motion_1;
uint32_t last_motion_2;
uint32_t last_motion_3;
uint32_t last_motion_4;
uint32_t last_motion_5;
uint32_t last_motion_6;
uint32_t last_motion_7;
uint32_t last_motion_8;
uint32_t last_motion_9;
uint32_t last_motion_10;
uint32_t last_motion_11;
uint32_t last_motion_12;
// cage motion sensors pins
const byte PIR_1 = 13;
const byte PIR_2 = 2;
const byte PIR_3 = 3;
const byte PIR_4 = 4;
const byte PIR_5 = 5;
const byte PIR_6 = 6;
const byte PIR_7 = 7;
const byte PIR_8 = 8;
const byte PIR_9 = 9;
const byte PIR_10 = 10;
const byte PIR_11 = 11;
const byte PIR_12 = 12;

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("RAD Arduino Nano running");
  last_motion_1 = last_motion_2 = last_motion_3 = millis();
  last_motion_3 = last_motion_4 = last_motion_5 = last_motion_6;
  last_motion_6 = last_motion_7 = last_motion_8 = last_motion_9;
  last_motion_9 = last_motion_10 = last_motion_11 = last_motion_12;
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(PIR_3, INPUT);
  pinMode(PIR_4, INPUT);
  pinMode(PIR_5, INPUT);
  pinMode(PIR_6, INPUT);
  pinMode(PIR_7, INPUT);
  pinMode(PIR_8, INPUT);
  pinMode(PIR_9, INPUT);
  pinMode(PIR_10, INPUT);
  pinMode(PIR_11, INPUT);
  pinMode(PIR_12, INPUT);
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

  // check cage #3
  is_motion = digitalRead(PIR_3);
  if (is_motion) {
    if ((millis() - last_motion_3) > motion_cooldown) {
      last_motion_3 = millis();
      Serial.println("MOTION CAGE 3");
    }
  }

  // check cage #4
  is_motion = digitalRead(PIR_4);
  if (is_motion) {
    if ((millis() - last_motion_4) > motion_cooldown) {
      last_motion_4 = millis();
      Serial.println("MOTION CAGE 4");
    }
  }

  // check cage #5
  is_motion = digitalRead(PIR_5);
  if (is_motion) {
    if ((millis() - last_motion_5) > motion_cooldown) {
      last_motion_5 = millis();
      Serial.println("MOTION CAGE 5");
    }
  }

  // check cage #6
  is_motion = digitalRead(PIR_6);
  if (is_motion) {
    if ((millis() - last_motion_6) > motion_cooldown) {
      last_motion_6 = millis();
      Serial.println("MOTION CAGE 6");
    }
  }

  // check cage #7
  is_motion = digitalRead(PIR_7);
  if (is_motion) {
    if ((millis() - last_motion_7) > motion_cooldown) {
      last_motion_7 = millis();
      Serial.println("MOTION CAGE 7");
    }
  }

  // check cage #8
  is_motion = digitalRead(PIR_8);
  if (is_motion) {
    if ((millis() - last_motion_8) > motion_cooldown) {
      last_motion_8 = millis();
      Serial.println("MOTION CAGE 8");
    }
  }

  // check cage #9
  is_motion = digitalRead(PIR_9);
  if (is_motion) {
    if ((millis() - last_motion_9) > motion_cooldown) {
      last_motion_9 = millis();
      Serial.println("MOTION CAGE 9");
    }
  }

  // check cage #10
  is_motion = digitalRead(PIR_10);
  if (is_motion) {
    if ((millis() - last_motion_10) > motion_cooldown) {
      last_motion_10 = millis();
      Serial.println("MOTION CAGE 10");
    }
  }

  // check cage #11
  is_motion = digitalRead(PIR_11);
  if (is_motion) {
    if ((millis() - last_motion_11) > motion_cooldown) {
      last_motion_11 = millis();
      Serial.println("MOTION CAGE 11");
    }
  }

  // check cage #12
  is_motion = digitalRead(PIR_12);
  if (is_motion) {
    if ((millis() - last_motion_12) > motion_cooldown) {
      last_motion_12 = millis();
      Serial.println("MOTION CAGE 12");
    }
  }
}
