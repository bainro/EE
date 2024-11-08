// note that most serial prints are hard-coded in Matlab
// i.e. do not change them without changing Matlab too

#include <Wire.h> 

//pin assignments
const byte top_button = 2; // toggle motion sensor
const byte bot_button = 3; // toggle motion sensor
const byte reward_pin_top = 4; // solenoid transistor
const byte reward_pin_bot = 5; // solenoid transistor
const byte top_sensor1 = 6; // pre-reward gate
const byte top_sensor2 = 7; // post-reward gate
const byte bot_sensor1 = 8; // pre-reward gate
const byte bot_sensor2 = 9; // post-reward gate

uint16_t reward_cooldown = 4000;
// for debounce. Min time between presses
uint16_t btn_press_dt = 1000;

// temporary variables
unsigned int temp;
unsigned int templow;
unsigned int temphigh;
uint8_t  tempb;
uint8_t tempPIR;

uint32_t cur_time;
uint32_t last_top_btn; // for software debounce
uint32_t last_bot_btn; // for software debounce
uint32_t sessionStartTime;
uint32_t reward_start_time_top;
uint32_t reward_start_time_bot;
uint32_t manual_R_start_time_T;
uint32_t manual_R_start_time_B;
uint16_t reward_duration_top = 100; // @TODO communicate this with matlab!
uint16_t reward_duration_bot = 100; // @TODO communicate this with matlab!

bool manual_reward_top = false;
bool manual_reward_bot = false;
bool solenoid_status_T = false; // used for open/close
bool solenoid_status_B = false; 
// states of the state machines
bool top_is_paused = true; // Whether the states are locked
bool top_in_state1 = true; // init state / cooldown period
bool top_in_state2 = false; // motion before gate
bool top_in_state3 = false; // give reward, then back to state1
bool bot_is_paused = true;
bool bot_in_state1 = true;
bool bot_in_state2 = false;
bool bot_in_state3 = false;

void setup() {
  // Initialize Serial Connections
  Serial.begin(115200);         //  Initialize serial connection
  Wire.begin();                 //  join i2c bus (address optional for master)
  Serial.println("Arduino for Table #3 is running");

  // PIR motion sensor is determined is an input here.  
  pinMode(top_button, INPUT);
  pinMode(bot_button, INPUT);
  pinMode(top_sensor1, INPUT);
  pinMode(top_sensor2, INPUT);
  pinMode(bot_sensor1, INPUT);
  pinMode(bot_sensor2, INPUT);
  pinMode(reward_pin_top, OUTPUT);
  pinMode(reward_pin_bot, OUTPUT);

  sessionStartTime = millis();
}

void loop() {
  if (Serial.available()) {
    decode_serial_data();
  }
    
  cur_time = millis() - sessionStartTime;

  tempPIR = digitalRead(top_button);
  // software button debouncing
  temp = (cur_time - last_top_btn > btn_press_dt); 
  if (tempPIR && temp) {
    last_top_btn = cur_time;
    Serial.println("[TOP] button pressed!");
  }

  tempPIR = digitalRead(bot_button);
  // software button debouncing
  temp = (cur_time - last_bot_btn > btn_press_dt); 
  if (tempPIR && temp) {
    last_bot_btn = cur_time;
    Serial.println("[BOTTOM] button pressed!");
  }

  // check if manual reward needs to end
  if (manual_reward_top) {
    if ((millis() - manual_R_start_time_T) > reward_duration_top) {
      manual_reward_top = false;
      digitalWrite(reward_pin_top, LOW);
      Serial.println("[T] manual reward turned off");
    }
  }
  if (manual_reward_bot) {
    if ((millis() - manual_R_start_time_B) > reward_duration_bot) {
      manual_reward_bot = false;
      digitalWrite(reward_pin_bot, LOW);
      Serial.println("[B] manual reward turned off");
    }
  }

  ///////////////////////////////////
  // "state" machine for top track //
  ///////////////////////////////////

  if (!top_is_paused) {
    if (top_in_state1) {
      tempPIR = digitalRead(top_sensor1); // read pre-gate PIR
      if (tempPIR == HIGH) {           
        top_in_state1 = false;
        top_in_state2 = true;
        Serial.println("[TOP]: pre-gate motion detected");
        Serial.println("[TOP]: entering state 2"); 
      }
    }
    else if (top_in_state2) {
      tempPIR = digitalRead(top_sensor2);
      if (tempPIR == HIGH) {           
        // dispense reward!
        top_in_state2 = false;
        top_in_state3 = true;
        Serial.println("[TOP]: post-gate motion detected"); 
        Serial.println("[TOP]: entering state 3"); 
        reward_start_time_top = millis();
        digitalWrite(reward_pin_top, HIGH);
        Serial.println("[TOP]: reward dispensed!"); 
      }
    }  
    else if (top_in_state3) {
      // check if rewards should be turned off
      if (digitalRead(reward_pin_top) == HIGH) {
        if ((millis() - reward_start_time_top) > reward_duration_top) {
          digitalWrite(reward_pin_top, LOW);
          Serial.println("[TOP]: reward turned off");
        }
      }
      // prevent long trigger of pre-gate PIR sensor from early state transition
      if ((millis() - reward_start_time_top) > reward_cooldown) {
        top_in_state3 = false;
        top_in_state1 = true;
        Serial.println("[TOP]: entering state 1");
      }
    }
    else {
      Serial.println("[TOP]: ERROR! Invalid state!"); // sanity check
    }
  }

  //////////////////////////////////////
  // "state" machine for bottom track //
  //////////////////////////////////////

  if (!bot_is_paused) {  
    if (bot_in_state1) {
      tempPIR = digitalRead(bot_sensor1); // read pre-gate PIR
      if (tempPIR == HIGH) {           
        bot_in_state1 = false;
        bot_in_state2 = true;
        Serial.println("[BOTTOM]: pre-gate motion detected");
        Serial.println("[BOTTOM]: entering state 2"); 
      }
    }
    else if (bot_in_state2) {
      tempPIR = digitalRead(bot_sensor2);
      if (tempPIR == HIGH) {           
        // dispense reward!
        bot_in_state2 = false;
        bot_in_state3 = true;
        Serial.println("[BOTTOM]: post-gate motion detected"); 
        Serial.println("[BOTTOM]: entering state 3"); 
        reward_start_time_bot = millis();
        digitalWrite(reward_pin_bot, HIGH);
        Serial.println("[BOTTOM]: reward dispensed!"); 
      }
    }  
    else if (bot_in_state3) {
      // check if rewards should be turned off
      if (digitalRead(reward_pin_bot) == HIGH) {
        if ((millis() - reward_start_time_bot) > reward_duration_bot) {
          digitalWrite(reward_pin_bot, LOW);
          Serial.println("[BOTTOM]: reward turned back off");
        }
      }
      // prevent long trigger of pre-gate PIR sensor from early state transition
      if ((millis() - reward_start_time_bot) > reward_cooldown) {
        bot_in_state3 = false;
        bot_in_state1 = true;
        Serial.println("[BOTTOM]: entering state 1");
      }
    }
    else {
      Serial.println("[BOTTOM]: ERROR! Invalid state!"); // sanity check
    }
  }

} // loop

