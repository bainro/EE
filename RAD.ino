#include <Wire.h> 

bool is_motion = false;
bool last_motion_1;
bool last_motion_2;
bool last_motion_3;
bool last_motion_4;
bool last_motion_5;
bool last_motion_6;

// cage motion sensors pins
const byte PIR_1 = 11; // @TODO FIX WHEN GOING TO MEGA!!!
const byte PIR_2 = 2;
const byte PIR_3 = 3;
const byte PIR_4 = 4;
const byte PIR_5 = 13;
const byte PIR_6 = 6;

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

// cage USB status PINs
const byte USB_1 = 7;
const byte USB_2 = 8;
const byte USB_3 = 9;
const byte USB_4 = 10;
const byte USB_5 = 5;
const byte USB_6 = 12;

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("RAD Arduino Nano running");
  
  last_motion_1 = last_motion_2 = last_motion_3 = millis();
  last_motion_3 = last_motion_4 = last_motion_5 = last_motion_6;
  
  // sensors
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(PIR_3, INPUT);
  pinMode(PIR_4, INPUT);
  pinMode(PIR_5, INPUT);
  pinMode(PIR_6, INPUT);

  // USB status PINs
  pinMode(USB_1, INPUT);
  pinMode(USB_2, INPUT);
  pinMode(USB_3, INPUT);
  pinMode(USB_4, INPUT);
  pinMode(USB_5, INPUT);
  pinMode(USB_6, INPUT);

  // figure out which cages are plugged in at startup
  usb_1 = digitalRead(USB_1);
  usb_2 = digitalRead(USB_2);
  usb_3 = digitalRead(USB_3);
  usb_4 = digitalRead(USB_4);
  usb_5 = digitalRead(USB_5);
  usb_6 = digitalRead(USB_6);
  last_usb_check = millis();

  // matlab assumes they're unplugged at startup
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

}