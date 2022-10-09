#ifndef HEADER_H
#define HEADER_H
#pragma once
//libraries, global vars, and essential functions in here:
#include <Adafruit_BNO055.h>
#include <QTRSensors.h>//for qtr
extern Adafruit_BNO055 bno;//def in EncTurns.cpp
extern imu::Vector<3> rot;//def in EncTurns.cpp
extern QTRSensors qtr;//def in LineTracing.cpp
extern volatile int enc;//def in functions.cpp

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



