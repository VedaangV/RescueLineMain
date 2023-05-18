//code for obstacle avoidance.
#include "Header.h"
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;
int trig = 4;
int echo = 5;
float wheelBase = 15;
float wheelDia = 4.8;
float botLength = 25;
void distanceISR()
{

  if (pulseFirst)
  {
    pulseMsSt = micros();
    pulseFirst = 0;
  }
  else
  {
    pulseMsEd = micros();
    pulseFirst = 1;
  }
}

float getFrontDistance()//get distance of Front US
{

  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  long pulseTime = pulseIn(echo, HIGH);
  float distance;
  distance = pulseTime * 0.034 / 2;
  if (distance < 0)
  {
    distance = 0;
  }
  #ifdef debug_obstacle
    Serial.print("Distance: ");
    Serial.println(distance);
 #endif
  return (distance);
}

bool seeObs(long dist) { //sees obstacle?
  Serial.println(getFrontDistance());
  if (getFrontDistance() <= dist) {
    return true;
  }
  else {
    return false;
  }
}

/*void avoid(int sign) { //move around obstacle
  bool flag = false;
  forwardCm(15.0, 100);
  enc_turn(-90 * sign, 100);
  forwardCm(40.0, 70);
  enc_turn(-90 * sign, 100);
  int enc1 = enc;
  while (leftBlack() == 0 && rightBlack() == 0) { //forward until black line
    if (enc - enc1 >= cm_to_encoders(24.0) && !flag) { //if traveled more than or 22cm (lost the line)
      enc_turn(-90 * sign, 100);//turn again to find the line
      flag = true;//ensures that it only does this pnce
    }
    go_motors(70);
    qtr.read(bw_vals);
  }
  forwardCm(7.0, 70);
  enc_turn(90 * sign, 100);
}*/
void avoid(int sign){
  float proportion = 17.0;
  float baseSpeed = 8.0;
  float obs_width = 10.0;//assumed width of obstacle for testing
  float bot_dist = 5.0;//the distance between the bot and obstacle before turning (measured)
  float circum = ((obs_width + (2 * bot_dist)) * PI)/2;//calculation of the circumference of the circular path bot must take
  float previous_enc = enc;
  while(enc- previous_enc < 0.2 * (cm_to_encoders(circum))){//until completed at least 20% of the turn
    setMultipleMotors(baseSpeed + (baseSpeed * (proportion-1) * (sign < 0)), ((baseSpeed*proportion) -  (baseSpeed * (proportion-1) * (sign < 0))));//circular turn, speeds: (base_speed * proportion), base_speed
  }
  qtr.read(bw_vals);
  while(leftBlack() == 0 && rightBlack() == 0){//keep turning until black line
    qtr.read(bw_vals);
  }
  forwardCm(5.0, 80);
  qtr.read(bw_vals);
  while(leftBlack() == 0 && rightBlack() == 0){//turn back onto line
    qtr.read(bw_vals);
    rturn(70 * sign);
  }
  forwardCm(0.7, 80);
  enc_turn(7.5 * sign, 120);
  forwardCm(0.7, 80);
  enc_turn(7.5 * sign, 120);
   
}
void obstacle() { //main obstacle function
  if (seeObs(5.0)) {
    #ifdef debug_obstacle
      Serial.println("Saw obstacle");
    #endif
    enc_turn(90, 100);
    if (seeObs(10.0)) { //sees wall
      #ifdef debug_obstacle
        Serial.println("Saw wall");
      #endif       
      enc_turn(180, 100);
     avoid(-1);
    }
    else {
      avoid(1);
    }
  }
}
