#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <RCSwitch.h>
#define PIN            0  
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);
char auth[] = "my token";
const char* ssid = "Darude Lanstrom";
const char* password = "our password";
RCSwitch mySwitch = RCSwitch();
int motionCount = 0;

void setup()
{
  Serial.begin(9600);
  delay(10);

  // connect to blink
  Blynk.begin(auth, ssid, password);
  Serial.println(Blynk.connected());

  // 433 Mhz radio receiver pin
  mySwitch.enableReceive(4);
  strip.begin();
  strip.show();

  // set time manually: hour, minute, second, day, month, year
  setTime(15,1,0,20,2,2017);
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
  Serial.println(motionCount);
  Blynk.virtualWrite(V7, motionCount);
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
  // clock/random based color to Neopixel ring
   for(int t=0; t<strip.numPixels(); t++) {
    strip.setPixelColor(t, hour() * random(1, 2), minute() * random(1, 2), second() * random(1, 2));
    strip.show();
    }

  // if motion detected, call motionDetected function
  if (int(mySwitch.available()) == 1) {
    motionDetected();
  } 

  // run, reset and wait for a seconds
  Blynk.run();
  delay(100); // not to overload blynk I guess
  mySwitch.resetAvailable();
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
