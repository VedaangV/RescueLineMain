#include "Header.h"
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
int BNO055_SAMPLERATE_DELAY_MS = (10);
imu::Vector<3> rot;
int BNO = 2;
float yaw = 0.0;

const int m1_forward = -75;
const int m2_forward = 75;
//****************************************************************************************************************\\


void enc_turn(int deg, int speed)//function for turning a specific amount of degrees.
{
  tcaselect(BNO);
  int target = deg;
  motorsStop();
  yaw = getYaw();

  //right turn
  if (deg > 0)
  {
    //target = yaw + deg;
    if (target > 360)
    {
      target = target - 360;
    }

    while (/*yaw > target + 1 ||*/ yaw < target - 1)
    {
      rightMotorRun(-speed);
      leftMotorRun(speed);
      yaw = getYaw();
      Serial.print("Yaw: ");
      Serial.println(yaw);
    }
    motorsStop();
  }

  //left turn
  if (deg < 0)
  {
    if (yaw < abs(deg))
    {
      target = yaw + (360 - abs(deg));
    }

    else
    {
      target = yaw - abs(deg);
    }
    while (yaw > target + 2 /*|| yaw < target - 2*/)
    {
      rightMotorRun(speed);
      leftMotorRun(-speed);
      yaw = getYaw();
    }

    while (yaw > target)
    {
      lturn(speed);
      yaw = getYaw();
    }
    motorsStop();
  }

}
float getYaw() { //gets yaw from BNO
  tcaselect(BNO);
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;
}
void bnoSetup() { //bno init
  tcaselect(BNO);
  if (!bno.begin(8))
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1) {};
  }
}
