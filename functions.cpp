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

void forward_enc(int encoders, int motor_speed) {//go forward for enc
  enc = 0;
  while (enc <= encoders) {
    lmotor.run(motor_speed);
    rmotor.run(-1 * motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backward_enc(int encoders, int motor_speed) {//go backward for enc
  enc = 0;
  while (enc >= -1 * encoders) {
    lmotor.run(-1 * motor_speed);
    rmotor.run(motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backwardCm(float dist, int motor_speed) { //go backward for cm
  backward_enc(cm_to_encoders(dist), motor_speed);
}
void forwardCm(float dist, int motor_speed) {//go forward for cm
  forward_enc(cm_to_encoders(dist), motor_speed);
}

void setMultipleMotors(int left, int right) { //sets motors
  lmotor.run(left);
  rmotor.run(-1 * right);
}
void go_motors(int motorSpeed) { //goes forward
  lmotor.run(motorSpeed);
  rmotor.run(-1 * motorSpeed);
}
void basic_turn(int motorSpeed) { //basic turn
  lmotor.run(motorSpeed);
  rmotor.run(motorSpeed);
}
void rturn(int motorSpeed) { //right turn
  lmotor.run(motorSpeed);
  rmotor.run(motorSpeed);
}
void lturn(int motorSpeed) { //left turn
  lmotor.run(-1 * motorSpeed);
  rmotor.run(-1 * motorSpeed);
}
void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void rightMotorRun(int motorSpeed) { //move right motor
  rmotor.run(-1 * motorSpeed);
}
void leftMotorRun(int motorSpeed) { //move left motor
  lmotor.run(motorSpeed);
}
void motorsStop() { //stop all motors
  lmotor.run(0);
  rmotor.run(0);
}
int getCase() { //checks for all important cases during a run. Based on the return val, the main function is determined.
  /*if(<silver condition>){EVAC
    return 1;
    }*/

  if (get_color(6.5) != 0) {
    return 2;
  }

  if (seeObs(7.0)) { //OBSTACLE
    return 3;
  }

  return 0;

}
