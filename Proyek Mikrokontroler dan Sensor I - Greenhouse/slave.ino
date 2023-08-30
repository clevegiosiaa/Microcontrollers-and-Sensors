#include <Wire.h>
#include <Servo.h>

byte pinArray[3] = {2, 3, 4};
Servo servo5;

void setup() {
  Wire.begin(8);
  servo5.attach(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  
}

void loop() {
  delay(100); 
}

void receiveEvent(int numBytes) {
  while(Wire.available() > 0) {  
    for (int i=0; i<3; i++) {
      int data = Wire.read();
      Serial.println(data);
      if (i == 2) {
        servo5.write(90 * data);
      }
      digitalWrite(pinArray[i], data);
    
    }
  }
}
