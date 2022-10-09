//code for line tracing.
#include "Header.h"
const int sensorCount = 8;//# of qtr array sensors
int bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0;
float derivative = 0.0;
float last_error = 0.0;
QTRSensors qtr;
void setup_qtr() {
  const uint8_t qtr_pins[] = {22, 23, 24, 25, 26, 27, 28, 29};
  
  qtr.setTypeRC();
  qtr.setEmitterPin(30);
  qtr.resetCalibration();

  for (int i = 0; i < 100; i++) {//100 iterations
    qtr.calibrate();
    qtr.setSensorPins(qtr_pins, sensorCount);
  }
}
float error_calc(){
  
  float target_vals[] = {48, -808, 48, 108};//desired difference between sensor pairs
  float multipliers[] = {1, 1, 1, 1};//multipliers for each sensor pair
  float error = 0.0;//error in PID
  qtr.read(bw_vals);
  for(int i = 0; i < sensorCount / 2; i++){//iterates through all sensor pairs, outer to inner
    float current_val = (bw_vals[7 - i] - bw_vals[i]);//difference between current pair of sensors
    error += (target_vals[i] - current_val) * multipliers[i]; //difference between desired difference of sensor vals and actual difference of sensor vals. Each pair has its own multiplier.
  }
  return error;
}
void qtr_print(){
  qtr.read(bw_vals);
  for(int i = 0; i < sensorCount; i++){
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[i]);
    
  }
}
void pid_print(){
  Serial.print("PID error: "); Serial.println(error_calc());
  
}
void diff_print(){
  for(int i = 0; i < sensorCount/2; i++){
    Serial.print("Pair ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[7 - i] - bw_vals[i]);
  }
}
void lineTrace(){
  const int base_speed = 65;
  const float kp = 0.01;
  const float ki = 0.0;
  const float kd= 0.0;
  float error = error_calc();
  integral += error;
  derivative = error - last_error;
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);
  setMultipleMotors(base_speed + adjustSpeed, base_speed - adjustSpeed);
  last_error = error;
}
