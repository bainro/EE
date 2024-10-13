#include "WiFi.h"
#include "HTTPClient.h"

const char* ssid = "D3AB8E";
const char* password =  "4CE156WJ01BBE";
const char builtin_LED = 2;
const int v_div_pin = 34;
float batt_v = 0;
bool is_recording = false;
bool is_motion = false;
uint16_t motion_cooldown = 5000; // 5 seconds
uint32_t last_motion;
String animalId = "unspecified_mouse";
const byte PIR_pin = 2; // IR motion sensor
String currentLine = ""; // holds incoming data from the client

NetworkServer server(80);

void setup() {
  Serial.begin(115200);
  last_motion = millis();
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
  if (is_recording) {
    is_motion = digitalRead(PIR_pin);
    if (is_motion) {
      if ((millis() - last_motion) > motion_cooldown) {
        last_motion = millis();
        Serial.println("MOTION DETECTED!");
        HTTPClient http;
        String urlWithParams = "http://192.168.0.2:8080/motion?animal_id=" + animalId;
        http.begin(urlWithParams.c_str());
        //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int response = http.GET();
        http.end();
      }
    }
  }

  NetworkClient client = server.accept();  // listen for incoming clients
  if (client) {                     // if you get a client,
    Serial.println("\nNew Client.");  // print a message out the serial port
    
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
            client.print("Click <a href='/start'>here</a> to start recording motion.<br>");
            client.print("Click <a href='/stop'>here</a> to stop recording motion.<br>");
            client.print("Click <a href='/recording'>here</a> to check if recording.<br>");

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
          // read the battery voltage pin w/ 12bit precision & max 3.3V
          batt_v = ((float)analogRead(v_div_pin)) / 4095 * 3.3;
          // convert it to the true voltage given our voltage divider
          batt_v = batt_v * 3.13; // extra 13% from experimental measures
          // the content of the HTTP response follows the header:
          client.print(String(batt_v, 2));
          // The HTTP response ends with another blank line:
          client.println();
          break; // don't send default HTTP response
        } else if (currentLine.endsWith("GET /recording")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          if (is_recording) {
            client.println("true");
          }
          else {
            client.println("false");
          }
          client.println();
          break; // don't send default HTTP response
        } else if (currentLine.endsWith("GET /start")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          is_recording = true;
          client.println("acknowledged");
          client.println();
          break; // don't send default HTTP response
        } else if (currentLine.endsWith("GET /stop")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          is_recording = false;
          client.println("acknowledged");
          client.println();
          break; // don't send default HTTP response
        } else if (currentLine.endsWith("GET /animal")) {
          // set animal ID for logging motion events
          c = client.read();
          animalId = "";
          // the rest of the requested URL is the animal ID
          // delimited with a carriage return
          while (c != '\r' && c != '\n' && c != ' ') {
            animalId += c;
            c = client.read();
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("acknowledged");
          client.println();
          break; // don't send default HTTP response
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
