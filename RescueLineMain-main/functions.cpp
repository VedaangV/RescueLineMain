//below are essential functions, used all across our project.
#include "Header.h"
int redPin = A11;
int greenPin = A10;
int bluePin = A9;
volatile int enc = 0;
void motorSetup()
{
  TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);//970hz
  TCCR4A = _BV(WGM40);
    TCCR4B = _BV(CS41) | _BV(CS40) | _BV(WGM42);//970hz
  TCCR2A = _BV(WGM21) | _BV(WGM20);//970hz
    TCCR2B = _BV(CS22);
    
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
}

float cm_to_encoders(float cm) {
  const float wheelDiameter = 6.42;
  return cm / (wheelDiameter * PI) * 360;
}

void Interruptfunc() {//interrupt func for encoders
  if (digitalRead(31) == HIGH) {
    enc++;
  }
  else {
    enc--;
  }
}

void forward_enc(int encoders, int motor_speed) {//go forward for enc
  int previousEnc = enc;
  while (enc <=  previousEnc + encoders) {
    //Serial.println(enc);
    setMultipleMotors(motor_speed, motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backward_enc(int encoders, int motor_speed) {//go backward for enc
  int previousEnc = enc;

  while (enc >=  previousEnc - encoders) {
    setMultipleMotors(-motor_speed, -motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backwardCm(float dist, int motor_speed) { //go backward for cm
  backward_enc(cm_to_encoders(dist), motor_speed);
}
void forwardCm(float dist, int motor_speed) {//go forward for cm
  forward_enc(cm_to_encoders(dist), motor_speed);
}


void setMultipleMotors(int left, int right) { //set motors
  int boost = 0;//in case motors are slow
  //port 1
  left = left > 255 ? 255 : left;
  left = left < -255 ? -255 : left;
  
  if (left > 0)
  {
    #ifdef main_bot
    left += boost;
    #endif
    digitalWrite(35, LOW);
    delayMicroseconds(5);
    digitalWrite(34, HIGH);
    analogWrite(12, left); //pwm pin 12
  }
  else if (left < 0)
  {
    #ifdef main_bot
    left -= boost;
    #endif
    digitalWrite(34, LOW);
    delayMicroseconds(5);
    digitalWrite(35, HIGH);
    analogWrite(12, -left);
  }
  else
  {
    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    analogWrite(12, 0);
  }

  right = left > 255 ? 255 : right;
  right = right < -255 ? -255 : right;
  right = -right;
  if (right > 0)
  {
    #ifdef main_bot
    right += boost;
    #endif
    digitalWrite(36, LOW);
    delayMicroseconds(5);
    digitalWrite(37, HIGH);
    analogWrite(8, right);
  }
  else if (right < 0)
  {
    #ifdef main_bot
    right -= boost;
    #endif
    digitalWrite(37, LOW);
    delayMicroseconds(5);
    digitalWrite(36, HIGH);
    analogWrite(8, -right);
  }
  else
  {
    digitalWrite(36, LOW);
    digitalWrite(37, LOW);
    analogWrite(8, 0);
  }

}
//port 3
void grabMotorRun(int16_t speed)
{
  speed = speed > 255 ? 255 : speed;
  speed = speed < -255 ? -255 : speed;
  //speed = -speed;
  if(speed > 0)
  {
    digitalWrite(42, LOW);
    delayMicroseconds(5);
    digitalWrite(43, HIGH);
    analogWrite(9,speed);
  }
  else if(speed < 0)
  {
    digitalWrite(43, LOW);
    delayMicroseconds(5);
    digitalWrite(42, HIGH);
    analogWrite(9,-speed);
  }
  else
  {
    digitalWrite(42, LOW);
    digitalWrite(43, LOW);
    analogWrite(9,0);
  }
}
void go_motors(int motorSpeed) { //goes forward
  setMultipleMotors(motorSpeed, motorSpeed);
}
void rturn(int motorSpeed) { //right turn
  setMultipleMotors(motorSpeed, -motorSpeed);
}
void lturn(int motorSpeed) { //left turn
  setMultipleMotors(-motorSpeed, motorSpeed);
}
void tcaselect(uint8_t i) {//selects tca channel
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void rightMotorRun(int motorSpeed) { //move right motor
  setMultipleMotors(0, motorSpeed);
}
void leftMotorRun(int motorSpeed) { //move left motor
  setMultipleMotors(motorSpeed, 0);
}
void motorsStop() { //stop all motors
  setMultipleMotors(0, 0);
}
void set_LED(led_colors color) { //set led to red, green, or blue
#ifdef main_bot
  digitalWrite(redPin, HIGH & (color == red));//set red high if color is red
  digitalWrite(greenPin, HIGH & (color == green));//set green high if color is green
  digitalWrite(bluePin, HIGH & (color == blue));//set blue high if color is blue
#endif
}
