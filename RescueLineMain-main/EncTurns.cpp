#include "Header.h"
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
float yaw = 0;
const int m1_forward = -75;
const int m2_forward = 75;
float starting_yaw;
sensors_event_t orientationData;
imu::Vector<3> rot;


float getYaw()//get rotation of BNO
{
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  rot = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;

}
float getPitch()//get tilt (pitch) of BNO
{
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.y;
}

void enc_turn(int deg, int speed)//turns a specific amount of degrees
{

  int target = 0;
  motorsStop();
  yaw = getYaw();
  target = yaw + deg;

  if (target < 0)
  {
    target += 360;
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

  while ((yaw > target + 2) || (yaw < target - 2))
  {
    //Serial.println(yaw);
    setMultipleMotors(-calc_speed, calc_speed);
    yaw = getYaw();
  }

  motorsStop();
}
void enc_turn_abs(int deg, int speed)//turns a certain amount of degrees, rounding starting rotation to the nearest 90
{

  int target = 0;
  motorsStop();
  int abs_yaw = getYaw();
  abs_yaw += ((abs_yaw % 90 >= 45 /* should round up?*/) & (90 - (abs_yaw % 90))/*round up*/) + ((abs_yaw % 90 < 45 /*should round down?*/) & (-1 * (abs_yaw % 90))/*round down*/); //rounds yaw to nearest 90
  target = abs_yaw + deg;

  if (target < 0)
  {
    target += 360;
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

  while ((yaw > target + 2) || (yaw < target - 2))
  {
    //Serial.println(yaw);
    setMultipleMotors(-calc_speed, calc_speed);
    yaw = getYaw();
  }

  motorsStop();
}
bool enc_turn(int deg, int speed, int tCase)//turns a specific amount of degrees
{
  bool seeBlack = 0;
  int target = 0;
  motorsStop();
  yaw = getYaw();
  target = yaw + deg;

  if (target < 0)
  {
    target += 360;
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
  int calc_speed_right = ((deg * -1) * speed) / abs(deg) + 10;
  Serial.print("\n\n\n\n\n\n\n\n\n\n\nRIGHT MOTOR: ");
  Serial.println(calc_speed);
  Serial.print("LEFT MOTOR: ");
  Serial.println(-calc_speed);

  while ((yaw > target + 2) || (yaw < target - 2))
  {
    //Serial.println(yaw);
    setMultipleMotors(-calc_speed, calc_speed_right);
    qtr.read(bw_vals);
    if (bw_vals[tCase] > BLACK_THRESH) {
      seeBlack = true;
    }
    yaw = getYaw();
  }

  motorsStop();
  return seeBlack;
}

void bnoSetup()
{
  if (!bno.begin(8))
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  
}
