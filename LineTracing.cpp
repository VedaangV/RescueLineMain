//code for line tracing.
#include "Header.h"
const int sensorCount = 8;//# of qtr array sensors
float integral = 0.0;
float derivative = 0.0;
float last_error = 0.0;
int bw_vals[sensorCount]; //array for qtr vals
int counter = 0;
QTRSensors qtr;
void setup_qtr() {
  qtr.setSensorPins((const uint8_t[]){22, 23, 24, 25, 26, 30, 28, 29}, 8);
  qtr.setTypeRC();
  //qtr.setEmitterPin(2);
}
float error_calc(){
  
  float target_vals[] = {116, 0, -128, 0};//desired difference between sensor pairs
  float multipliers[] = {1.9, 1.6, 1.2, 1.0};//multipliers for each sensor pair
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
  qtr.read(bw_vals);
  for(int i = 0; i < sensorCount/2; i++){
    Serial.print("Pair ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[7 - i] - bw_vals[i]);
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

    else if (bw_vals[i] > 2300 && result == 1) { //if see black when previously white
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < 700 && !case_flag) {
      result = 1;//all white
    }

    else if (bw_vals[i] > 2300 && !case_flag) {
      result = 2;//all black
    }

  }
  return result;
}
int check_left(){
  int result = 0;//0-no uniform val, 1-white, 2 - black
  bool case_flag = false;
  for(int i = 0; i < 4; i++){
    
  if (bw_vals[i] < 500 && result == 2) { //if see white when previously see black
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > 2300 && result == 1) { //if see black when previously white
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < 700 && !case_flag) {
      result = 1;//all white
    }

    else if (bw_vals[i] > 2300 && !case_flag) {
      result = 2;//all black
    }

  }
  return result;
}
int check_right(){
  int result = 0;//0-no uniform val, 1-white, 2 - black
  bool case_flag = false;
  for(int i = 4; i < 7; i++){
    
  if (bw_vals[i] < 500 && result == 2) { //if see white when previously see black
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] > 2300 && result == 1) { //if see black when previously white
      result = 0;//different vals
      case_flag = true;
    }

    else if (bw_vals[i] < 700 && !case_flag) {
      result = 1;//all white
    }

    else if (bw_vals[i] > 2300 && !case_flag) {
      result = 2;//all black
    }

  }
  return result;
}
void lineTrace(){
  
 /* if (check_all() == 2) 
  { //if see all white (gap) or see all black (cross intersection)
    go_motors(80);//forward speed 60
    delay(30);
    return;
  }*/
  /*if((check_left() == 2 || check_right() == 2) && counter >= 5){
    counter = 0;
    go_motors(150);
    delay(90);
    if(check_all() == 1)
    {//see all white
      go_motors(-150);
      delay(90);
    }
  }*/
  digitalWrite(A9, HIGH);
  digitalWrite(A10, LOW);
  digitalWrite(A11, LOW);
  const int base_speed = 55;
  const float kp = 0.05;
  const float ki = 0.0000000;
  const float kd= 0.0;
  float error = error_calc();
  integral += error;
  derivative = error - last_error;
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);
  // MAIN BOT
  setMultipleMotors(base_speed + adjustSpeed, base_speed - adjustSpeed);
  //BACKUP BOT setMultipleMotors(base_speed - adjustSpeed, base_speed + adjustSpeed);
  last_error = error;
  counter++;
}
