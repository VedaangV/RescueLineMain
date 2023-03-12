float s1[6];
float s2[6];
//float gr_avg = 0;

float green_check = 11;
int green_count = 0;

void get_ok2()
{
  //Serial.println("Serail 2 Available  ");
  //Serial.println(Serial2.available());

  while(Serial2.available())
  {
    if(Serial2.read() == 'K')
    {
      //Serial.println("found ok");
      break;
    }
  }
  
}

void get_ok3()
{
  //Serial.println("Serail 3 Available  ");
  //Serial.println(Serial3.available());

  while(Serial3.available())
  {
    if(Serial3.read() == 'K')
    {
      //Serial.println("found ok");
      break;
    }
  }
  
}


void setup() 
{
  Serial.begin(115200);
  
  Serial2.begin(115200);
  Serial2.println("ATINTTIME=1");

  while(Serial2.available()==0)
  {
    Serial.println("Serial 2 not available");
  }
  get_ok2();

  Serial3.begin(115200);
  Serial3.println("ATINTTIME=1");

  while(Serial3.available()== 0)
  {
    Serial.println("Serial 3 not available");
  }
  
  get_ok3();

  //Serial2.println("ATBURST=1");
  //get_ok2();

  //Serial3.println("ATBURST=1");
  //get_ok3();

    /*get_ok();
  Serial2.println("ATBURST=1");
  get_ok();*/
  
}

//****************************************************************************************************************\\


void get_vals()
{
  Serial2.println("ATDATA");
  get_ok2();

  s1[0] = Serial2.parseInt();
  s1[1] = Serial2.parseInt();
  s1[2] = Serial2.parseInt();
  s1[3] = Serial2.parseInt();
  s1[4] = Serial2.parseInt();
  s1[5] = Serial2.parseInt();
  
  /*Serial.print(s1[0]);
  Serial.print("  ");
  Serial.print(s1[1]);
  Serial.print("  ");
  Serial.print(s1[2]);
  Serial.print("  ");
  Serial.print(s1[3]);
  Serial.print("  ");
  Serial.print(s1[4]);
  Serial.print("  ");
  Serial.print(s1[5]);
  Serial.println("  ");*/

  


//---------------------------------------------------------------------------\\


  Serial3.println("ATDATA");
  get_ok3();

  
  s2[0] = Serial3.parseInt();
  s2[1] = Serial3.parseInt();
  s2[2] = Serial3.parseInt();
  s2[3] = Serial3.parseInt();
  s2[4] = Serial3.parseInt();
  s2[5] = Serial3.parseInt();
  

  /*Serial.print(s2[0]);
  Serial.print("  ");
  Serial.print(s2[1]);
  Serial.print("  ");
  Serial.print(s2[2]);
  Serial.print("  ");
  Serial.print(s2[3]);
  Serial.print("  ");
  Serial.print(s2[4]);
  Serial.print("  ");
  Serial.print(s2[5]);
  Serial.println("  ");*/

  //Since we are using the g/r ratio to detect green, r cannot be equal to zero. if r == 0, set it to 1
  if(s1[5] == 0)
  {
    s1[5] = 1;
  }

  if(s2[5] == 0)
  {
    s2[5] = 1;
  }

  
}






int get_color()//checks for green based on color vals
{
  get_vals();
 
  bool rcolor = 0;
  bool lcolor = 0;

   
  if (s1[2]/s1[5] >= green_check)
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if (s2[2]/s2[5] >= green_check)
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
  switch (get_color())
  {
    case 3:
      Serial.println("check 3");
      green_count++;
      if(green_count > 5)
      {
        enc_turn(180, 100);
        Serial.print("GREEN 3");
        green_count = 0;
      }
      break;
      
    case 2:
      Serial.println("check 2");
      green_count++;
      if(green_count > 5)
      {
        //forwardCm(1.5, 70);
        enc_turn(90, 100);
        Serial.print("GREEN 2");
        green_count = 0;
      }
      break;

    case 1:
      Serial.println("check 1");
      green_count++;
      if(green_count > 5)
      {
        //forwardCm(1.5, 70);
        enc_turn(-90, 100);
        Serial.print("GREEN 1");
        green_count = 0;
      }
      break;
      
    default:
      Serial.println("0");
      green_count = 0;
      break;
  }

}

void loop() 
{
  greensq();
}