///////////////////////////////////
//      FUNCTION DEFINITIONS     //
///////////////////////////////////

// read a byte from matlab
uint8_t readbyte()
{
  while (!Serial.available()) {}
  tempb = Serial.read();
  return tempb;
}

// read a 16-bit word from matlab
uint16_t readword()
{
  while (!Serial.available()) {}
  templow = Serial.read();
  while (!Serial.available()) {}
  temphigh = Serial.read();
  temphigh = templow + 256*temphigh;
  return temphigh;
}

// read and decode serial data from matlab
void decode_serial_data() {
  /*
  'a'   pause/resume state updating 
  't'   reset time
  'r'   reward
  */

  tempb = Serial.read(); // reads a byte from matlab

  // act on the byte just received from matlab
  if (tempb == 'a') {
    temphigh = readbyte();
    templow = readbyte();
    
    if (temphigh == 1) { // top
      // prevents reward from getting stuck on
      digitalWrite(reward_pin_top, LOW);
      if (templow == 0) {
        top_is_paused = true;
        Serial.println("arduino pause top state updates ack");
      } else {
        top_is_paused = false;
        Serial.println("arduino resume top state updates ack");
      }
    } else { // bottom
      // prevents reward from getting stuck on
      digitalWrite(reward_pin_bot, LOW);
      if (templow == 0) {
        bot_is_paused = true;
        Serial.println("arduino pause bottom state updates ack");
      } else {
        bot_is_paused = false;
        Serial.println("arduino resume bottom state updates ack");
      }
    }
    
    return;
  }
  
  // update reward solenoid duration from matlab GUI
  if (tempb == 't') {
    while (!Serial.available()) {}
    tempb = Serial.read();
    while (!Serial.available()) {}
    templow = Serial.read();
    while (!Serial.available()) {}
    temphigh = Serial.read();

    if (tempb == 0) { // top
      reward_duration_top = templow + 256*temphigh; 
      Serial.println("[T] arduino R dur change ack");
    }
    else { // bottom
      reward_duration_bot = templow + 256*temphigh;
      Serial.println("[B] arduino R dur change ack");
    }

    return;
  }
  
  // respond to manual reward buttons from matlab
  if (tempb == 'r') {
    while (!Serial.available()) {}
    tempb = Serial.read();

    if (tempb == 0) { // top
      digitalWrite(reward_pin_top, HIGH);
      manual_R_start_time_T = millis();
      Serial.println("[T] arduino manual reward ack");
      manual_reward_top = true;
    }
    else { // bottom
      digitalWrite(reward_pin_bot, HIGH);
      manual_R_start_time_B = millis();
      Serial.println("[B] arduino manual reward ack");
      manual_reward_bot = true;
    }

    return;
  }

  // respond to solenoid open and close calls
  if (tempb == 's') {
    while (!Serial.available()) {}
    tempb = Serial.read();

    if (tempb == 0) { // top
      solenoid_status_T = !solenoid_status_T;
      if (solenoid_status_T) {
        Serial.println("[T] arduino solenoid opened");
        digitalWrite(reward_pin_top, HIGH);
      } else {
        Serial.println("[T] arduino solenoid closed");
        digitalWrite(reward_pin_top, LOW);
      }
    }
    else { // bottom
      solenoid_status_B = !solenoid_status_B;
      if (solenoid_status_B) {
        Serial.println("[B] arduino solenoid opened");
        digitalWrite(reward_pin_bot, HIGH);
      } else {
        Serial.println("[B] arduino solenoid closed");
        digitalWrite(reward_pin_bot, LOW);
      }
    }

    return;
  }
}
