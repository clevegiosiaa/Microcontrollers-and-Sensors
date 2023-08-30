int bPin = 5;
int aPin = 6;
int buttonPin = 7;
int ledPin = 13;

int vala = 0;
int valb = 0;
int newa = 0;
int buttonState = 0;
unsigned long lastButtonPress = 0;

String cw = "Clockwise";
String ccw = "Counter-Clockwise";

void setup()
{
  Serial.begin(9600);
  vala = digitalRead(aPin);
  valb = digitalRead(bPin);
  newa = digitalRead(aPin);
  pinMode(buttonPin, INPUT_PULLUP);  
}

void loop()
{ 
  buttonState = digitalRead(buttonPin);
  newa = digitalRead(aPin);
  valb = digitalRead(bPin);
  if (newa != vala) {
    vala = newa;  
    if (vala != valb){
      Serial.println(cw);
    } else {
      Serial.println(ccw);
    }
  }
  if (buttonState == LOW){
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button Pressed!");
    }
    lastButtonPress = millis();
  }
}
