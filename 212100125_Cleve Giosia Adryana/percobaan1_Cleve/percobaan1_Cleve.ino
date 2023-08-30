const int led1 = 13;
String datamasukan;

void setup()
{
  Serial.begin(9600);
  
  pinMode(led1, OUTPUT);
  Serial.println("serial siap");
}

void loop()
{
  if (Serial.available() > 0)  {
    datamasukan = Serial.readStringUntil('\n');
    
    if (datamasukan == "OFF" || datamasukan == "off"){
      digitalWrite(led1, LOW);}
    else if(datamasukan == "ON" || datamasukan =="on"){
      digitalWrite(led1, HIGH);}
  
    Serial.print("Anda mengetikkan: ");
    Serial.println(datamasukan);
  }
}
