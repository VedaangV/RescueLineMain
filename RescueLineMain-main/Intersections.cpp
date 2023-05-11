//v10
#include "Header.h"
float rsensor[6];
float lsensor[6];

#ifdef main_bot
#define greenCounter() (green_count > 4) //check if sensor sees green at least __ times in a row before accepting as a "true green" rather than accidental green
#define seeGradient(s)  (s >= 25 && s <= 65)

#else
#define greenCounter() (green_count > 2) //check if sensor sees green at least __ times in a row before accepting as a "true green" rather than accidental green
#define seeGradient(s)  (s >= 17 && s <= 45)
#endif

#define seeSilver (rsum > 300 || lsum > 300)
#define silverCounter (silver_count > 5) //check is sensor sees silver at least ___ times in a row before accepting as "true silver" rather than accidental silver
#define falseGreen()  ((leftBlack() >= 2) || (rightBlack() >= 2)) //check if QTR array is seeing half black on either right or left sensors to avoid false green

const float green_checkL = 0;//16.0;
const float green_checkR = 0;//15.0;

const int doubleGreen = 3; const int rightGreen = 2; const int leftGreen = 1; const int silver = 4;
int green_count = 0; int silver_count = 0;
int rsum = 0;
int lsum = 0;




//after reading values, as7262 sensors print out "OK" get_ok functions search for the "OK" on Serial 2 and Serial 3 respectively to remove the "OK" so it doesn't interfere with reading
void init_color(sides sensor) {
  switch (sensor) {
    case right:
      Serial2.begin(115200);
      Serial2.println("ATINTTIME=1");
      get_ok(right);
      Serial2.println("ATGAIN=1");
      get_ok(right);
      //Serial2.println("ATTCSMD=1");
     // get_ok(right);
      while (Serial2.available() == 0)
      {
        Serial.println("Serial 2 not available");
      }
      Serial.println("serial 2 available");
      get_ok(right);

    case left:
      Serial3.begin(115200);
      Serial3.println("ATINTTIME=1");
      get_ok(left);
      Serial3.println("ATGAIN=1");
      get_ok(left);
      //Serial3.println("ATTCSMD=1");
     // get_ok(left);
      while (Serial3.available() == 0)
      {
        Serial.println("Serial 3 not available");
      }
      Serial.println("serial 3 available");
      get_ok(left);
  }
}
void get_ok(sides sensor) //searches on serial 2
{
  switch (sensor) {
    case right:
      while (Serial2.available())
      {
        if (Serial2.read() == 'K')
          break;
      }
    case left:
      while (Serial3.available())
      {
        if (Serial3.read() == 'K')
          break;
      }

  }



}



void get_vals()
{


  rsensor[V] = Serial2.parseInt(); //will return as zero because sensor is on bank mode 1
  rsensor[B] = Serial2.parseInt(); //will return as zero because sensor is on bank more 1
  rsensor[G] = Serial2.parseInt();
  rsensor[Y] = Serial2.parseInt();
  rsensor[O] = Serial2.parseInt();
  rsensor[R] = Serial2.parseInt();




  //extra code for printing- use either this or the above code



  rsum = rsensor[G] + rsensor[Y] + rsensor[O] + rsensor[R];






  //---------------------------------------------------------------------------\\

  lsensor[V] = Serial3.parseInt(); //will return as zero because sensor is on bank mode 1
  lsensor[B] = Serial3.parseInt(); //will return as zero because sensor is on bank mode 2
  lsensor[G] = Serial3.parseInt();
  lsensor[Y] = Serial3.parseInt();
  lsensor[O] = Serial3.parseInt();
  lsensor[R] = Serial3.parseInt();




  //extra code for printing- use either this or above code


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


  #ifdef debug_greensq  
    Serial.println("rsensor");
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
    Serial.println("  ");
    Serial.println("lsensor");
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
    Serial.println("  ");    
    Serial.print("g/r ratio left: ");
    Serial.println(lsensor[G]/lsensor[R]);
    Serial.print("g/r ratio right: ");
    Serial.println(rsensor[G]/rsensor[R]);
    Serial.print("Sum right: ");
    Serial.println(rsum);
    Serial.print("Sum Left: ");
    Serial.println(lsum);
  #endif
    



}

