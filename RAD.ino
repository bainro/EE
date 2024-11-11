#include <Wire.h> 


uint16_t motion_cooldown = 5000; // 5 sec
bool is_motion = false;
uint32_t last_motion_1;
uint32_t last_motion_2;
uint32_t last_motion_3;
uint32_t last_motion_4;
uint32_t last_motion_5;
uint32_t last_motion_6;

// cage motion sensors pins
const byte PIR_1 = 13;
const byte PIR_2 = 2;
const byte PIR_3 = 3;
const byte PIR_4 = 4;
const byte PIR_5 = 5;
const byte PIR_6 = 6;

// how often to check if USBs plugged in or not
// if too low a value, will need to debounce!
uint32_t usb_check_dur = 5000; // 5 sec
uint32_t last_usb_check;
bool is_connected = false;
bool temp_usb = false;
bool usb_1 = false;

// cage USB status PINs
const byte USB_1 = 7;

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("RAD Arduino Nano #1 running");
  last_usb_check = millis();
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

  // figure out which cages are plugged in at startup
  usb_1 = digitalRead(USB_1);
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
  }

  // check cage #1
  is_motion = digitalRead(PIR_1);
  if (is_motion) {
    if ((millis() - last_motion_1) > motion_cooldown) {
      // see if it's actually plugged in since no pulldown Rs
      if (usb_1) {
        last_motion_1 = millis();
        Serial.println("MOTION CAGE 1");
      }
    }
  }

}