#include <EncoderStepCounter.h>
#include <WiFi.h>

EncoderStepCounter encoder(18, 19);

// Replace with your network credentials
const char* ssid = "AEU-STEM";
const char* password = "aeustem2019";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output27State = "off";
String Relay, Relay2, Relay3 = "off";
bool outputStatus;
bool outputStatus2;

// Assign output variables to GPIO pins
const int output26 = 14;
const int output27 = 15;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  encoder.begin();
  attachInterrupt(18, interrupt, CHANGE);
  attachInterrupt(19, interrupt, CHANGE);
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void interrupt() {
  encoder.tick();
}
long int position = 0;

void loop() {
 
  WiFiClient client = server.available();  // Listen for incoming clients
  signed char pos = encoder.getPosition();
  if (pos != 0) {
    position += pos;
    encoder.reset();
  }
  int round = position / 446;
  if (round == 0 ) {Serial.println(WiFi.localIP());}
  else {Serial.println(round);}
  
  if (client) {  // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                                             // print a message out in the serial port
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      // if there's bytes to read from the client,
      char c = client.read();  // read a byte, then
      Serial.write(c);         // print it out the serial monitor
      header += c;
      if (c == '\n') {  // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTPs/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          // turns the GPIOs on and off

          if (header.indexOf("GET /27/1") >= 0) {
            Serial.println("GPIO 27 on");
            output27State = "on";
            analogWrite(output27, 100);
            analogWrite(output26, 0);
            outputStatus = false;
          } else if (header.indexOf("GET /27/0") >= 0) {
            Serial.println("GPIO 27 off");
            output27State = "off";
            analogWrite(output26, 100);
            analogWrite(output27, 0);
            outputStatus = true;
          } else if (header.indexOf("GET /32/1") >= 0) {
            Serial.println("Relay1 on");
            Relay = "on";
            digitalWrite(32, 1);
          } else if (header.indexOf("GET /32/0") >= 0) {
            Serial.println("Relay1 off");
            Relay = "off";
            digitalWrite(32, 0);
          } else if (header.indexOf("GET /33/1") >= 0) {
            Serial.println("Relay2 on");
            Relay2 = "on";
            digitalWrite(33, 1);
          } else if (header.indexOf("GET /33/0") >= 0) {
            Serial.println("Relay2 off");
            Relay2 = "off";
            digitalWrite(33, 0);
          } else if (header.indexOf("GET /25/1") >= 0) {
            Serial.println("Relay3 on");
            Relay3 = "on";
            digitalWrite(25, 1);
          } else if (header.indexOf("GET /25/0") >= 0) {
            Serial.println("Relay3 off");
            Relay3 = "off";
            digitalWrite(25, 0);
          }

          // Display the HTML web page
          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"icon\" href=\"data:,\">");
          client.println("<title>AEU IOT</title>");
          // CSS to style the on/off buttons
          // Feel free to change the background-color and font-size attributes to fit your preferences
          client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
          client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
          client.println(".button2 {background-color: #555555;}</style></head>");

          // Web Page Heading
          client.println("<body><h1>AEU Smart Home</h1>");
          // Display current state, and ON/OFF buttons for GPIO 27
          client.println("<p>Curtain - State " + output27State + "</p>");
          // If the output27State is off, it displays the ON button
          if (output27State == "off") {
            client.println("<p><a href=\"/27/1\"><button class=\"button\">UP</button></a></p>");
          } else {
            client.println("<p><a href=\"/27/0\"><button class=\"button button2\">DOWN</button></a></p>");
          }
          if (Relay == "off") {
            client.println("<a style=\"display:block-inline; justify-content:left\" href=\"/32/1\"><button class=\"button \">LED1</button></a>");
          } else {
            client.println("<a style=\"display:block-inline; justify-content:left\"  href=\"/32/0\"><button class=\"button button2\">LED1</button></a>");
          }
          if (Relay2 == "off") {
            client.println("<a style=\"display:block-inline; justify-content:center\" href=\"/33/1\"><button class=\"button \">LED2</button></a>");
          } else {
            client.println("<a style=\"display:block-inline; justify-content:center\" href=\"/33/0\"><button class=\"button button2\">LED2</button></a>");
          }
          if (Relay3 == "off") {
            client.println("<a style=\"display:block-inline; justify-content:right\" href=\"/25/1\"><button class=\"button \">LED3</button></a>");
          } else {
            client.println("<a style=\"display:block-inline; justify-content:right\" href=\"/25/0\"><button class=\"button button2\">LED3</button></a>");
          }
          // The HTTP response ends with another blank line
          client.println();
          // Break out of the while loop
          break;
        } else {  // if you got a newline, then clear currentLine
          currentLine = "";
        }
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  if (outputStatus == false && round >= 8) {
    analogWrite(output27, 0);
  } else if (outputStatus == true && round == 0) {
    analogWrite(output26, 0);
  }
}