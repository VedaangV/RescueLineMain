//code for obstacle avoidance.
#include "Header.h"

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
}

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


bool seeWall(){
  return false;//update to actual condition once sensor is finalized
}
void avoid(float obstacleDist){//avoids obstacle once detected. obstacleDist is length of object for testing.
  enc_turn(90);
  if(seeWall()){
    enc_turn(180);//turn to other side
    enc = 0;
    while(enc < cm_to_encoders((obstacleDist + 10)*PI)){//the number should be two times the distance stopped before the obstacle
      setMultipleMotors(75, 25);//circle turn around object
    }
    setMultipleMotors(0, 0);
  }
  else{
    enc = 0;
    while(enc < cm_to_encoders((obstacleDist + 10)*PI)){//the number should be two times the distance stopped before the obstacle
      setMultipleMotors(25, 75);//circle turn
    }
    setMultipleMotors(0, 0);
  }
}
