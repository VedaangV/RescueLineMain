//BASIC GREENSQ, NO SILVER, NO TURNS
#include "Wire.h"
float s1[6];
float s2[6];


void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  
  Serial2.begin(115200);
  Serial3.begin(115200);
  
  Serial2.println("ATINTTIME=1");
  get_ok();
  Serial3.println("ATINTTIME=1");
  get_ok();
}
//****************************************************************************************************************\\


void get_vals() 
{ //gets readings from both AS7262 color sensors

  Serial2.println("ATDATA");
  get_ok();
  s1[0] = Serial2.parseInt();
  s1[1] = Serial2.parseInt();
  s1[2] = Serial2.parseInt();
  s1[3] = Serial2.parseInt();
  s1[4] = Serial2.parseInt();
  s1[5] = Serial2.parseInt();

  Serial3.println("ATDATA");
  get_ok();
  s2[0] = Serial3.parseInt();
  s2[1] = Serial3.parseInt();
  s2[2] = Serial3.parseInt();
  s2[3] = Serial3.parseInt();
  s2[4] = Serial3.parseInt();
  s2[5] = Serial3.parseInt();
}

void get_ok()
{
  while(Serial2.available())
  {
    if(Serial2.read() == 'K')
    {
      //Serial.println("found ok");
      break;
    }
  }

  while(Serial3.available())
  {
    if(Serial3.read() == 'K')
    {
      //Serial.println("found ok");
      break;
    }
  }
}




int get_color(float green_check)//checks for green based on color vals
{
  get_vals();
  
  bool rcolor = 0;
  bool lcolor = 0;

  Serial.print("right: ");
  Serial.print(s1[2]/s1[5]);
  Serial.print("           left: ");
  Serial.println(s2[2]/s2[5]);
  
  
  if (s1[2] / s1[5] >= green_check)
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if (s2[2] / s2[5] >= green_check)
  {
    //Serial.println("left green");
    lcolor = 1;
  }

  return ((rcolor << 1) + lcolor);

}

void greensq()//checks for green and moves accordingly
{
  //perhaps insert code to prevent over-turning???
  //motorsStop();
  switch (get_color(6))
  {
    case 3:
      //Serial.println("3");
      //enc_turn(180, 100);
      break;
    case 2:
      //Serial.println("2");
      //forwardCm(1.5, 70);
      //enc_turn(90, 100);
      break;
    case 1:
      //Serial.println("1");
      //forwardCm(1.5, 70);
      //enc_turn(-90, 100);
      break;
    default:
      //Serial.println("0");
      break;
  }

}

void loop() 
{
  greensq();
}
