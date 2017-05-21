//mySwitch.send(5588305, 24); // TO TURN OFF, BE CAREFUL

#include <RCSwitch.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

int d1 = 5; // GPIO1
int d2 = 4; // GPIO2
int d3 = 0; // GPIO3
int d4 = 2; // GPIO2
int val = 0;
int alarmFlag = 0;
int mswitch = 0;
int msecs = 0;
int prev = 0;
int mswitchcount = 0;
int miccount = 0;
int ledState = 0;
//char trigger = "null";

const char* ssid = "Talli 1";
const char* password = "***********";
WiFiServer server(80);
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(d2, INPUT); // microphone
  pinMode(d3, INPUT); // microswitch
  digitalWrite(d3, HIGH);
  mySwitch.enableTransmit(d4); // 433 MHz transmitter
  mySwitch.setProtocol(1);
  mySwitch.setRepeatTransmit(15);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  Serial.println(" Connected, IP: ");
  Serial.println(WiFi.localIP()); 
   digitalWrite(LED_BUILTIN, HIGH);
   
}

void loop() {
 
  // read the digital pins
  val = digitalRead(d2);  // microphone
  mswitch = digitalRead(d3);

  // if print is ready (bed lowers and hits micro switch)
  if (mswitch == 0) {

    digitalWrite(LED_BUILTIN, LOW);
    mswitchcount = mswitchcount + 1;
    Serial.print("Microswitch pressed for: ");
    Serial.println(mswitchcount);

    delay(1000);

    // ten seconds pressed
    if (mswitchcount == 5) {

        Serial.println("print ready!");
        iftttReq("printReady");
     
        }
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    mswitchcount = 0;
    //Serial.println("Microswitch counter reset");// reset for counter 
    //Serial.println(mswitchcount);
  } // microswitch part ends


  
  // MICROPHONE
  if (val == 1 or alarmFlag == 1) {

    digitalWrite(LED_BUILTIN, LOW);
    miccount = miccount + 1;
    Serial.print("miccount: ");
    Serial.println(miccount);

    if (miccount == 10) {
       
       Serial.println("alarm!");

        // straight to notification
        iftttReq("fireAlarm");

        // then to cooldown but with short delay
        cooldownTimer(1000);
        
    }

   
    delay(500);
  } else {
    digitalWrite(LED_BUILTIN, HIGH); 
  }


// not using this because it disturbs other 433 MHz devices
  /* make sure that the remote controlled outlet stays on if everything ok
  if (val == 0 and alarmFlag != 1) {
    mySwitch.send(5588308, 24);
  } */

 resetTimer();
}

void iftttReq(String trigger) {
    Serial.print("Triggering: ");
    Serial.println(trigger);
    HTTPClient http;
    http.begin("http://maker.ifttt.com/trigger/" + trigger + "/with/key/**********************************");
    int httpCode = http.GET(); 
    Serial.println(httpCode);
    http.end();

    // printReady only turns the power off if notification is successfull:
    // fireAlarm goes straight to cooldownTimer
    if (httpCode < 300) {
          Serial.println("Request went through, going to cooldown..");
          cooldownTimer(60000);
        }     
}


void cooldownTimer(int msecs) {

  // blink led for visible feedback
  for(int i=0;i<10;i++) {
      Serial.print("Timer");
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
  }

/*
  // loop until power is switched off
  while (true) {
     
     Serial.print("Timer starting ");
     Serial.println(msecs);
     delay(msecs);
     Serial.println("Timer elapsed, shutting down...");

      //mySwitch.send(5588305, 24); // TO TURN OFF, BE CAREFUL
     
  }

  */
}

void resetTimer() {

  // current milliseconds since startup
  unsigned long curr = millis();

  // if over x milliseconds passed since previous reset
  if(curr - prev > 20000) {
    
    Serial.println("Resetting miccount");
    miccount = 0;
    prev = curr;

  }
}
