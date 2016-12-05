#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            13  
#define NUMPIXELS      24
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100;
const char* ssid = "Pullonkaula";
const char* password = " **** SALASANA TÄHÄN **** ";
WiFiServer server(80);

/////////////
// SETUP //
////////////
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pixels.begin(); // This initializes the NeoPixel library.
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

//////////
// LOOP //
//////////
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
if (request.indexOf("/LED=tuplajallu") != -1)  {
  for(int j=0;j<=50;j++){
    turnAllWhite();
    delay(delayval/10);
    turnAllOff();
    delay(delayval/10);    
 }
}
  
if (request.indexOf("/LED=ttyspecial") != -1)  {
  colorRunBlue();
}

if (request.indexOf("/LED=gintonic") != -1)  {
  colorRunWhite();
}

if (request.indexOf("/LED=pkerikoinen") != -1)  {
  twinkleRandom();
}
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br><br>");
  client.println("<a href=\"/LED=tuplajallu\"\"><button>Tuplajallu</button></a>");
  client.println("<a href=\"/LED=ttyspecial\"\"><button>TTY Special</button></a><br />");
  client.println("<a href=\"/LED=gintonic\"\"><button>Gin & Tonic</button></a><br />"); 
  client.println("<a href=\"/LED=pkerikoinen\"\"><button>Pullonkaulan Erikoinen</button></a><br />"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}

//////////////////
// LEDIFUNKTIOT //
//////////////////
void turnAllWhite(){
  for(int j=0;j<=NUMPIXELS;j++){
      pixels.setPixelColor(j, pixels.Color(255,255,255)); 
      pixels.show();
      //delay(delayval);
 }
}
///////////////////////////////////////////////////////////////////// 
 void turnAllOff(){
  for(int j=0;j<NUMPIXELS;j++){
     pixels.setPixelColor(j, pixels.Color(0,0,0));
     pixels.show();
  }
 }
///////////////////////////////////////////////////////////////////// 
void colorRunBlue(){
  for(int j=0;j<NUMPIXELS;j++){
      pixels.setPixelColor(j, pixels.Color(0,0,255)); 
      pixels.show();
      delay(delayval);
  } 
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 
      pixels.show();
      delay(delayval);
   } 
 }
///////////////////////////////////////////////////////////////////// 
void colorRunWhite(){
  for(int j=0;j<NUMPIXELS;j++){
      pixels.setPixelColor(j, pixels.Color(255,255,255)); 
      pixels.show();
      delay(delayval);
  } 
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 
      pixels.show();
      delay(delayval);
   } 
 }
///////////////////////////////////////////////////////////////////// 
void twinkleRandom() {
  for (int i=0; i<200; i++) {
     int Pixel = random(NUMPIXELS);
     pixels.setPixelColor(Pixel,random(0,255),random(0,255),random(0,255));
     pixels.show();
     delay(delayval);
     pixels.setPixelColor(Pixel,0,0,0);
   } turnAllOff();
}
