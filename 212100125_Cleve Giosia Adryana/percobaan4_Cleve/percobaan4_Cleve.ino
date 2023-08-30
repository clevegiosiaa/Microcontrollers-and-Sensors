int aPin = 3;
int adc = 0;
int led =13;
int onoff = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode (led, OUTPUT);
}

void loop()
{
  adc = analogRead(aPin);
  Serial.println(adc);
  if(adc <= 500)
  {
    onoff = HIGH;
  }
  else
  {
    onoff = LOW;
  }
  digitalWrite(led, onoff);
  delay(100);
}
