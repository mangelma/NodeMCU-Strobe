#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

const int relayPin = 4; // D2
const int ledPin = 5; // D1
const int interval = 1000; // for blinking without delay
int ledState = LOW;
unsigned long previousMillis = 0;  
unsigned long prev = 0; 
int NUMPIXELS = 1;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_RGBW + NEO_KHZ800);
const char* ssid = "Pullonkaula";

// CHANGE HERE
const char* password = "password"; // CHANGE HERE
// CHANGE HERE

WiFiServer server(80);

 
void setup() {
 // setTime(10); 
  Serial.begin(115200);
  delay(10);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    connectionError();
    delay(500);
  }
  Serial.print("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  pixels.begin();
  connectedBlink();
}

 
void loop() {

  ajastin(); // automatic shut off

  if (WiFi.status() != WL_CONNECTED) {
    connectionError(); // makes ws2812 red if connection is lost
  }

  if (digitalRead(relayPin) == 1) {
   connectedBlink(); // if light is turned off
   }

  if (digitalRead(relayPin) == 0) {
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // if led strip is on, ws2812 green
    pixels.show();
   }

  // client stuff from this on
  WiFiClient client = server.available();
  if (!client) { return; }
  // if no client, not continued
  
 
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  if (request.indexOf("/ON") > 0)  {
    digitalWrite(relayPin, LOW);
  }

  if (request.indexOf("/OFF") > 0)  {
      digitalWrite(relayPin, HIGH); 
  }
   
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // important
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<a href=\"/ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/OFF\"\"><button>Turn Off </button></a><br />");
  client.println("<a href=\"www.pullonkaula.net\"\">(c) Pullonkaula IoT Deparment</button></a><br />"); 
  client.println("</html>"); 
  //delay(1000);
  
} // end of loop()

// In case of e.g. lost connection, do something
void connectionError() {
  pixels.setPixelColor(0, pixels.Color(0,255,0)); // RED
  pixels.show(); 
}

// TODO: make some fade instead of blinking
void connectedBlink() {
unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    if (ledState == LOW) {
      ledState = HIGH;
      pixels.setPixelColor(0, pixels.Color(0,0,255)); }
    else {
      ledState = LOW;
      pixels.setPixelColor(0, pixels.Color(0,0,0)); 
    }
     pixels.show(); 
  }
}


///// for automatic shutoff
void ajastin() {
unsigned long curr = millis();
  if(curr - prev > 900000) { // 15 mins
    prev = curr;
    digitalWrite(relayPin, HIGH);     
  }
}

