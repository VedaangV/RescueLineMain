//code for obstacle avoidance.
#include "Header.h"
long duration, inches, cm;
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;
const int pingPin = 39;

long microToInch(long microseconds) {
  return microseconds / 29 / 2;
}

long microToCm(long microseconds) {
  return microseconds / 73.746 / 2;
}

long getCm() { //get Cm from ping sensor
  pinMode(pingPin, OUTPUT);//set as output
  digitalWrite(pingPin, LOW);//start with low pulse for clean high
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);//set as input
  duration = pulseIn(pingPin, HIGH);//read input in microseconds
  return microToCm(duration);//convert micro s to cm
}
long getInch() { //get inches from ping sensor
  pinMode(pingPin, OUTPUT);//set as output
  digitalWrite(pingPin, LOW);//start with low pulse for clean high
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);//set as input
  duration = pulseIn(pingPin, HIGH);//read input in microseconds
  return microToInch(duration);//convert micro s to inch
}
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
  long pulseTime = pulseMsEd - pulseMsSt;
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
  forwardCm(7.0, 70);
  enc_turn(-90 * sign, 70);
  forwardCm(17.0, 70);
  enc_turn(-90 * sign, 70);
  forwardCm(7.0, 70);
  enc_turn(90 * sign, 70);
}
void obstacle() { //main obstacle function
  if (seeObs(5.0)) {
    enc_turn(90, 70);
    if (seeObs(10.0)) { //sees wall
      enc_turn(180, 70);
      avoid(-1);
    }
    else {
      avoid(1);
    }
  }
}

