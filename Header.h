#pragma once
//libraries, global vars, and essential functions in here:
#include <Adafruit_BNO055.h>
#define BNO055_SAMPLERATE_DELAY_MS (10)
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
imu::Vector<3> rot;
#include <QTRSensors.h>//for qtr
#include "MeMegaPi.h"//for megaPi
QTRSensors qtr;
MeMegaPiDCMotor lmotor(PORT1B);//left motor
MeMegaPiDCMotor rmotor(PORT2B);//right motor
int sensorCount = 8;//# of qtr array sensors
float bw_vals[sensorCount]; //array for qtr vals
float integral = 0.0;
float derivative = 0.0
float last_error = 0.0;
//
//all func declarations.
void get_color();
void greensq();
void go_motors(int motorSpeed);
void rturn(int motorSpeed);
void lturn(int motorSpeed);
void setup_qtr();
float error_calc();
void qtr_print();
void pid_print();
void lineTrace();



