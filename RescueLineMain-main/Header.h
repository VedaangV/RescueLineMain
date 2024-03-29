#ifndef HEADER_H
#define HEADER_H
#pragma once
//libraries, global vars, and essential functions in here:
#define main_bot 1
//#define back_up_bot 1
//#define debug_greensq
//#define debug_lineTrace
//#define debug_obstacle
//#define debug_blackInt
//#define debug_none
#define SR
//#define vedaanghouse
#include <Adafruit_BNO055.h>
#include <QTRSensors.h>//for qtr
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <SoftwareSerial.h>
#define SERVOPIN A15
#define TCAADDR 0x70
extern QTRSensors qtr;
#ifdef main_bot 1

#ifdef vedaanghouse
#define BLACK_THRESH 1439//greater than means it's black
#define WHITE_THRESH 861 //less than means it's white
#endif

#ifdef SR
#define BLACK_THRESH 1936//greater than means it's black
#define WHITE_THRESH 1000//less than means it's white
#endif

#define leftBlack() ((bw_vals[7] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH))
#define rightBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH))
#else

#ifdef vedaanghouse
#define BLACK_THRESH 1802 //vedaanghouse-5/7
#define WHITE_THRESH 958 //vedaanghouse-5/7
#endif

#ifdef SR
#define BLACK_THRESH 1242 //storming-5/11
#define WHITE_THRESH 654  //storming-5/11
#endif

#define rightBlack() ((bw_vals[7] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH))
#define leftBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH))

#endif
extern float wheelBase;
extern float wheelDia;
extern float botLength;
extern int x;
extern int redPin;
extern int greenPin;
extern int bluePin;
enum led_colors {red, green, blue, off};
extern volatile int enc;

//for line tracing--------------------------------
extern int leftSensor;
extern int rightSensor;
extern int bw_vals[8];
extern const int sensorCount;//# of qtr array sensors
extern float integral; extern float derivative; extern float last_error;
enum qtr_check {different, white, black};
extern int count;

//for intersections-----------------------------
extern const char* serialReq;
enum sides {right, left};
extern const float green_check;
extern const int doubleGreen; extern const int rightGreen; extern const int leftGreen; extern const int silver; extern const int no_green;
enum colors {V = 0, B = 1, G = 2, Y = 3, O = 4, R = 5};
extern int green_count; extern int silver_count;
extern int rsum;
extern int lsum;

//for obstacle--------------------------
extern int trig;
extern int echo;

//functions.cpp---------------------------
void setMultipleMotors(int left, int right);
void grabMotorRun(int16_t speed);
void motorSetup();
void go_motors(int motorSpeed);
void rturn(int motorSpeed);
void lturn(int motorSpeed);
float cm_to_encoders(float cm);
void Interruptfunc();
void forward_enc(int enc, int motor_speed);
void forwardCm(float dist, int motor_speed);
void backward_enc(int enc, int motor_speed);
void backwardCm(float dist, int motor_speed);
void tcaselect(uint8_t i);
void rightMotorRun(int motorSpeed);
void leftMotorRun(int motorSpeed);
void motorsStop();
void set_LED(led_colors);

//enc_turn-----------------------------------
void enc_turn(int deg, int speed);
float getYaw();
float getPitch();
void bnoSetup();
void enc_turn_abs(int deg, int speed);

//intersections------------------------------
int get_color();
void get_vals();
void greensq();
void greensqturn(int turn_target);
void get_ok(sides);
void init_color(sides);

//line tracing------------------------------
void setup_qtr();
float error_calc();
void qtr_print();
void pid_print();
void lineTrace();
void diff_print();
void blackIntersections();


//obstacle-------------------------------
bool seeObs(long dist);
long getCm();
long getInch();
long microToCm(long microseconds);
long microToInch(long microseconds);
void avoid(int sign);
void obstacle();
float getFrontDistance();

//Evac-----------------------------------
void evacSetup();
void dumpKit();

#endif
