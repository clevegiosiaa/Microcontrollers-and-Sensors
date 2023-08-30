

int sensorPower = 7;
int sensorPin = 3;

void setup() {
	pinMode(sensorPower, OUTPUT);
	
	digitalWrite(sensorPower, LOW);
	
	Serial.begin(9600);
}

void loop() {
	
	Serial.print("Analog output: ");
        digitalWrite(sensorPower, HIGH);								
	int val = analogRead(sensorPin);	
	digitalWrite(sensorPower, LOW);		
	Serial.println(val);
}

