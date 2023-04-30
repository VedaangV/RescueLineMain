#include <QTRSensors.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include <U8x8lib.h>
#include <Wire.h>
#include "C:\Users\ae900\Documents\Arduino\Rangerbot\Ranger_Functions\Ranger_Functions.ino"
#define PWMA 11 //ranger pin D11
#define DIRA1 48// ranger pinD48
#define DIRA2 49//ranger pin D49
#define PWMB 10//ranger pin D10
#define DIRB1 47//ranger pin D47
#define DIRB2 46//ranger pin D46
#define ENAA 19//ranger pin D19 interrupt pin
#define ENBA 18//ranger pin D18
#define ENAB 42//ranger pin D42 interrupt pin
#define ENBB 43//ranger pin D43
#define PINGP A15
#define LTRIG A6
#define LECHO A11
#define RTRIG A7
#define RECHO A12

float wheelbase = 7.3;
float radius = 4;
float innerSpeed = 80;
float outerSpeed = 80;

//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);     
float yaw = 0;
float startyaw = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

QTRSensors qtr;


const int m1_forward = -75;
const int m2_forward = 75;
double m1_speed = 0;
double m2_speed = 0;
const double Kp = 0.09;//0.09
const double baseSpeed = 100;

const int SensorCount = 8;
uint16_t sensorValues[SensorCount];


int black = 1100;
int white = 300;
int th = (black + white) / 2;
//int th = 500;

int pin = 2;
float distance;
volatile unsigned long distStart = 0;
volatile unsigned long distEnd = 0;
volatile unsigned long distStartTmp = 0;// temporary distance
volatile bool first = 1;
unsigned long serialLoop = 0;
unsigned long currentMillis = millis();
long frontDist = 100;
/* 
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;*/
float getYaw()
{
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;
}
void imuLeft(int speed,int angle)
{
   yaw = getYaw();
  int target = yaw + angle;
  if (target > 360)
  {
    target = (target + 90) - 360;
  }

  // 90 degree turn to the right
  while (yaw < target)
  {
    Serial3.print("deg90right yaw:");
    Serial3.println(yaw);
    left(speed);
    yaw = getYaw();
  }
  brake();
}
void imuRight(int speed,int angle)
{
  //Serial33.println("turn 90 left");
  yaw = getYaw(); 
  int target = 0;
  
  if (yaw < 90)
  {
    target = yaw + (360-angle);
  }

  else 
  {
    target = yaw - angle;
  }

  //Serial33.print("Target: ");
  //Serial3.println(target);

  while (yaw < target)
  {

    Serial3.print("deg90left yaw:");
    Serial3.println(yaw);
    right(speed);

    yaw = getYaw();

  }

  while (yaw > target)
  {

    Serial3.print("deg90left yaw:");
    Serial3.println(yaw);
    right(speed);

    yaw = getYaw();

  }
  brake();
}

