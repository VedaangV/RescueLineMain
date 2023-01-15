#ifndef HEADER_H
#define HEADER_H
#pragma once
//libraries, global vars, and essential functions in here:
#include <Adafruit_BNO055.h>
#include <QTRSensors.h>//for qtr
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#define TCAADDR 0x70


//all func declarations.

void setMultipleMotors(int left, int right);
void go_motors(int motorSpeed);
void rturn(int motorSpeed);
void lturn(int motorSpeed);
void setup_qtr();
float error_calc();
void qtr_print();
void pid_print();
void lineTrace();
void diff_print();
float cm_to_encoders(float cm);
void Interruptfunc();
void forward_enc(int enc, int motor_speed);
void forwardCm(float dist, int motor_speed);
void backward_enc(int enc, int motor_speed);
void backwardCm(float dist, int motor_speed);
bool seeObs(long dist);
long getCm();
long getInch();
long microToCm(long microseconds);
long microToInch(long microseconds);
void avoid(int sign);
void obstacle();
void tcaselect(uint8_t i);
int check_tCase();
int check_all();
void enc_turn(int deg, int speed);
int enc_turn_line(int deg, int speed);
void enc_turn2(int deg, int speed);
float getYaw();
void get_ok();
int get_color(float green_check);
void get_vals();
void greensq();
void motorsStop();
void rightMotorRun(int motorSpeed);
void leftMotorRun(int motorSpeed);
void bnoSetup();
void tcaselect(uint8_t i);
int getCase();
float qtr_average(int start, int finish);
#endif
