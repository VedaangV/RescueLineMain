#include <Wire.h>
//port 9
#define S1 A9//Led
#define S2 A14//sensor switch
unsigned int redval = 0;
unsigned int greenval = 0;
unsigned int blueval = 0;
unsigned int clearval = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(S1,OUTPUT);
  pinMode(S2, OUTPUT);
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  Wire.begin();
  Serial.begin(115200);
  delay(100);
  Wire.beginTransmission(0x38);
  Wire.write(0x40);
  Wire.write(0xC0);
  if(Wire.endTransmission())
  {
    Serial.println("Error Setup");
  }
  Wire.beginTransmission(0x38);
  Wire.write(0x42);
  Wire.write(0x10);
  if(Wire.endTransmission())
  {
    Serial.println("Error Mode control 2");
  }
  Wire.beginTransmission(0x38);
  Wire.write(0x44);
  Wire.write(0x02);
  if(Wire.endTransmission())
  {
    Serial.println("Error Mode control 3");
  }
  Serial.println("Ready!");
  delay(200);
}
byte colordata[8] = { 0 };
void loop() {
  // put your main code here, to run repeatedly:
  //red____________
  getcolor();
  //redval = (colordata[1] <<8)|colordata[0];
  /*Serial.print("red:");*/ Serial.println(redval);
  /*Serial.print("green");*/ Serial.println(greenval);
  /*Serial.print("blue:");*/ Serial.println(blueval);
  /*Serial.print("clear");*/ Serial.println(clearval);
  Serial.println("___________");
  delay(100);
  
}
void getcolor()
{
  Wire.beginTransmission(0x38);
  Wire.write(0x50);
  if(Wire.endTransmission())
  {
    Serial.println("Bad Call");
  }
  Wire.requestFrom(0x38, 2);
  if(2 == Wire.available())
  {
    redval = Wire.read();
    redval |= (Wire.read() << 8);
    
  }
  //green_____________________
  Wire.beginTransmission(0x38);
  Wire.write(0x52);
  if(Wire.endTransmission())
  {
    Serial.println("Bad Call");
  }
  Wire.requestFrom(0x38, 2);
  if(2 == Wire.available())
  {
    greenval = Wire.read();
    greenval |= (Wire.read() << 8);
    
  }
  //blue__________________________
  Wire.beginTransmission(0x38);
  Wire.write(0x54);
  if(Wire.endTransmission())
  {
    Serial.println("Bad Call");
  }
  Wire.requestFrom(0x38, 2);
  if(2 == Wire.available())
  {
    blueval = Wire.read();
    blueval |= (Wire.read() << 8);
    
  }
  //clear_____________________________
  Wire.beginTransmission(0x38);
  Wire.write(0x56);
  if(Wire.endTransmission())
  {
    Serial.println("Bad Call");
  }
  Wire.requestFrom(0x38, 2);
  if(2 == Wire.available())
  {
    clearval = Wire.read();
    clearval |= (Wire.read() << 8);
    
  }
}
