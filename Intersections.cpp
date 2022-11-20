//Greensq without turns
int s1[6];
int s2[6];
void get_ok()
{
  //Serial.println("in get ok");
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

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  /*Serial2.println("ATINTTIME=1");
  get_ok();
  Serial2.println("ATBURST=1");
  get_ok();*/
}

int get_color()
{
  bool rcolor = 0;
  bool lcolor = 0;
  //while(!Serial2.available());
  Serial2.println("ATDATA");
  get_ok();

  Serial3.println("ATDATA");
  get_ok();
  
  //Serial.println("-----------------Serial 2---------------------");
  //Serial.print("v:");
  s1[0] = Serial2.parseInt();
  //Serial.print(s1[0]);
  
  //Serial.print(",b:");
  s1[1] = Serial2.parseInt();
  //Serial.print(s1[1]);

  //Serial.print(",g:");
  s1[2]= Serial2.parseInt();
  //Serial.print(s1[2]);

  //Serial.print(",y:");
  s1[3] = Serial2.parseInt();
  //Serial.print(s1[3]);

  //Serial.print(",o:");
  s1[4] = Serial2.parseInt();
  //Serial.print(s1[4]);

  //Serial.print(",r:");
  s1[5] = Serial2.parseInt();
  //Serial.println(s1[5]);

  //Serial.println("\t-----------------Serial 3---------------------");

  //Serial.print("v:");
  s2[0] = Serial3.parseInt();
  //Serial.print(s1[0]);
  
  //Serial.print(",b:");
  s2[1] = Serial3.parseInt();
  //Serial.print(s1[1]);

  //Serial.print(",g:");
  s2[2]= Serial3.parseInt();
  //Serial.print(s1[2]);

  //Serial.print(",y:");
  s2[3] = Serial3.parseInt();
  //Serial.print(s1[3]);

  //Serial.print(",o:");
  s2[4] = Serial3.parseInt();
  //Serial.print(s1[4]);

  //Serial.print(",r:");
  s2[5] = Serial3.parseInt();
  //Serial.println(s1[5]);

  float green_check = 10; //check against certain ratio (ex: green/red)

  if(s1[2]/s1[5] >= green_check)
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if(s2[2]/s2[5] >= green_check)
  {
    //Serial.println("left green");
    lcolor = 1;
  }

  return((rcolor << 1) + lcolor);
  
}

void greensq()
{
    //perhaps insert code to prevent over-turning???
    int green_val = get_color();
    switch (green_val)
    {
      case 3:
        Serial.println("3");
        //enc_turn(180);
        break;
      case 2:
        Serial.println("2");
        //enc_turn(90);
        break;
      case 1:
        Serial.println("1");
        //enc_turn(90);
        break;
      default:
        Serial.println("default/0");
        break;
    }
    
  
}

void loop() 
{
   //int green_val = get_color();
   //Serial.println(green_val);
   greensq();
}

