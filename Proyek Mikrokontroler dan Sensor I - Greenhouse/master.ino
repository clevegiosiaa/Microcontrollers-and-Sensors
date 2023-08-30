// C++ code
//
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int temp, light, moist;
const int m_sens = A0;
const int l_sens = A2;
const int t_sens = A1;
const int btt_pin = 2;
const int l_act = 12;
const int ac_act = 13;
const int rs = 8, en = 3, d4 = 6, d5 = 7, d6 = 4, d7 = 5;
bool pressed = false;
String tap = "Off", lamps = "Off", ac = "Off";
Servo servo5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(20, 4);
  servo5.attach(11);
  pinMode(btt_pin, INPUT_PULLUP);
  pinMode(l_act, OUTPUT);
  pinMode(ac_act, OUTPUT);
  lcd.print("Auto Greenhouse");
  lcd.setCursor(0, 1);
  lcd.print("Env. System");
  delay(500);
  lcd.clear();
  delay(500);
  attachInterrupt(digitalPinToInterrupt(2), lcd_showact, FALLING);
}

void loop()
{
  Wire.beginTransmission(8);
int x = analogRead(l_sens);
  temp = analogRead(t_sens);
  light = 0.0048*pow(x,2) + 5.1035*x - 116.11; 
  moist = analogRead(m_sens); 

  byte dataArray[3];
  
  if (temp < 1020) {
    ac = "On";
    dataArray[0] = 1;
  } else {
    ac = "Off";
    dataArray[0] = 0;
  }
  if (light < 15) {
    lamps = "On";
    dataArray[1] = 1;
  } else {
    lamps = "Off";
    dataArray[1] = 0;
  }
  if (moist < 1000) {
    tap = "On";
    dataArray[2] = 1;
  } else {
    tap = "Off";
    dataArray[2] = 0;
  }
  digitalWrite(13, HIGH);
  Wire.write(dataArray, 3);
  digitalWrite(13, LOW);

  Wire.endTransmission();
  
  Serial.print("Temp:");
  Serial.print(temp);
  Serial.print(", Light:");
  Serial.print(light);
  Serial.print(", Moist:");
  Serial.print(moist);
  Serial.print(", ac_status:");
  Serial.print(ac);
  Serial.print(", lamps_status:");
  Serial.print(lamps);
  Serial.print(", tap_status:");
  Serial.println(tap);
  
  if (!pressed) lcd_showsens(temp, light, moist);
  
  delay(1000);
}

void lcd_showsens(int temp, int light, int moist)
{
  lcd.clear();
  lcd.print("S.M. : ");
  lcd.setCursor(6, 0);
  lcd.print(moist);
  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.setCursor(6, 1);
  lcd.print(light);
  lcd.setCursor(-4, 2);
  lcd.print("Temp :");
  lcd.setCursor(2, 2);
  lcd.print(temp);
}

void lcd_showact()
{
  pressed = !pressed;
  if (pressed) {
    lcd.clear();
    lcd.print("Water:");
    lcd.setCursor(6, 0);
    lcd.print(tap);
    lcd.setCursor(0, 1);
    lcd.print("Lamps:");
    lcd.setCursor(6, 1);
    lcd.print(lamps);
    lcd.setCursor(-4, 2);
    lcd.print("AC   :");
    lcd.setCursor(2, 2);
    lcd.print(ac);
    }
}
