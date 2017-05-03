#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
unsigned long previousMillis = 0;  
unsigned long prev = 0; 
int BUTTON_PIN = D1; //button is connected to GPIO pin D1
int state = 0;
int status;

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(4);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  ajastin();

  status = digitalRead(BUTTON_PIN);
  //Serial.println(status);

  if (status == 1) {

    Serial.println("toggling...");

    if (state == 1) {
      mySwitch.send(5588308, 24);
      state = 0;
    } else {
      mySwitch.send(5588305, 24);
      state = 1;
    }
    
    delay(1000);
  }
}

/* receiving
if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  }

}

/// blinking 433 outlet
void blink433() {
  mySwitch.send(5588308, 24);
  delay(1000);  
  mySwitch.send(5588305, 24);
  delay(1000); 
}
*/


///// for automatic shutoff
void ajastin() {
unsigned long curr = millis();
  if(curr - prev > 900000) { // 15 mins
    Serial.println(curr);
    prev = curr;
    mySwitch.send(5588305, 24);
  }
}
