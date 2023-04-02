//v10
#include "Header.h"
float rsensor[6];
float lsensor[6];

#ifdef main_bot
#define greenCounter() (green_count > 4) //check if sensor sees green at least __ times in a row before accepting as a "true green" rather than accidental green
#define seeGradient(s)  (s >= 25 && s <= 65)

#else
#define greenCounter() (green_count > 3) //check if sensor sees green at least __ times in a row before accepting as a "true green" rather than accidental green
#define seeGradient(s)  (s >= 17 && s <= 45)
#endif

#define seeSilver (rsensor[G]/rsensor[R] >= 25 && lsensor[G]/lsensor[R] >= 25)
#define silverCounter (silver_count > 7) //check is sensor sees silver at least ___ times in a row before accepting as "true silver" rather than accidental silver
#define falseGreen()  ((check_left() == 2) || (check_right() == 2)) //check if QTR array is seeing half black on either right or left sensors to avoid false green


const float green_check = 11;

const int doubleGreen = 3; const int rightGreen = 2; const int leftGreen = 1;

enum colors {V = 0, B = 1, G = 2, Y = 3, O = 4, R = 5};
enum colorCases {green = 1, silver = 4};

int green_count = 0; int silver_count = 0;
int rsum = 0;
int lsum = 0;


//after reading values, as7262 sensors print out "OK" get_ok functions search for the "OK" on Serial 2 and Serial 3 respectively to remove the "OK" so it doesn't interfere with reading
void get_ok2() //searches on serial 2
{
  while (Serial2.available())
  {
    if (Serial2.read() == 'K')
    {
      break;
    }
  }

}

void get_ok3() //searches on serial 3
{
  while (Serial3.available())
  {
    if (Serial3.read() == 'K')
    {
      break;
    }
  }

}

//****************************************************************************************************************\\

void get_vals()
{
  rsensor[V] = Serial2.parseInt(); //will return as zero because sensor is on bank mode 1
  rsensor[B] = Serial2.parseInt(); //will return as zero because sensor is on bank more 1
  rsensor[G] = Serial2.parseInt();
  rsensor[Y] = Serial2.parseInt();
  rsensor[O] = Serial2.parseInt();
  rsensor[R] = Serial2.parseInt();


  /*
    //extra code for printing- use either this or the above code
    Serial.print(rsensor[V]);
    Serial.print("  ");
    Serial.print(rsensor[B]);
    Serial.print("  ");
    Serial.print(rsensor[G]);
    Serial.print("  ");
    Serial.print(rsensor[Y]);
    Serial.print("  ");
    Serial.print(rsensor[O]);
    Serial.print("  ");
    Serial.print(rsensor[R]);
    Serial.println("  ");*/


  rsum = rsensor[G] + rsensor[Y] + rsensor[O] + rsensor[R];

  //Serial.print("Sum right: ");
  //Serial.println(rsum);




  //---------------------------------------------------------------------------\\




  lsensor[V] = Serial3.parseInt(); //will return as zero because sensor is on bank mode 1
  lsensor[B] = Serial3.parseInt(); //will return as zero because sensor is on bank mode 2
  lsensor[G] = Serial3.parseInt();
  lsensor[Y] = Serial3.parseInt();
  lsensor[O] = Serial3.parseInt();
  lsensor[R] = Serial3.parseInt();


  /*
    //extra code for printing- use either this or above code
    Serial.print(lsensor[V]);
    Serial.print("  ");
    Serial.print(lsensor[B]);
    Serial.print("  ");
    Serial.print(lsensor[G]);
    Serial.print("  ");
    Serial.print(lsensor[Y]);
    Serial.print("  ");
    Serial.print(lsensor[O]);
    Serial.print("  ");
    Serial.print(lsensor[R]);
    Serial.println("  ");*/

  //Since we are using the g/r ratio to detect green, r cannot be equal to zero. if r == 0, set it to 1
  if (rsensor[R] == 0)
  {
    rsensor[R] = 1;
  }

  if (lsensor[R] == 0)
  {
    lsensor[R] = 1;
  }


  lsum = lsensor[G] + lsensor[Y] + lsensor[O] + lsensor[R];

  //Serial.print("Sum Left: ");
  //Serial.println(lsum);


}

void greensqturn(int turn_target) //code for turning after detecting greensq
{
#ifdef main_bot
  if (turn_target < 0) {
    forwardCm(15, 50);
    enc_turn(turn_target, 100);
  }
  else {
    forwardCm(7, 50);
    enc_turn(turn_target, 100);
    backwardCm(4.5, 50);

  }
#else
  forwardCm(9, 50);
  enc_turn(turn_target, 100);
  backwardCm(3, 50);
#endif
}

void nudge() //nudges robot forward to determine accidental green
{
  go_motors(100);
  delay(5);
  motorsStop();
}




int get_color()//checks for green based on color vals
{
  get_vals();

  bool rcolor = 0; //set to 1 if rsensor is determined to be seeing green
  bool lcolor = 0; //set to 1 if lsensor is determined to be seeing green

  // Serial.print("rsensor: ");
  //Serial.println(rsensor[G] / rsensor[R]);//green over red
  // Serial.print("lsensor: ");
  // Serial.println(lsensor[G] / lsensor[R]);//green over red



  if (rsensor[G] / rsensor[R] >= green_check && seeGradient(rsum)) //is right sensor seeing green?
  {
    rcolor = green;
  }

  if (lsensor[G] / lsensor[R] >= green_check && seeGradient(lsum)) //is left sensor seeing green?
  {
    lcolor = green;
  }

  if (seeSilver) //checks for silver
  {
    return (silver);
  }

  return ((rcolor << 1) + lcolor); //creates binary number: 11=double green, 10=right green, 01=left green, 00=no green

}

void greensq()//checks for green and moves accordingly
{
  //setting indicator LED to green color
  digitalWrite(A10, HIGH);
  digitalWrite(A9, LOW);
  digitalWrite(A11, LOW);


  switch (get_color())
  {
    case doubleGreen:
      digitalWrite(A10, LOW);
      digitalWrite(A9, LOW);
      digitalWrite(A11, HIGH);
      //Serial.println("check doubleGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      if (!falseGreen() && greenCounter())
      {
        //Serial.print("DOUBLE GREEN");
        enc_turn(180, 150);
        green_count = 0;
        // Serial.println("zero");
      }
      break;

    case rightGreen:
      //Serial.println("check rightGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      if (!falseGreen() && greenCounter())
      {
        nudge();
        if (get_color() == rightGreen) //recheck to see whether robot is still seeing green after nudge. if yes, proceed to turn.
        {
          // Serial.println("RIGHT GREEN");
          greensqturn(90);
          green_count = 0;
          // Serial.println("zero");
        }

        else
        {
          green_count = 0;
        }

      }
      break;

    case leftGreen:
      // Serial.println("check leftGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      if (!falseGreen() && greenCounter())
      {
        nudge();
        if (get_color() == leftGreen) //recheck to see whether robot is still seeing green after nudge. if yes, proceed to turn.
        {
          //Serial.println("LEFT GREEN");
          greensqturn(-90);
          green_count = 0;
          // Serial.println("zero");
        }

        else
        {
          // Serial.println("zero");
          green_count = 0;
        }
      }
      break;

    case silver:
      silver_count++;
      if (silverCounter) //checks to see silver __ times in a row before accepting
      {
        motorsStop();
        Serial.println("SILVER");
        delay(5000);
      }
      break;

    default: //robot is not detecting green
      green_count = 0;
      //  Serial.println("zero");
      silver_count = 0;
      break;
  }

}
