#include <Megapi_Functions.h>
#include "Utils.h"
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <QTRSensors.h>

QTRSensors qtr;

const int m1_forward = 75;
const int m2_forward = 75;
double m1_speed = 0;
double m2_speed = 0;

int black = 1100;
int white = 200;
int th = (black + white) / 2;
int bwValues[8];

const int SensorCount = 8;
uint16_t sensorValues[SensorCount];
int32_t sensorValues32[SensorCount];


void setup()
{
  megapiSetup();
  Wire.begin();
  Serial.begin(9600);
  Wire.setClock(400);
  
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) { 29, 27, 25, 23, 28, 26, 24, 22 }, SensorCount);
  qtr.setEmitterPin(2);


}

void loop() 
{
  linetrace();
}

int qtr_check()
{
  qtr.read(sensorValues);
  for (int i = 0; i < SensorCount; i++)
  {
    //2500 means dark, 0 means light
    //1 means dark, 0 means light

    if(sensorValues[i] < th)
    {
      bwValues[i] = 0;
    }

    else if (sensorValues[i] > th)
    {
      bwValues[i] = 1;
    }

    else
    {
      bwValues[i] = 0;
    }
    
  }

  int line_values = (bwValues[0]<<7) + (bwValues[1]<<6) + (bwValues[2]<<5) + (bwValues[3]<<4) + (bwValues[4]<<3) + (bwValues[5]<<2) + (bwValues[6]<<1) + (bwValues[7]);
  //Serial2.println(line_values);

  return(line_values);


}

void b_intersection()
{
  //move forward SPECIFIC # OF CM
  int line_val = qtr_check();
  
  if (line_val != 0)
      {
        //go_motors();
      }

      else
      {
         //move backward SPECIFIC # OF CM 
      }

}



void linetrace()
{
  
  switch(line_values)
  {
    case 0:
      //gap
      break;
      
    case 255:
      //forward
      break;
    case 240:
      b_intersection();
      //pid
      break;
      
    case 15:
      b_intersection();
      //pid
      break;

    case 248:
      b_intersection();
      //pid
      break;
      
    case 31:
      b_intersection();
      //pid
      break;
      
    default:
      //run PID
      break;
  }

}
