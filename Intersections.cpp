//EV3, NO TURNS, NO SILVER, INCOMPLETE

void setup() 
{
  
}
//****************************************************************************************************************\\

int getbyte()
{
  while(Serial3.available() <= 0)
  {
    delay(0.5);
  }
  return Serial3.read();
}

void get_vals()
{
   

}


int get_color(float green_check)//checks for green based on color vals
{
  get_vals();
  
  bool rcolor = 0;
  bool lcolor = 0;
  
  if (green/red >= green_check)
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if (green/red >= green_check)
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
  switch (get_color(4))
  {
    case 3:
      Serial.println("3");
      //enc_turn(180, 100);
      break;
    case 2:
      Serial.println("2");
      //forwardCm(1.5, 70);
      //enc_turn(90, 100);
      break;
    case 1:
      Serial.println("1");
      //forwardCm(1.5, 70);
      //enc_turn(-90, 100);
      break;
    default:
      Serial.println("0");
      break;
  }

}

void loop() 
{
  greensq();
}
