#include <RCSwitch.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

int d1 = 5; // GPIO1
int d2 = 4; // GPIO2
int d3 = 0; // GPIO3
int d4 = 2; // GPIO2
int val = 0;
int alarmFlag = 0;

const char* ssid = "Talli 1";
const char* password = "******************************";
WiFiServer server(80);
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  pinMode(d2, INPUT); // microphone
  mySwitch.enableTransmit(d4); // 433 MHz transmitter
  mySwitch.setProtocol(1);
  mySwitch.setRepeatTransmit(15);
  delay(1000);

  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  Serial.println(WiFi.localIP());  
}

void loop() {
  
  // read the microphone DO 
  val = digitalRead(d2);  

  // if microphone hears fire alarm beeping or other loud noises
  if (val == 1 or alarmFlag == 1) {

    // HTTP GET to IFTTT
    HTTPClient http;
    http.begin("http://maker.ifttt.com/trigger/fireAlarm/with/key/uxuNVDMsQlK_wHre3ksvX");
    int httpCode = http.GET(); 
    Serial.println(httpCode);
    http.end();

    // wait for trigger request to be sent, not sure if actually necessary
    delay(1000);

    // actual message to remote controller outled to shut down
    mySwitch.send(5588305, 24);

    // to get back to this loop
    alarmFlag = 1;
  } 

  // make sure that the remote controlled outlet stays on if everything ok
  if (val == 0 and alarmFlag != 1) {
    mySwitch.send(5588308, 24);
  }

  // end of loop
}
