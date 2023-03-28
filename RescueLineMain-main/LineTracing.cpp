//code for line tracing.

#include "Header.h"
#define CALIBRATION_ITERATIONS 100
#define BLACK_THRESH 1500//greater than means it's black
#define WHITE_THRESH 800 //less than means it's white
const int sensorCount = 8;//# of qtr array sensors
int bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0; float derivative = 0.0; float last_error = 0.0;
enum qtr_check {different, white, black};
int count = 0;
#ifdef main_bot
const float kp = 0.035;
const float ki = 0.0000002;
const float kd = 0.000;

#else
const float kp = 0.025;//error multiplier
const float ki = 0.000001;//integral multiplier
const float kd = 0.04; //kd multiplier
#endif

QTRSensors qtr;


void setup_qtr() {
  //setup for the qtr sensor
  const uint8_t qtr_pins[] = {22, 23, 24, 25, 26, 30, 28, 29};//qtr sensor ports (for all 8 eyes)

  qtr.setTypeRC();
  //qtr.setEmitterPin(2);
  qtr.resetCalibration();

  for (int i = 0; i < CALIBRATION_ITERATIONS; i++) {
    qtr.calibrate();
    qtr.setSensorPins(qtr_pins, sensorCount);
  }
}
float qtr_average(int start, int finish) { //average of all sensors between start and finish. Endpoints included.
  int sum = 0;
  qtr.read(bw_vals);
  for (int i = start; i < finish + 1; i++) {
    sum += bw_vals[i];
  }
  return (sum / (finish - start + 1));
}

float error_calc() {

  //calculate the error for PID line tracing

  //desired difference between sensor pairs (ideally 0, but sensors are not perfect):
#ifdef main_bot
  float target_vals[] = {108, 0, 324, 296}; //3/19-storming. 8/*FINAL ROBOT*/
  float multipliers[] = {1.9, 1.6, 1.2, 1.0};
#else
  float target_vals[] = {380, 168, 223, 168}; //3/4-home.
  float multipliers[] = {6.5, 4.33, 2.67, 1.00};//multipliers for each sensor pair (outer to inner). Outer pairs will have higher multipliers since varying values in them would indicate more extreme cases.
#endif

  float error = 0.0;//error in PID
  qtr.read(bw_vals);

  for (int i = 0; i < sensorCount / 2; i++) { //iterates through all sensor pairs, outer to inner
    float current_val = (bw_vals[7 - i] - bw_vals[i]);//difference between current pair of sensors
    float current_error = target_vals[i] - current_val;//difference between desired val and actual val
    error += (current_error) * multipliers[i]; //sum up errors of each sensor pair
  }

  return error;
}
void qtr_print() {
  //print the qtr values in sensor order

  qtr.read(bw_vals);
  Serial.print("\n");
  for (int i = 0; i < sensorCount; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[i]);

  }
}
void pid_print() {

  //print the error in PID
  Serial.print("PID error: "); Serial.println(error_calc());

}
void diff_print() {//print the diff between sensor pairs.
  qtr.read(bw_vals);
  for (int i = 0; i < sensorCount / 2; i++) {
    Serial.print("Pair ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[7 - i] - bw_vals[i]);
  }
}

int check_all() {//check all sensors to see if they have the same relative values


  qtr.read(bw_vals);
  int result = different;
  bool case_flag = false;

  for (int i = 0; i < 8; i++) {
    if (bw_vals[i] < WHITE_THRESH && result == black) { //if see white when previously see black
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > BLACK_THRESH && result == white) { //if see black when previously white
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < WHITE_THRESH && !case_flag) {
      result = white;//all white
    }

    else if (bw_vals[i] > BLACK_THRESH && !case_flag) {
      result = black;//all black
    }

  }
  return result;
}
int check_left() {//check all sensors to see if they have the same relative values


  qtr.read(bw_vals);
  int result = different;
  bool case_flag = false;

  for (int i = 0; i < 4; i++) {
    if (bw_vals[i] < WHITE_THRESH && result == black) { //if see white when previously see black
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > BLACK_THRESH && result == white) { //if see black when previously white
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < WHITE_THRESH && !case_flag) {
      result = white;//all white
    }

    else if (bw_vals[i] > BLACK_THRESH && !case_flag) {
      result = black;//all black
    }

  }
  return result;
}
int check_right() {//check all sensors to see if they have the same relative values


  qtr.read(bw_vals);
  int result = different;
  bool case_flag = false;

  for (int i = 4; i < 8; i++) {
    if (bw_vals[i] < WHITE_THRESH && result == black) { //if see white when previously see black
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > BLACK_THRESH && result == white) { //if see black when previously white
      result = different;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < WHITE_THRESH && !case_flag) {
      result = white;//all white
    }

    else if (bw_vals[i] > BLACK_THRESH && !case_flag) {
      result = black;//all black
    }

  }
  return result;
}
void tCase()//case for t-intersection (|-)
{
  const int turnDelay = 225;
  bool seeBlack = false;
  qtr.read(bw_vals);
  if ((bw_vals[7] > BLACK_THRESH && bw_vals[6] > BLACK_THRESH && bw_vals[5] > BLACK_THRESH && bw_vals[4] > BLACK_THRESH) || (bw_vals[0] > BLACK_THRESH && bw_vals[1] > BLACK_THRESH && bw_vals[2] > BLACK_THRESH && bw_vals[3] > BLACK_THRESH)) // if sees black on either edge
  {
    //^^checks for tCase
    float previousTime = millis();

    if (bw_vals[7] > BLACK_THRESH) {//if t_case on left side
      while (millis() < previousTime + turnDelay) {//turn for turndelay ms
        lturn(70);//turning towards t-case line
        qtr.read(bw_vals);
        if (bw_vals[0] > BLACK_THRESH) {
          seeBlack = true;//if sees black on opposite sensor during any point of the turn
        }
      }
      if (seeBlack) { //if saw black (meaning it is a false line)
        rturn(70);//turn back
        delay(turnDelay);
      }
      motorsStop();
    }
    else {//if t_case on right side
      while (millis() < previousTime + turnDelay) {//turn of turnDelay ms
        rturn(70);//turn towards t-case line
        qtr.read(bw_vals);
        if (bw_vals[7] > BLACK_THRESH) {
          seeBlack = true;//if see black
        }
      }
      if (seeBlack) {
        lturn(70);//turn back
        delay(turnDelay);
      }
    }
    if (seeBlack) {
      go_motors(70);//go forward if it is a falseline
      delay(turnDelay);
    }

  }
}
void lineTrace() {//main line tracking function

  int base_speed = 50 + getPitch();//base speed for Line Tracing
  tCase();
  float error = error_calc();//calculating error
  integral += error;//summing up all erors during runtime
  derivative = error - last_error;//checking the change in the errors over time.
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);//final number that calculates how much to adjust the motors.

#ifdef main_bot
  setMultipleMotors(base_speed + adjustSpeed, base_speed - adjustSpeed);
#else
  setMultipleMotors(base_speed - adjustSpeed, base_speed + adjustSpeed);
#endif

  last_error = error;
  digitalWrite(A10, LOW);
  digitalWrite(A9, HIGH);
  digitalWrite(A11, LOW);


}
