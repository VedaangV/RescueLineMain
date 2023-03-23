//below are essential functions, used all across our project.
#include "Header.h"
volatile int enc = 0;

float cm_to_encoders(float cm) {
  const float wheelDiameter = 6.42;
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
  int previousEnc = enc;
  while (enc <= previousEnc + encoders) {
    setMultipleMotors(motor_speed,motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backward_enc(int encoders, int motor_speed) {//go backward for enc
  int previousEnc = enc;
  while (enc >= -1 * (previousEnc + encoders)) {
    setMultipleMotors(-motor_speed,-motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backwardCm(float dist, int motor_speed) { //go backward for cm
  backward_enc(dist * 56 , motor_speed);
}
void forwardCm(float dist, int motor_speed) {//go forward for cm
  forward_enc(dist * 56, motor_speed);
}



void setMultipleMotors(int left, int right){
  //port 1
  left = left > 255 ? 255 : left;
  left = left < -255 ? -255 : left;
 
  if(left > 0)
  {
    digitalWrite(35, LOW);
    delayMicroseconds(5);
    digitalWrite(34, HIGH);
    analogWrite(12,left);//pwm pin 12
  }
  else if(left < 0)
  {
    digitalWrite(34, LOW);
    delayMicroseconds(5);
    digitalWrite(35, HIGH);
    analogWrite(12,-left);
  }
  else
  {
    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    analogWrite(12,0);
  }

  right = left > 255 ? 255 : right;
  right = right < -255 ? -255 : right;
  right = -right;
  if(right > 0)
  {
    digitalWrite(36, LOW);
    delayMicroseconds(5);
    digitalWrite(37, HIGH);
    analogWrite(8,right);
  }
  else if(right < 0)
  {
    digitalWrite(37, LOW);
    delayMicroseconds(5);
    digitalWrite(36, HIGH);
    analogWrite(8,-right);
  }
  else
  {
    digitalWrite(36, LOW);
    digitalWrite(37, LOW);
    analogWrite(8,0);
  }

}
void go_motors(int motorSpeed) { //goes forward
  setMultipleMotors(motorSpeed,motorSpeed);
}
void rturn(int motorSpeed) { //right turn
  setMultipleMotors(motorSpeed,-motorSpeed);
}
void lturn(int motorSpeed) { //left turn
  setMultipleMotors(-motorSpeed,motorSpeed);
}
void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void rightMotorRun(int motorSpeed) { //move right motor
  setMultipleMotors(0,motorSpeed);
}
void leftMotorRun(int motorSpeed) { //move left motor
  setMultipleMotors(motorSpeed,0);
}
void motorsStop() { //stop all motors
  setMultipleMotors(0,0);
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
