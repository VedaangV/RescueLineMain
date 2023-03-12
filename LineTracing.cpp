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
enum qtr_check {different, white, black};

void setup_qtr() {
  //setup for the qtr sensor
#define CALIBRATION_ITERATIONS 100
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
  float target_vals[] = {368, 160, 320, 344}; //2/26-storming.
  //float target_vals[] = {380, 168, 223, 168}; //3/4-home.

  float multipliers[] = {6.5, 4.33, 2.67, 1.00};//multipliers for each sensor pair (outer to inner). Outer pairs will have higher multipliers since varying values in them would indicate more extreme cases.
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

int check_all() {//check all sensors to see if they have the same relative values
#define BLACK_THRESH 1000
#define WHITE_THRESH 500

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
void lineTrace() {//main line tracking function
  const int base_speed = 65;//base speed for Line Tracing
  const float kp = 0.05;//error multiplier
  const float ki = 0.00000;//integral multiplier
  const float kd = 0.0; //kd multiplier
  if (check_all() == white ||  check_all() == black) { //if see all white (gap) or see all black (cross intersection)
    go_motors(base_speed);//move forward
    return;
  }


  float error = error_calc();//calculating error
  integral += error;//summing up all errors during runtime
  derivative = error - last_error;//checking the change in the errors over time.
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);//final number that calculates how much to adjust the motors.
  setMultipleMotors(base_speed - adjustSpeed, base_speed + adjustSpeed);
  last_error = error;
}
