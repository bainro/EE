// PIR-based motion sensor using arduino mega
#include <Wire.h> 

bool is_motion = false;
bool last_motion_1;
bool last_motion_2;
bool last_motion_3;
bool last_motion_4;
bool last_motion_5;
bool last_motion_6;
bool last_motion_7;
bool last_motion_8;
bool last_motion_9;
bool last_motion_10;
bool last_motion_11;
bool last_motion_12;
bool last_motion_13;
bool last_motion_14;
bool last_motion_15;
bool last_motion_16;

// cage motion sensors pins
const byte PIR_1 = 52;
const byte PIR_2 = 50;
const byte PIR_3 = 48;
const byte PIR_4 = 46;
const byte PIR_5 = 44;
const byte PIR_6 = 42;
const byte PIR_7 = 40;
const byte PIR_8 = 38;
const byte PIR_9 = 36;
const byte PIR_10 = 34;
const byte PIR_11 = 32;
const byte PIR_12 = 30;
const byte PIR_13 = 28;
const byte PIR_14 = 26;
const byte PIR_15 = 24;
const byte PIR_16 = 22;

// how often to check if USBs plugged in or not
// if too low a value, will need to debounce!
uint32_t usb_check_dur = 5000; // 5 sec
uint32_t last_usb_check;
bool is_connected = false;
bool temp_usb = false;
bool usb_1 = false;
bool usb_2 = false;
bool usb_3 = false;
bool usb_4 = false;
bool usb_5 = false;
bool usb_6 = false;
bool usb_7 = false;
bool usb_8 = false;
bool usb_9 = false;
bool usb_10 = false;
bool usb_11 = false;
bool usb_12 = false;
bool usb_13 = false;
bool usb_14 = false;
bool usb_15 = false;
bool usb_16 = false;

// cage USB status PINs
const byte USB_1 = 53;
const byte USB_2 = 51;
const byte USB_3 = 49;
const byte USB_4 = 47;
const byte USB_5 = 45;
const byte USB_6 = 43;
const byte USB_7 = 41;
const byte USB_8 = 39;
const byte USB_9 = 37;
const byte USB_10 = 35;
const byte USB_11 = 33;
const byte USB_12 = 31;
const byte USB_13 = 29;
const byte USB_14 = 27;
const byte USB_15 = 25;
const byte USB_16 = 23;

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("RAD Arduino Nano running");
  
  last_motion_1 = last_motion_2 = last_motion_3 = millis();
  last_motion_3 = last_motion_4 = last_motion_5 = last_motion_6;
  last_motion_6 = last_motion_7 = last_motion_8 = last_motion_9;
  last_motion_9 = last_motion_10 = last_motion_11 = last_motion_12;
  last_motion_12 = last_motion_13 = last_motion_14 = last_motion_15;
  last_motion_15 = last_motion_16;
  
  // sensors
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
  pinMode(PIR_13, INPUT);
  pinMode(PIR_14, INPUT);
  pinMode(PIR_15, INPUT);
  pinMode(PIR_16, INPUT);

  // USB status PINs
  pinMode(USB_1, INPUT);
  pinMode(USB_2, INPUT);
  pinMode(USB_3, INPUT);
  pinMode(USB_4, INPUT);
  pinMode(USB_5, INPUT);
  pinMode(USB_6, INPUT);
  pinMode(USB_7, INPUT);
  pinMode(USB_8, INPUT);
  pinMode(USB_9, INPUT);
  pinMode(USB_10, INPUT);
  pinMode(USB_11, INPUT);
  pinMode(USB_12, INPUT);
  pinMode(USB_13, INPUT);
  pinMode(USB_14, INPUT);
  pinMode(USB_15, INPUT);
  pinMode(USB_16, INPUT);

  // figure out which cages are plugged in at startup
  usb_1 = digitalRead(USB_1);
  usb_2 = digitalRead(USB_2);
  usb_3 = digitalRead(USB_3);
  usb_4 = digitalRead(USB_4);
  usb_5 = digitalRead(USB_5);
  usb_6 = digitalRead(USB_6);
  usb_7 = digitalRead(USB_7);
  usb_8 = digitalRead(USB_8);
  usb_9 = digitalRead(USB_9);
  usb_10 = digitalRead(USB_10);
  usb_11 = digitalRead(USB_11);
  usb_12 = digitalRead(USB_12);
  usb_13 = digitalRead(USB_13);
  usb_14 = digitalRead(USB_14);
  usb_15 = digitalRead(USB_15);
  usb_16 = digitalRead(USB_16);
  last_usb_check = millis();

  // matlab assumes they're unplugged at startup
  if (usb_16) {
    Serial.println("PLUGGED IN CAGE 16");
  }
  if (usb_15) {
    Serial.println("PLUGGED IN CAGE 15");
  }
  if (usb_14) {
    Serial.println("PLUGGED IN CAGE 14");
  }
  if (usb_13) {
    Serial.println("PLUGGED IN CAGE 13");
  }
  if (usb_12) {
    Serial.println("PLUGGED IN CAGE 12");
  }
  if (usb_11) {
    Serial.println("PLUGGED IN CAGE 11");
  }
  if (usb_10) {
    Serial.println("PLUGGED IN CAGE 10");
  }
  if (usb_9) {
    Serial.println("PLUGGED IN CAGE 9");
  }
  if (usb_8) {
    Serial.println("PLUGGED IN CAGE 8");
  }
  if (usb_7) {
    Serial.println("PLUGGED IN CAGE 7");
  }
  if (usb_6) {
    Serial.println("PLUGGED IN CAGE 6");
  }
  if (usb_5) {
    Serial.println("PLUGGED IN CAGE 5");
  }
  if (usb_4) {
    Serial.println("PLUGGED IN CAGE 4");
  }
  if (usb_3) {
    Serial.println("PLUGGED IN CAGE 3");
  }
  if (usb_2) {
    Serial.println("PLUGGED IN CAGE 2");
  }
  // reverse order ends us on cage #1 in the matlab GUI
  if (usb_1) {
    Serial.println("PLUGGED IN CAGE 1");
  }
}

