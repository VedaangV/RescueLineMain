//code for obstacle avoidance.
#include "Header.h"
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;

void distanceISR()
{

  if (pulseFirst)
  {
    pulseMsSt = micros();
    pulseFirst = 0;
  }
  else
  {
    pulseMsEd = micros();
    pulseFirst = 1;
  }
}

float getFrontDistance()//get distance of Front US
{
  digitalWrite(5, LOW);
  delay(1);
  digitalWrite(5, HIGH);
  delay(1);
  digitalWrite(5, LOW);
  long pulseTime = pulseIn(2, HIGH);
  float distance;
  distance = pulseTime * 0.034 / 2;
  if (distance < 0)
  {
    distance = 0;
  }
  return (distance);
}

bool seeObs(long dist) { //sees obstacle?
  if (getFrontDistance() <= dist) {
    return true;
  }
  else {
    return false;
  }
}

void avoid(int sign) { //move around obstacle
  forwardCm(15.0, 70);
  enc_turn(-90 * sign, 70);
  forwardCm(40.0, 70);
  enc_turn(-90 * sign, 70);
  forwardCm(15.0, 70);
  enc_turn(90 * sign, 70);
}
void obstacle() { //main obstacle function
  enc_turn(90, 70);
  if (seeObs(10.0)) { //sees wall
    enc_turn(180, 70);
    avoid(-1);
  }
  else {
    avoid(1);
  }
}
