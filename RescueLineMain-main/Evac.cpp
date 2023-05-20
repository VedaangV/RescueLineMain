//Code for the evac room
#include "Header.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo arm;  // create servo object to control a servo
SoftwareSerial camSerial(51, 52); // RX, TX

float current = 0;

void evacSetup()
{
  //setup tof sensors
  Wire.begin();
  //channel 0 = wall sensor
  tcaselect(7);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X #1"));
    while(1);
  }
  //Channel1 = grab sensor
  tcaselect(6);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X #2"));
    while(1);
  }

  //armsetup
  pinMode(A7,INPUT_PULLUP);//grabber sensor
  arm.attach(SERVOPIN);  // attaches the servo on pin 9 to the servo object
  arm.write(95);//bring it up
  delay(800);

  //setup coms___________________________________________________
  
  arm.detach();
  Serial.println("Contact cam");
  camSerial.begin(9600);
  camSerial.print("S");
  while(!camSerial.available())
  {
    delay(10);
  }
  
  int camMessage = camSerial.read();
  Serial.println("Got Response");
  
  delay(500);

  //clamp on block
  grabMotorRun(128);
  while(analogRead(A8)<540 && digitalRead(A7) == HIGH)
  {
    delay(1);
  }
  grabMotorRun(0);
  
}

void dumpKit()
{
  forwardCm(60, 100);
  //wake the cam back u
  pinMode(A7,INPUT_PULLUP);//grabber sensor
  arm.attach(SERVOPIN);  // attaches the servo on pin 9 to the servo object
  arm.write(12);
  delay(900);
  arm.detach();
  camSerial.print("S");
  while(!camSerial.available())
  {
    delay(10);
  }
  int camMessage = camSerial.read();
  Serial.println("Got Response");

  delay(500);
  //rotate to ball
  while(camMessage != 'C')
  {
    while(!camSerial.available())
    {
      delay(5);
    }
    camMessage = camSerial.read();
    Serial.println(camMessage);
    switch(camMessage)
    {
      case 'L':
        rturn(75);
        break;
     case 'R':
        lturn(75);
        break;
     default:
        break;
    }
  }
  motorsStop();
  arm.attach(SERVOPIN);  // attaches the servo on pin 9 to the servo object
  arm.write(100);//slightly higher
  delay(500);
  //drive until wall

  tcaselect(7);
  go_motors(-60);
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  while(measure.RangeMilliMeter > 50)
  {
    delay(10);
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    Serial.println(measure.RangeMilliMeter);
  }
  motorsStop();
  delay(500);
  arm.write(70);//slightly down
  grabMotorRun(-128);
  delay(1000);
  grabMotorRun(0);
  
  delay(5000);
}
