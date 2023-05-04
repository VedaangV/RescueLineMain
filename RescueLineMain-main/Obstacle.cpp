//code for obstacle avoidance.
#include "Header.h"
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;
int trig = 4;
int echo = 5;
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

  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  long pulseTime = pulseIn(echo, HIGH);
  float distance;
  distance = pulseTime * 0.034 / 2;
  if (distance < 0)
  {
    distance = 0;
  }
  return (distance);
}

bool seeObs(long dist) { //sees obstacle?
  Serial.println(getFrontDistance());
  if (getFrontDistance() <= dist) {
    return true;
  }
  else {
    return false;
  }
}

void avoid(int sign) { //move around obstacle
  bool flag = false;
  forwardCm(15.0, 100);
  enc_turn(-90 * sign, 100);
  forwardCm(40.0, 70);
  enc_turn(-90 * sign, 100);
  int enc1 = enc;
  while (leftBlack() == 0 && rightBlack() == 0) { //forward until black line
    if (enc - enc1 >= cm_to_encoders(24.0) && !flag) { //if traveled more than or 22cm (lost the line)
      enc_turn(-90 * sign, 100);//turn again to find the line
      flag = true;//ensures that it only does this pnce
    }
    go_motors(70);
    qtr.read(bw_vals);
  }
  forwardCm(7.0, 70);
  enc_turn(90 * sign, 100);
}
void obstacle() { //main obstacle function
  if (seeObs(8.0)) {
    enc_turn(90, 100);
    if (seeObs(10.0)) { //sees wall
      enc_turn(180, 100);
      avoid(-1);
    }
    else {
      avoid(1);
    }
  }
}