void greensqturn(int turn_target) //code for turning after detecting greensq
{
#ifdef main_bot
  if (turn_target < 0) {
    forwardCm(7, 50);
    enc_turn_abs(turn_target, 100);
    backwardCm(3.0, 50);

  }
  else {
    forwardCm(7, 50);
    enc_turn_abs(turn_target, 100);
    backwardCm(4.0, 50);

  }
#else
if(turn_target < 0){
  forwardCm(9.5, 50);  
  enc_turn(-70, 100);
  qtr.read(bw_vals);
  while(bw_vals[leftSensor] < WHITE_THRESH){
    lturn(80);
    qtr.read(bw_vals);
  }
  enc_turn(-10, 90);
  backwardCm(2.5, 50);
}
if(turn_target > 0){
  forwardCm(9.5, 50);  
  enc_turn(70, 100);
  qtr.read(bw_vals);
  while(bw_vals[rightSensor] < WHITE_THRESH){
    rturn(80);
    qtr.read(bw_vals);
  }
  enc_turn(10, 90);  
  backwardCm(2.5, 50);
}
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
  float br_ratio = 4.0;
  get_vals();

  bool rcolor = 0; //set to 1 if rsensor is determined to be seeing green
  bool lcolor = 0; //set to 1 if lsensor is determined to be seeing green



  qtr.read(bw_vals);
  if (rsensor[G] / rsensor[R] >= green_checkR && rsensor[B]/rsensor[R] >= br_ratio && !falseGreen()) //is right sensor seeing green?
  {
    rcolor = 1;
  }

  if (lsensor[G] / lsensor[R] >= green_checkL && lsensor[B]/lsensor[R] >= br_ratio && !falseGreen()) //is left sensor seeing green?
  {
    lcolor = 1;
  }
  #ifdef debug_greensq
    if(falseGreen()){
      Serial.println("FALSE GREEN");
    }
#endif
  if (seeSilver) //checks for silver
  {
    return (4);
  }
  #ifdef debug_greensq
    Serial.print("\n");
    Serial.print("get_color: ");
    Serial.println((rcolor << 1) + lcolor);
   #endif
    
  return ((rcolor << 1) + lcolor); //creates binary number: 11=double green, 10=right green, 01=left green, 00=no green

}



void greensq()//checks for green and moves accordingly
{
  //setting indicator LED to green color
#ifdef main_bot
  set_LED(green);
#endif


  switch (get_color())
  {
    case doubleGreen:
      set_LED(red);
      Serial.println("check doubleGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      qtr.read(bw_vals);
      if (!falseGreen() && greenCounter())
      {
        Serial.print("DOUBLE GREEN");

        set_LED(off);
        enc_turn(180, 150);
        green_count = 0;
        // Serial.println("zero");
      }
      break;

    case rightGreen:
      Serial.println("check rightGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      qtr.read(bw_vals);

      if (!falseGreen() && greenCounter())
      {
        //nudge();
        Serial2.println(serialReq);
        Serial3.println(serialReq);
        // if (get_color() == 2) //recheck to see whether robot is still seeing green after nudge. if yes, proceed to turn.
        //{

        set_LED(off);
        Serial.println("RIGHT GREEN");
        greensqturn(90);
        green_count = 0;
        // Serial.println("zero");
        //}

        /*  else
          {
            green_count = 0;
           }*/

      }
      break;

    case leftGreen:
      Serial.println("check leftGreen"); //at this point, the robot has seen at least 1 green value in a row, and is checking for more
      green_count++;
      silver_count = 0;
      qtr.read(bw_vals);
      if (!falseGreen() && greenCounter())
      {
        //    nudge();
        Serial2.println(serialReq);
        Serial3.println(serialReq);
        //    if (get_color() == 1) //recheck to see whether robot is still seeing green after nudge. if yes, proceed to turn.
        //    {

        set_LED(off);
        Serial.println("LEFT GREEN");
        greensqturn(-90);
        green_count = 0;
        // Serial.println("zero");
        //    }

        /*   else
          {
             Serial.println("zero");
            green_count = 0;
          }*/
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
