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

#define leftWhite() ((bw_vals[7] < WHITE_THRESH) + (bw_vals[6] <WHITE_THRESH) + (bw_vals[5] < WHITE_THRESH) + (bw_vals[4] < WHITE_THRESH))
#define rightWhite() ((bw_vals[3] < WHITE_THRESH) + (bw_vals[2] <WHITE_THRESH) + (bw_vals[1] < WHITE_THRESH) + (bw_vals[0] < WHITE_THRESH))
int leftSensor = 7;
int rightSensor = 0;

#else
const float kp = 0.045;//error multiplier
const float ki = 0.000000;//integral multiplier
const float kd = 0.00; //kd multiplier


#define rightWhite() ((bw_vals[7] < WHITE_THRESH) + (bw_vals[6] <WHITE_THRESH) + (bw_vals[5] < WHITE_THRESH) + (bw_vals[4] < WHITE_THRESH))
#define leftWhite() ((bw_vals[3] < WHITE_THRESH) + (bw_vals[2] <WHITE_THRESH) + (bw_vals[1] < WHITE_THRESH) + (bw_vals[0] < WHITE_THRESH))
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
  #ifdef SR
  float target_vals[] = {48, -48, 344, 296}; //4/16-storming. 8/*FINAL ROBOT*/
  #endif
  float multipliers[] = {1.9, 1.6, 1.2, 1.0};
#else
  #ifdef SR
  float target_vals[] = {-56, -48, -56, -204}; //5/7-storming.
  #endif
  #ifdef vedaanghouse
  float target_vals[] = {0, -156, -208, -112}; //5/7-vedaanghouse
  #endif
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

/*This handles all black intersection cases*/

void lineTrace() {//main line tracking function
  float maxSpeed = 125.0;
  int base_speed = 60  + getPitch();//base speed for Line Tracing
  // gap();
  float error = error_calc();//calculating error
  integral += error;//summing up all erors during runtime
  derivative = error - last_error;//checking the change in the errors over time.
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);//final number that calculates how much to adjust the motors.
#ifdef main_bot
  float leftSpeed = base_speed + adjustSpeed;
  float rightSpeed = base_speed - adjustSpeed;


#else
  float leftSpeed = base_speed - adjustSpeed;
  float rightSpeed = base_speed + adjustSpeed;
#endif
  if (leftSpeed < -maxSpeed) {
    leftSpeed = -maxSpeed;
  }
  if (leftSpeed > maxSpeed) {
    leftSpeed = maxSpeed;
  }
  if (rightSpeed < -maxSpeed) {
    rightSpeed = -maxSpeed;
  }
  if (rightSpeed > maxSpeed) {
    rightSpeed = maxSpeed;
  }
  setMultipleMotors(leftSpeed, rightSpeed);
  last_error = error;
#ifdef main_bot
  //set LED to blue for debugging
  set_LED(blue);
#endif

#ifdef debug_lineTrace
  Serial.print("\n");
  Serial.print("Error: ");
  Serial.println(error);
  Serial.print("Derivative: ");
  Serial.println(derivative);
  Serial.print("Integral: ");
  Serial.println(integral);
  Serial.print("Adjust Speed: ");
  Serial.println(adjustSpeed);
  Serial.print("leftSpeed: ");
  Serial.print(leftSpeed);
  Serial.print("  rightSpeed: ");
  Serial.println(rightSpeed);
#endif


}
