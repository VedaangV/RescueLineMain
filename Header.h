#ifndef HEADER_H
#define HEADER_H
#pragma once
//libraries, global vars, and essential functions in here:
#include "Megapi_Functions.h"
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <Adafruit_BNO055.h>
#include <QTRSensors.h>//for qtr
extern Adafruit_BNO055 bno;//def in EncTurns.cpp
extern imu::Vector<3> rot;//def in EncTurns.cpp
extern QTRSensors qtr;//def in LineTracing.cpp
extern volatile int enc;//def in functions.cpp

//BNO variables for IMU turns
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
static float yaw = 0.0;

//motor control variables
const int m1_forward = -75;
const int m2_forward = 75;

//greensq arrays
int s1[6];
int s2[6];

//obstacle variables
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;


//all func declarations.

int get_color();1
void setMultipleMotors(int left, int right);
void greensq();
void go_motors(int motorSpeed);
void rturn(int motorSpeed);
void lturn(int motorSpeed);
void setup_qtr();
float error_calc();
void qtr_print();
void pid_print();
void lineTrace();
void diff_print();
void enc_turn(int deg);
void setupBNO();
void getBNO();
float cm_to_encoders(float cm);
void Interruptfunc();
void forward_enc(int enc, int motorSpeed);
void forwardCm(float dist);
bool seeWall();
void avoid();
#endif



