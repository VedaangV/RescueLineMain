//BASIC LOGIC FOR GREENSQ
#include "Header.h"
float meanLeft = 0.0;
float meanRight = 0.0;
//float greenWhiteThresh = 501.0;
//float greenBlackThresh = 1499.0;
int s1[6];
int s2[6];

void get_vals() { //gets readings from both AS7262 color sensors

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

  /* Serial.print("s1: ");
    Serial.print(s1[2]/s1[5]);
    Serial.print(" S1 GREEN: ");
    Serial.println(s1[2]);
    Serial.print("s2: ");
    Serial.print(s2[2]/s2[5]);
    Serial.print(" S2 GREEN: ");
    Serial.println(s2[2]);*/

  //print vals and calculate the mean on left and right

  Serial.println("Left values (vbgyor): ");


  for (int i = 0; i < 6; i++) {
    Serial.print(s2[i]);
    Serial.print(" ");
    meanLeft += s2[i];
  }

  meanLeft /= 6;
  Serial.print("\nMean Left: ");
  Serial.print(meanLeft);
  Serial.println("\n");
  Serial.println("Right values (vbgyor): ");

  for (int i = 0; i < 6; i++) {
    Serial.print(s1[i]);
    Serial.print(" ");
    meanRight += s1[i];
  }

  meanRight /= 6;
  Serial.print("\nMean Right: ");
  Serial.print(meanRight);
}

void get_ok()//communication through serial for color sensors
{
  //Serial.println("in get ok");

  while (Serial2.available())
  {


    if (Serial2.read() == 'K')
    {
      //Serial.println("found 2 ok");
      break;
    }
  }
  while (Serial3.available())
  {

    if (Serial3.read() == 'K')
    {
      //Serial.println("found 3 ok");
      break;
    }
  }
}




int get_color(float green_check)//checks for green based on color vals
{
  get_vals();

  bool rcolor = 0;
  bool lcolor = 0;

  if ((s1[2] / s1[5] >= green_check && meanRight < 12))
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if (s2[2] / s2[5] >= green_check && meanLeft < 12)
  {
    //Serial.println("left green");
    lcolor = 1;
  }


  return ((rcolor * 2) + lcolor);

}


void greensq()//checks for green and moves accordingly
{
  //perhaps insert code to prevent over-turning???
  switch (get_color(12))
  {
    case 3:
      motorsStop();
      delay(700);
      Serial.println("3");
      forwardCm(5.5, 70);
      enc_turn(180, 100);
      backwardCm(1.0, 70);
      break;
    case 2:
      motorsStop();
      delay(700);
      Serial.println("2");
      forwardCm(5.5, 70);
      enc_turn(90, 100);
      backwardCm(1.0, 70);

      break;
    case 1:
      motorsStop();
      delay(700);
      Serial.println("1");
      forwardCm(5.5, 70);
      enc_turn(-90, 100);
      backwardCm(1.0, 70);

      break;
    default:
      Serial.println("default/0");
      break;
  }



}
