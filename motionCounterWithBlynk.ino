#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <RCSwitch.h>
#include <WidgetRTC.h>
#define PIN            0  
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);
char auth[] = "auth";
const char* ssid = "Darude Lanstrom";
const char* password = "password";
RCSwitch mySwitch = RCSwitch();
int motionCountAll = 1500;
int motionCountHour = 50;
int motionCountDay = 710;
int currentHour;
int currentDay;

WidgetRTC rtc;

void setup()
{
  Serial.begin(9600);
  delay(10);
  mySwitch.enableReceive(4);  // Receiver on interrupt 0 => that is pin #2

  // connect to blink
  Blynk.begin(auth, ssid, password);
  Serial.println(Blynk.connected());
  
  // neopixels
  strip.begin();
  strip.show();

  // set time manually: hour, minute, second, day, month, year
  //setTime(18,0,0,20,2,2017);
  // set time from Blynk
  rtc.begin();
  Serial.println("Time is: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  
}



// neopixel example: color from slider
BLYNK_WRITE(V1)
{
  int shift = param.asInt();
  Serial.println(shift);
  for (int i = 0; i < strip.numPixels(); i++)
  { strip.setPixelColor(i, Wheel(shift & 255)); }
  strip.show();
  delay(10000);
}

// Simulate motion event
BLYNK_WRITE(V2)
{
  int btn = param.asInt();
  Serial.println(btn);
  motionDetected();
}

// send motion count to blynk and because interactive, display rainbowcycle on neopixel
void  motionDetected() {
  
  motionCountAll = motionCountAll + 1;
  Blynk.virtualWrite(V7, motionCountAll);
  Blynk.virtualWrite(V8, String(hour()) + ":" + String(minute()) + ":" + String(second()) );

  if (hour() == currentHour) {
    motionCountHour = motionCountHour + 1;
    Serial.print("Motion count last hour: ");
    Serial.println(motionCountHour);
    Blynk.virtualWrite(V9, motionCountHour);
  } else {
    currentHour = hour();
  }

    if (day() == currentDay) {
      motionCountDay = motionCountDay + 1;
      Serial.print("Motion count last day: ");
      Serial.println(motionCountDay);
      Blynk.virtualWrite(V10, motionCountDay);
  } else {
    currentDay = day();
  }

  Serial.println("Time of the event was: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.println("back to scanning");
  
  Blynk.run();
  rainbowCycle(5);
}

// after rainbow turn off leds
void turnOffLeds() {
   for(int k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(k, 0, 0, 0);
    strip.show();
  }
}

// main loop
void loop()
{
  

  // if motion detected, call motionDetected function
  if (mySwitch.available()) {
    Serial.println("Motion detected: " + String(motionCountAll));
    motionDetected();
    mySwitch.resetAvailable();
  } else {
    // clock/random based color to Neopixel ring
   for(int t=0; t<strip.numPixels(); t++) {
    strip.setPixelColor(t, hour() * random(1, 2), minute() * random(1, 2), second() * random(1, 2));
    strip.show();
    }
  }

  //Serial.println(mySwitch.available());
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.println("");

  if (hour() == 18 && minute() == 47 && second() == 15) {
   Serial.print("IFTTT event fired with payload of :");
   Serial.println(motionCountDay);
   Blynk.virtualWrite(V20, motionCountDay);
  }

  // run, reset and wait for a seconds
  
  delay(1000); // not to overload blynk I guess
  Blynk.run();
}


// From NeoPixel examples
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  turnOffLeds();
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
