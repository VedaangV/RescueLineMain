#include "Megapi_Functions.h"
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
static float yaw = 0.0;

const int m1_forward = -75;
const int m2_forward = 75;
//****************************************************************************************************************\\


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  if (!bno.begin(8))
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

}
//****************************************************************************************************************\\

float getYaw()
{
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;
}

void enc_turn(int deg, int speed)
{
  int target = 0;
  motorsStop();
  yaw = getYaw();
  target = yaw + deg;

  if (target < 0)
  {
    target+= 360;
  }

  else if (target > 360)
  {
    target -= 360;
  }
  Serial.print("Degree: ");
  Serial.println(deg);
  
  Serial.print("target: ");
  Serial.println(target);
  int calc_speed = ((deg * -1) * speed) / abs(deg);
  Serial.print("\n\n\n\n\n\n\n\n\n\n\nRIGHT MOTOR: ");
  Serial.println(calc_speed);
  Serial.print("LEFT MOTOR: ");
  Serial.println(-calc_speed);

  while (yaw > target + 2 || yaw < target - 2)
    {
      Serial.println(yaw);
      rightMotorRun(calc_speed);
      leftMotorRun(-calc_speed);
      yaw = getYaw();
    }
    
  motorsStop();
}

void loop()
{
  enc_turn(45, 150);
  delay(500);
}