void loop() {

  if ((millis() - last_usb_check) > usb_check_dur) {
    last_usb_check = millis();
    temp_usb = usb_1;
    usb_1 = digitalRead(USB_1);
    if (temp_usb != usb_1) {
      // change detected in USB connection status
      if (usb_1) {
        Serial.println("PLUGGED IN CAGE 1");
      }
      else {
        Serial.println("UNPLUGGED CAGE 1");
      }
    }

    temp_usb = usb_2;
    usb_2 = digitalRead(USB_2);
    if (temp_usb != usb_2) {
      if (usb_2) {
        Serial.println("PLUGGED IN CAGE 2");
      }
      else {
        Serial.println("UNPLUGGED CAGE 2");
      }
    }

    temp_usb = usb_3;
    usb_3 = digitalRead(USB_3);
    if (temp_usb != usb_3) {
      if (usb_3) {
        Serial.println("PLUGGED IN CAGE 3");
      }
      else {
        Serial.println("UNPLUGGED CAGE 3");
      }
    }

    temp_usb = usb_4;
    usb_4 = digitalRead(USB_4);
    if (temp_usb != usb_4) {
      if (usb_4) {
        Serial.println("PLUGGED IN CAGE 4");
      }
      else {
        Serial.println("UNPLUGGED CAGE 4");
      }
    }

    temp_usb = usb_5;
    usb_5 = digitalRead(USB_5);
    if (temp_usb != usb_5) {
      if (usb_5) {
        Serial.println("PLUGGED IN CAGE 5");
      }
      else {
        Serial.println("UNPLUGGED CAGE 5");
      }
    }

    temp_usb = usb_6;
    usb_6 = digitalRead(USB_6);
    if (temp_usb != usb_6) {
      if (usb_6) {
        Serial.println("PLUGGED IN CAGE 6");
      }
      else {
        Serial.println("UNPLUGGED CAGE 6");
      }
    }

    temp_usb = usb_7;
    usb_7 = digitalRead(USB_7);
    if (temp_usb != usb_7) {
      if (usb_7) {
        Serial.println("PLUGGED IN CAGE 7");
      }
      else {
        Serial.println("UNPLUGGED CAGE 7");
      }
    }

    temp_usb = usb_8;
    usb_8 = digitalRead(USB_8);
    if (temp_usb != usb_8) {
      if (usb_8) {
        Serial.println("PLUGGED IN CAGE 8");
      }
      else {
        Serial.println("UNPLUGGED CAGE 8");
      }
    }

    temp_usb = usb_9;
    usb_9 = digitalRead(USB_9);
    if (temp_usb != usb_9) {
      if (usb_9) {
        Serial.println("PLUGGED IN CAGE 9");
      }
      else {
        Serial.println("UNPLUGGED CAGE 9");
      }
    }

    temp_usb = usb_10;
    usb_10 = digitalRead(USB_10);
    if (temp_usb != usb_10) {
      if (usb_10) {
        Serial.println("PLUGGED IN CAGE 10");
      }
      else {
        Serial.println("UNPLUGGED CAGE 10");
      }
    }

    temp_usb = usb_11;
    usb_11 = digitalRead(USB_11);
    if (temp_usb != usb_11) {
      if (usb_11) {
        Serial.println("PLUGGED IN CAGE 11");
      }
      else {
        Serial.println("UNPLUGGED CAGE 11");
      }
    }

    temp_usb = usb_12;
    usb_12 = digitalRead(USB_12);
    if (temp_usb != usb_12) {
      if (usb_12) {
        Serial.println("PLUGGED IN CAGE 12");
      }
      else {
        Serial.println("UNPLUGGED CAGE 12");
      }
    }

    temp_usb = usb_13;
    usb_13 = digitalRead(USB_13);
    if (temp_usb != usb_13) {
      if (usb_13) {
        Serial.println("PLUGGED IN CAGE 13");
      }
      else {
        Serial.println("UNPLUGGED CAGE 13");
      }
    }

    temp_usb = usb_14;
    usb_14 = digitalRead(USB_14);
    if (temp_usb != usb_14) {
      if (usb_14) {
        Serial.println("PLUGGED IN CAGE 14");
      }
      else {
        Serial.println("UNPLUGGED CAGE 14");
      }
    }

    temp_usb = usb_15;
    usb_15 = digitalRead(USB_15);
    if (temp_usb != usb_15) {
      if (usb_15) {
        Serial.println("PLUGGED IN CAGE 15");
      }
      else {
        Serial.println("UNPLUGGED CAGE 15");
      }
    }

    temp_usb = usb_16;
    usb_16 = digitalRead(USB_16);
    if (temp_usb != usb_16) {
      if (usb_16) {
        Serial.println("PLUGGED IN CAGE 16");
      }
      else {
        Serial.println("UNPLUGGED CAGE 16");
      }
    }
  }

  // check cage #1
  is_motion = digitalRead(PIR_1);
  if (is_motion) {
    if (!last_motion_1) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_1) {
        Serial.println("MOTION CAGE 1");
      }
    }
  }
  last_motion_1 = is_motion;

  // check cage #2
  is_motion = digitalRead(PIR_2);
  if (is_motion) {
    if (!last_motion_2) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_2) {
        Serial.println("MOTION CAGE 2");
      }
    }
  }
  last_motion_2 = is_motion;

  // check cage #3
  is_motion = digitalRead(PIR_3);
  if (is_motion) {
    if (!last_motion_3) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_3) {
        Serial.println("MOTION CAGE 3");
      }
    }
  }
  last_motion_3 = is_motion;

  // check cage #4
  is_motion = digitalRead(PIR_4);
  if (is_motion) {
    if (!last_motion_4) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_4) {
        Serial.println("MOTION CAGE 4");
      }
    }
  }
  last_motion_4 = is_motion;

  // check cage #5
  is_motion = digitalRead(PIR_5);
  if (is_motion) {
    if (!last_motion_5) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_5) {
        Serial.println("MOTION CAGE 5");
      }
    }
  }
  last_motion_5 = is_motion;

  // check cage #6
  is_motion = digitalRead(PIR_6);
  if (is_motion) {
    if (!last_motion_6) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_6) {
        Serial.println("MOTION CAGE 6");
      }
    }
  }
  last_motion_6 = is_motion;  

  // check cage #7
  is_motion = digitalRead(PIR_7);
  if (is_motion) {
    if (!last_motion_7) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_7) {
        Serial.println("MOTION CAGE 7");
      }
    }
  }
  last_motion_7 = is_motion;

  // check cage #8
  is_motion = digitalRead(PIR_8);
  if (is_motion) {
    if (!last_motion_8) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_8) {
        Serial.println("MOTION CAGE 8");
      }
    }
  }
  last_motion_8 = is_motion;

  // check cage #9
  is_motion = digitalRead(PIR_9);
  if (is_motion) {
    if (!last_motion_9) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_9) {
        Serial.println("MOTION CAGE 9");
      }
    }
  }
  last_motion_9 = is_motion;

  // check cage #10
  is_motion = digitalRead(PIR_10);
  if (is_motion) {
    if (!last_motion_10) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_10) {
        Serial.println("MOTION CAGE 10");
      }
    }
  }
  last_motion_10 = is_motion;

  // check cage #11
  is_motion = digitalRead(PIR_11);
  if (is_motion) {
    if (!last_motion_11) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_11) {
        Serial.println("MOTION CAGE 11");
      }
    }
  }
  last_motion_11 = is_motion;

  // check cage #12
  is_motion = digitalRead(PIR_12);
  if (is_motion) {
    if (!last_motion_12) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_12) {
        Serial.println("MOTION CAGE 12");
      }
    }
  }
  last_motion_12 = is_motion;

  // check cage #13
  is_motion = digitalRead(PIR_13);
  if (is_motion) {
    if (!last_motion_13) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_13) {
        Serial.println("MOTION CAGE 13");
      }
    }
  }
  last_motion_13 = is_motion;

  // check cage #14
  is_motion = digitalRead(PIR_14);
  if (is_motion) {
    if (!last_motion_14) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_14) {
        Serial.println("MOTION CAGE 14");
      }
    }
  }
  last_motion_14 = is_motion;

  // check cage #15
  is_motion = digitalRead(PIR_15);
  if (is_motion) {
    if (!last_motion_15) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_15) {
        Serial.println("MOTION CAGE 15");
      }
    }
  }
  last_motion_15 = is_motion;

  // check cage #16
  is_motion = digitalRead(PIR_16);
  if (is_motion) {
    if (!last_motion_16) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_16) {
        Serial.println("MOTION CAGE 16");
      }
    }
  }
  last_motion_16 = is_motion;

}
