//code for line tracing.
QTRSensors qtr;
void setup_qtr() {
  int qtr_pins[] = {22, 23, 24, 25, 26, 27, 28, 29};
  
  qtr.setTypeRC();
  qtr.setEmitterPin(30);
  qtr.resetCalibration();

  for (int i = 0; i < 100; i++) {//100 iterations
    qtr.calibrate();
    qtr.setSensorPins((const uint8_t[]) qtr_pins[], sensorCount);
  }
}
float error_calc(){
  
  float target_vals[] = {0, 0, 0, 0};//desired difference between sensor pairs
  float multipliers[] = {1, 1, 1, 1};//multipliers for each sensor pair
  float error = 0.0;//error in PID
  qtr.read(bw_vals);
  for(int i = 0; i < sensorCount / 2; i++){//iterates through all sensor pairs, outer to inner
    float current_val = (bw_vals[sensorCount - i] - bw_vals[i])//difference between current pair of sensors
    float error += (target_vals[i] - current_val) * multipliers[i]; //difference between desired difference of sensor vals and actual difference of sensor vals. Each pair has its own multiplier.
  }
  return error;
}
void qtr_print(){
  qtr.read(bw_vals);
  for(int i = 0; i < sensorCount; i++){
    Serial.println("Sensor %d: %d", i, bw_vals[i]);
  }
}
void pid_print(){
  Serial.println("PID error: %f", error_calc());
}
void lineTrace(){
  const int base_speed = 50;
  const float kp = 1.0;
  const float ki = 0.0;
  const float kd= 0.0;
  float error = error_calc();
  integral += error;
  derivative = error - last_error;
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);
  lmotor.run(base_speed + adjustSpeed);
  rmotor.run(base_speed - adjustSpeed);
  last_error = error;
}
