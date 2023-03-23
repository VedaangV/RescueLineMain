//code for obstacle avoidance.
#include "Header.h"
/*
void distanceISR()
{
 if(pulseFirst)
 {
  pulseMsSt= micros(); 
  pulseFirst = 0;
 }
 else
 {
  pulseMsEd = micros();
  pulseFirst = 1;
 }
}*/
/*
float getFrontDistance()
{
  long pulseTime = pulseMsEd - pulseMsSt;
  float distance;
  distance = pulseTime * 0.034 / 2;
  if(distance < 0)
  {
    distance = 0;
  }
  return(distance);
}

void check_obstacle()
{
   float dist = getFrontDistance();
   if 
}
if (dist < obstacleDistance && frontDist != 0)
  {
    motorsStop();
    delay(500);
    //centerOnLine(); not necissairly needed- more tests required
    motorsStop();
    backward_cm(5);
    //tone(45, 1000,500);
    delay(500);
    point90right(90,0);
    delay(200);
    
    forward_cm(14);
    delay(200);

    point90left(90,0);
    delay(200);

    forward_cm(45);
    delay(200);

    point90left(90,0);
    delay(200);
    
    forward_cm(14);
    delay(200);

    point90right(90,0);
    //delay(2000);
    //float outerSpeed;
    //outerSpeed = (1+(trackwidth/radius))* m2_speed;
    //delay(2000);
    /*
    qtr.read(sensorValues);
    //prop_turn(outerSpeed, m2_speed);
    
    delay(500);
    qtr.read(sensorValues);
    while((sensorValues[0] < th && sensorValues[1] < th && sensorValues[2] < th && sensorValues[3] < th && sensorValues[4] < th && sensorValues[5] < th && sensorValues[6] < th && sensorValues[7] < th))
    {
      qtr.read(sensorValues);
    }*//*
    
    motorsStop();
    /*
    Serial2.println("black");
    delay(1000);
    rturn(128);
    delay(250);
    motorsStop();
    delay(1000);*//*
    for(int i =0; i<4; i++)
    {
      if(pulseFirst == 1)
      {
        digitalWrite(A10,HIGH);
        delayMicroseconds(12);
        digitalWrite(A10,LOW);
      }
      delay(500);
    }
    
  }
}*/
