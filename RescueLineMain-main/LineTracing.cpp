//code for line tracing.

#include "Header.h"
#define CALIBRATION_ITERATIONS 100

const int sensorCount = 8;//# of qtr array sensors
int bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0; float derivative = 0.0; float last_error = 0.0;
int count = 0;

#ifdef main_bot
const float kp = 0.035;
const float ki = 0.0000002;
const float kd = 0.0001;

//sensor order for qtr is different on each bot.
//MAIN BOT: sensor order acsends right to left (0 is right most, 7 is left most)
//BACK_UP BOT: sensor order ascends from left to right (0 is left most, 7 is right most);
#define leftBlack() ((bw_vals[7] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH))
#define rightBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH))
int leftSensor = 7;
int rightSensor = 0;

#else
const float kp = 0.0625;//error multiplier
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
  float target_vals[] = {48, -48, 344, 296}; //4/16-storming. 8/*FINAL ROBOT*/
  float multipliers[] = {1.9, 1.6, 1.2, 1.0};
#else
  float target_vals[] = {416, 152, 164, 52}; //4/27-storming.
  float multipliers[] = {2.2, 1.6, 1.2, 1.0};//multipliers for each sensor pair (outer to inner). Outer pairs will have higher multipliers since varying values in them would indicate more extreme cases.
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


void tCase()//case for t-intersection: |--
{
  qtr.read(bw_vals);
  Serial.print("Left Black: ");
  Serial.println(leftBlack());
  Serial.print("Right Black: ");
  Serial.println(rightBlack());
  if ((leftBlack() >= 4) != (rightBlack() >= 4)) // if sees black on either edge, not both
  {
    int turn = ((leftBlack() >= 4) * -90) + ((rightBlack() >= 4) * 90);//turn is -90 for left, 90 for right
    forwardCm(2.5, 70);//move past the intersection case
    qtr.read(bw_vals);
    if (leftBlack() == 0 && rightBlack() == 0) { //all white?(meaning it is a 90 turn)
      enc_turn(turn, 100);//turn 90 degrees
      while (leftBlack() == 0 && rightBlack() == 0) {
        go_motors(-70);//back up until you see black line again
        qtr.read(bw_vals);
      }
      backwardCm(1.5, 70);
    }
    motorsStop();
  }

}
void lineTrace() {//main line tracking function

  int base_speed = 60  + getPitch();//base speed for Line Tracing
  //tCase();

  float error = error_calc();//calculating error
  integral += error;//summing up all erors during runtime
  derivative = error - last_error;//checking the change in the errors over time.
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);//final number that calculates how much to adjust the motors.
  Serial.println("adjustSpeed: ");
  Serial.println(adjustSpeed);
#ifdef main_bot
  setMultipleMotors(base_speed + adjustSpeed, base_speed - adjustSpeed);
#else
  setMultipleMotors(base_speed - adjustSpeed, base_speed + adjustSpeed);
#endif

  last_error = error;
#ifdef main_bot
  //set LED to blue for debugging
  set_LED(blue);
#endif


}
