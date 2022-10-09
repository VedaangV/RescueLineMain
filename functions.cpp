//below are essential functions, used all across our project. 
#include "Header.h"
#include "MeMegaPi.h"
MeMegaPiDCMotor lmotor(PORT1B);//left motor
MeMegaPiDCMotor rmotor(PORT2B);//right motor
volatile int enc = 0;

float cm_to_encoders(float cm) {
  const float wheelDiameter = 6.0;
  return cm / (wheelDiameter * PI) * 360;
}

void Interruptfunc() {
  if (digitalRead(31) == HIGH) {
    enc--;
  }
  else {
    enc++;
  }
}

void forward_enc(int encoders, int motorSpeed) {//go forward for enc
  enc = 0;
  while (enc <= encoders) {
    lmotor.run(50);
    rmotor.run(-50);
  }
  setMultipleMotors(0, 0);
}

void forwardCm(float dist) {//go forward for cm
  enc = 0;
  while (enc <= cm_to_encoders(dist)) {
    setMultipleMotors(50, 50);
  }
  setMultipleMotors(0, 0);
}

void setMultipleMotors(int left, int right){
  lmotor.run(left);
  rmotor.run(right);
}
void go_motors(int motorSpeed){
  lmotor.run(motorSpeed);
  rmotor.run(motorSpeed);
}
void rturn(int motorSpeed){
  lmotor.run(motorSpeed);
  rmotor.run(-1 * motorSpeed);
}
void lturn(int motorSpeed){
  lmotor.run(-1 * motorSpeed);
  rmotor.run(motorSpeed);
}
