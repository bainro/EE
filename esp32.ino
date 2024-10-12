#include "WiFi.h"

const char* ssid = "D3AB8E";
const char* password =  "4CE156WJ01BBE";
const char builtin_LED = 2;
const int v_div_pin = 34;
float batt_v = 0;
const byte PIR_pin = 2; // IR motion sensor
String currentLine = ""; // holds incoming data from the client

NetworkServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(builtin_LED, OUTPUT);  // set the LED pin mode
  pinMode(v_div_pin, INPUT); // for reading battery voltage
  pinMode(PIR_pin, INPUT);
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href='/battery'>here</a> to check battery.<br>");

            // The HTTP response ends with another blank line:
            client.println();

            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see what the client request was
        if (currentLine.endsWith("GET /battery")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // read the battery voltage pin
          batt_v = ((float)analogRead(v_div_pin)) / 4095 * 3.3;
          // convert it to the true voltage given our voltage divider
          batt_v = batt_v * 3.13; // extra 13% from experimental measures

          // the content of the HTTP response follows the header:
          client.print("Battery voltage is: " + String(batt_v, 2) + "V.<br>");
          client.print("Click <a href='/battery'>here</a> to check the latest battery volatage.<br>");

          bool is_motion = digitalRead(PIR_pin);
          if (is_motion) {
            client.println("MOTION DETECTED!");
          }
          else {
            client.println("no motion detected ...");
          }

          // The HTTP response ends with another blank line:
          client.println();
          digitalWrite(builtin_LED, HIGH);  // turn the LED on
          break; // don't send default HTTP response
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
