#pragma once
//libraries, global vars, and essential functions in here:
#include <Adafruit_BNO055.h>
#define BNO055_SAMPLERATE_DELAY_MS (10)
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
imu::Vector<3> rot;
//
//all func declarations.
void get_color();
void greensq();