void countDist()
{
  if(first)
  {
    distStartTmp = micros(); 
  }
  else
  {
    distEnd = micros();
    distStart = distStartTmp;
  }
  first = !first;
}
//returns distance in cm
float getFrontDistance()
{
  long duration;
  pinMode(PINGP, OUTPUT);
  digitalWrite(PINGP, LOW);//send a solid low signal
  delayMicroseconds(2);
  digitalWrite(PINGP, HIGH);
  delayMicroseconds(6);//high pulde for at least 2 microseconds to start the sensor
  digitalWrite(PINGP, LOW);
  //detecting
  pinMode(PINGP, INPUT);
  duration = pulseIn(PINGP, HIGH, 7000);//7e000
  duration = duration / 29 / 2;
  return(duration);
}
float getLeftDistance()
{
  long duration;
  float distance;
  // Clears the trigPin
  digitalWrite(LTRIG, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(LTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(LTRIG, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(LECHO, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return (distance);
}
float getRightDistance()
{
  long duration;
  float distance;
  // Clears the trigPin
  digitalWrite(RTRIG, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(RTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(RTRIG, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(RECHO, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return (distance);
}


void setup()
{
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){32, 34, 36, 40, 33, 35, 37, 41}, SensorCount);
  pinMode(PWMA, OUTPUT);//
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA1, OUTPUT);
  pinMode(DIRA2, OUTPUT);
  pinMode(DIRB1, OUTPUT);
  pinMode(DIRB2, OUTPUT);
  pinMode(ENAA, INPUT_PULLUP);
  pinMode(ENAB, INPUT_PULLUP);
  pinMode(ENBA, INPUT_PULLUP);
  pinMode(ENBB, INPUT_PULLUP);
  Wire.begin();
  if (!bno.begin(8))
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial3.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  //qtr.setEmitterPin(2);
  //u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  //u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  //u8x8log.setRedrawMode(0);    // 0: Update screen with newline, 1: Update screen for every char  
  Serial3.begin(9600);
  pinMode(pin, INPUT);
  pinMode(LTRIG, OUTPUT);
  pinMode(RTRIG, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(pin), countDist, CHANGE);

}

void rturn(int m2)
{
  //Serial.println("right turn");
  dragright(m2);
  delay(100);
}

void lturn(int m1)
{
  //Serial.println("left turn");
  dragleft(m1);
  delay(100);
}
void centerOnLine()
{
  int left_val;
  int right_val;
  int centerError = 1000;
  while(abs(centerError) > 300)
  {
    qtr.read(sensorValues);
    
    left_val = (sensorValues[0] * 1.3) + (sensorValues[1] * 1.2) + (sensorValues[2] * 1.1) + sensorValues[3];
    right_val = (sensorValues[7] * 1.3) + (sensorValues [6] * 1.2) + (sensorValues[5] * 1.1) + sensorValues[4];
    centerError = left_val - right_val;
    //negative turn right
    if(centerError > 300)
    {
      slowright(100,20);
    }
    else if(centerError < -300)
    {
      slowleft(100,20);
    }
    else
    {
      brake();
    }
  }
  brake();
}

void simplePid(int error)
{
  if (error > 0)
  {
    m1_speed = baseSpeed + abs(error * Kp);
    m2_speed = baseSpeed - abs(error * Kp);
  }

  else if (error < 0)
  {
    m2_speed = baseSpeed + abs(error * Kp);
    m1_speed = baseSpeed - abs(error * Kp);
  }

  else
  {
    m1_speed = baseSpeed;
    m2_speed = baseSpeed;
  }

  m1_speed = constrain(m1_speed, -255, 255);
  m2_speed = constrain(m2_speed, -255, 255);

}


void loop()
{
  //Serial.println("Linetrace");
  //2500 means dark, 0 means light
  //1 means dark, 0 means light
  qtr.read(sensorValues);
  
  int left_val = (sensorValues[0] * 1.3) + (sensorValues[1] * 1.2) + (sensorValues[2] * 1.1) + sensorValues[3];
  int right_val = (sensorValues[7] * 1.3) + (sensorValues [6] * 1.2) + (sensorValues[5] * 1.1) + sensorValues[4];
  int error = left_val - right_val;

  currentMillis = millis();
  if(currentMillis - serialLoop >= 10)
  {
    serialLoop = currentMillis;
    frontDist = getLeftDistance(); 
    //Serial3.print(getLeftDistance());
    //Serial3.print(" : ");
    Serial3.println(frontDist);
    //Serial3.print(" : ");
    //Serial3.println(getRightDistance());
  }
  if (frontDist <6 && frontDist != 0)
  {
    brake(); 
    Serial3.println(frontDist);
    delay(500);
    /*while(getFrontDistance() < 14)
    {
      backward(80);
    }
    brake();
    delay(100);
    
    Serial3.println(getFrontDistance());*/
    centerOnLine();
    tone(45, 1000,500);
    delay(500);
    imuRight(128,90);
    delay(1000);
    outerSpeed = (1+(wheelbase/radius))* innerSpeed;
    //delay(2000);
    qtr.read(sensorValues);
    prop(innerSpeed, outerSpeed);//right, left
    delay(500);
    qtr.read(sensorValues);
    while((sensorValues[0] < th && sensorValues[1] < th && sensorValues[2] < th && sensorValues[3] < th && sensorValues[4] < th && sensorValues[5] < th && sensorValues[6] < th && sensorValues[7] < th))
    {
      qtr.read(sensorValues);
      prop(innerSpeed, outerSpeed);
    }
    brake();
    Serial3.println("black");
    delay(1000);
    right(128);
    delay(250);
    brake();
    delay(1000);
  }
  else if (sensorValues[0] > th && sensorValues[1] > th && sensorValues[2] > th && sensorValues[3] > th && sensorValues[4] > th && sensorValues[5] > th && sensorValues[6] > th && sensorValues[7] > th)
  {
    Serial.println("all black");
    forward(m1_forward);
  }

  else if (sensorValues[0] < th && sensorValues[1] < th && sensorValues[2] < th && sensorValues[3] < th && sensorValues[4] < th && sensorValues[5] < th && sensorValues[6] < th && sensorValues[7] < th)
  {
    Serial.println("gap");
    brake();
  }
  
  else
  {
    //Serial.println("linetrace");
    simplePid(error);
    analogWrite(11, m1_speed);//PWMA
    digitalWrite(48, HIGH);//DIRA1
    digitalWrite(49, LOW);//DIRA2
    analogWrite(10, m2_speed);//PWMB
    digitalWrite(47, HIGH);//DIRB1
    digitalWrite(46, LOW);//DIRB2
  }



}
