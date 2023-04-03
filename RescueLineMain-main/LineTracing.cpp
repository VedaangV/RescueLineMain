//code for line tracing.

#include "Header.h"
#define CALIBRATION_ITERATIONS 100

const int sensorCount = 8;//# of qtr array sensors
int bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0; float derivative = 0.0; float last_error = 0.0;
enum qtr_check {different, white, black};
int count = 0;
#ifdef main_bot
const float kp = 0.035;
const float ki = 0.0000002;
const float kd = 0.000;
//sensor order for qtr is different on each bot.
//MAIN BOT: sensor order acsends right to left (0 is right most, 7 is left most)
//BACK_UP BOT: sensor order ascends from left to right (0 is left most, 7 is right most);
#define leftBlack() ((bw_vals[7] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH))
#define rightBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH))
int leftSensor = 7;
int rightSensor = 0;
#else
const float kp = 0.01;//error multiplier
const float ki = 0.000000;//integral multiplier
const float kd = 0.00; //kd multiplier
#define rightBlack() ((bw_vals[7] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH))
#define leftBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH))
int leftSensor = 0;
int rightSensor = 7;
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
  float target_vals[] = {420, 220, 264, 204}; //4/2-home.
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

  Serial.print("\n");
  for (int i = 0; i < sensorCount; i++) {
    qtr.read(bw_vals);
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
  
  for (int i = 0; i < sensorCount / 2; i++) {
    qtr.read(bw_vals);
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
int check_right() {//check all sensors to see if they have the same relative values


  qtr.read(bw_vals);
  int result = different;
  bool case_flag = false;

  for (int i = 0; i < 4 ; i++) {
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
  //set led to red
  #ifdef main_bot
  digitalWrite(A11, HIGH);
  digitalWrite(A10, LOW);
  digitalWrite(A9, LOW);
  #endif
  
  const int turnDeg = 30;
  bool seeBlack = false;
  qtr.read(bw_vals);
  if (leftBlack() >= 4 || rightBlack() >= 4) // if sees black on either edge
  {

    if (leftBlack() >= 4) {//if t_case on left side
      forwardCm(4.0, 80);//go forward 
      if(enc_turn(-1 * turnDeg, 80, rightSensor)){//turn towards the line branching off and check if the opposite side sensor ever sees black (meaning there is a line ahead.
        //if there is a line ahead (robot should go forward, its a t-case not 90)
        enc_turn(turnDeg, 80);//turn back to straight position
        forwardCm(2.0, 80);//forward 2cm
        delay(2000);
      }
      else{
        enc_turn(-1 * (90 - turnDeg), 80);
      }
    }
    else {//if t_case on right side
      forwardCm(4.0, 80);
     if(enc_turn(turnDeg, 80, leftSensor)){
      enc_turn(-1 * turnDeg, 80);
      forwardCm(2.0, 80);
     }
     else{
      enc_turn(90 - turnDeg, 80);
     }
    }
  }
  
}
void lineTrace() {//main line tracking function

  int base_speed = 55 + getPitch();//base speed for Line Tracing
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
  #ifdef main_bot
  digitalWrite(A10, LOW);
  digitalWrite(A9, HIGH);
  digitalWrite(A11, LOW);
  #endif


}
