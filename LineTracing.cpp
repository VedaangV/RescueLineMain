//code for line tracing.
#include "Header.h"
const int sensorCount = 8;//# of qtr array sensors
int bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0;
float derivative = 0.0;
float last_error = 0.0;
int count = 0;
unsigned long previousCase;
QTRSensors qtr;

void setup_qtr() {
  //setup for the qtr sensor

  const uint8_t qtr_pins[] = {22, 23, 24, 25, 26, 27, 28, 29};

  qtr.setTypeRC();
  qtr.setEmitterPin(30);
  qtr.resetCalibration();

  for (int i = 0; i < 100; i++) {//100 iterations
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
  float target_vals[] = { -96, -144, -240, -876}; //As of 10/30.

  float multipliers[] = {3.23, 2.33, 1.66, 1.0};//multipliers for each sensor pair (outer to inner). Outer pairs will have higher multipliers since varying values in them would indicate more extreme cases.
  float error = 0.0;//error in PID
  qtr.read(bw_vals);

  for (int i = 0; i < sensorCount / 2; i++) { //iterates through all sensor pairs, outer to inner
    float current_val = (bw_vals[7 - i] - bw_vals[i]);//difference between current pair of sensors
    float current_error = target_vals[i] - current_val;
    error += (current_error) * multipliers[i]; //difference between desired difference of sensor vals and actual difference of sensor vals. Each pair has its own multiplier.
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
int check_tCase() { // check "t-case" where there is a misguiding lien branching out to one side.

  qtr.read(bw_vals);
  int total = 0;
  int averageLeft = 0; //0 for white 1 for black
  int averageRight = 0;

  for (int i = 0; i < 4; i++) { //get the total of the left side qtr vals
    total += bw_vals[i];
  }

  if (total / 4 >= 1800) { //average of left side is black
    averageLeft = 1;
  }

  total = 0;

  for (int i = 4; i < 8; i++) { //get the total of the ride side qtr values
    total += bw_vals[i];
  }

  if (total / 4 >= 1800) {
    averageRight = 1;
  }

  if ((averageRight == 1 && averageLeft == 0)) { //if right sees black and left doesnt. Assume its t-case.
    return 2;
  }

  if ((averageRight == 0 && averageLeft == 1)) { //if left sees black and right doesnt. Assume t-case;
    return 1;
  }

  else {
    return 0;//no t-case
  }

}
int check_all() {//check all sensors to see if they have the same relative values

  qtr.read(bw_vals);
  int result = 0;// 0 - no uniform val, 1 - white, 2 - black
  bool case_flag = false;

  for (int i = 0; i < 8; i++) {
    if (bw_vals[i] < 500 && result == 2) { //if see white when previously see black
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > 1000 && result == 1) { //if see black when previously white
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < 500 && !case_flag) {
      result = 1;//all white
    }

    else if (bw_vals[i] > 1000 && !case_flag) {
      result = 2;//all black
    }

  }
  return result;
}
void lineTrace() {//main line tracking function

  if (check_all() == 1 ||  check_all() == 2) { //if see all white (gap) or see all black (cross intersection)
    go_motors(60);//forward speed 60
    return;
  }

  const int base_speed = 50;//base speed for Line Tracing
  const float kp = 0.065;//error multiplier
  const float ki = 0.0;//integral multiplier
  const float kd = 0.015; //kd multiplier
  float error = error_calc();
  integral += error;
  derivative = error - last_error;
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);
  setMultipleMotors(base_speed + adjustSpeed, base_speed - adjustSpeed);
  last_error = error;
  count += 1;
}
