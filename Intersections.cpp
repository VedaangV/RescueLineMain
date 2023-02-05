//USING MEDIANS AND AS7262
//V6

float s1[6][3];
float s2[6][3];
//float gr_avg = 0;
float s1_gr_med = 0;
float s2_gr_med = 0;


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

void get_vals()
{
  Serial2.println("ATDATA");
  get_ok2();

  s1[0][0] = Serial2.parseInt();
  Serial.print(s1[0][0]);
  Serial.print("  ");
  s1[1][0] = Serial2.parseInt();
  Serial.print(s1[1][0]);
  Serial.print("  ");
  s1[2][0] = Serial2.parseInt();
  Serial.print(s1[2][0]);
  Serial.print("  ");
  s1[3][0] = Serial2.parseInt();
  Serial.print(s1[3][0]);
  Serial.print("  ");
  s1[4][0] = Serial2.parseInt();
  Serial.print(s1[4][0]);
  Serial.print("  ");
  s1[5][0] = Serial2.parseInt();
  Serial.print(s1[5][0]);
  Serial.println("  ");

Serial2.println("ATDATA");
  get_ok2();
  
  s1[0][1] = Serial2.parseInt();
  Serial.print(s1[0][1]);
  Serial.print("  ");
  s1[1][1] = Serial2.parseInt();
  Serial.print(s1[1][1]);
  Serial.print("  ");
  s1[2][1] = Serial2.parseInt();
  Serial.print(s1[2][1]);
  Serial.print("  ");
  s1[3][1] = Serial2.parseInt();
  Serial.print(s1[3][1]);
  Serial.print("  ");
  s1[4][1] = Serial2.parseInt();
  Serial.print(s1[4][1]);
  Serial.print("  ");
  s1[5][1] = Serial2.parseInt();
  Serial.print(s1[5][1]);
  Serial.println("  ");

Serial2.println("ATDATA");
  get_ok2();
  
  s1[0][2] = Serial2.parseInt();
  Serial.print(s1[0][2]);
  Serial.print("  ");
  s1[1][2] = Serial2.parseInt();
  Serial.print(s1[1][2]);
  Serial.print("  ");
  s1[2][2] = Serial2.parseInt();
  Serial.print(s1[2][2]);
  Serial.print("  ");
  s1[3][2] = Serial2.parseInt();
  Serial.print(s1[3][2]);
  Serial.print("  ");
  s1[4][2] = Serial2.parseInt();
  Serial.print(s1[4][2]);
  Serial.print("  ");
  s1[5][2] = Serial2.parseInt();
  Serial.print(s1[5][2]);
  Serial.println("  ");


//---------------------------------------------------------------------------\\


  Serial3.println("ATDATA");
  get_ok3();

  
  s2[0][0] = Serial3.parseInt();
  Serial.print(s2[0][0]);
  Serial.print("  ");
  s2[1][0] = Serial3.parseInt();
  Serial.print(s2[1][0]);
  Serial.print("  ");
  s2[2][0] = Serial3.parseInt();
  Serial.print(s2[2][0]);
  Serial.print("  ");
  s2[3][0] = Serial3.parseInt();
  Serial.print(s2[3][0]);
  Serial.print("  ");
  s2[4][0] = Serial3.parseInt();
  Serial.print(s2[4][0]);
  Serial.print("  ");
  s2[5][0] = Serial3.parseInt();
  Serial.print(s2[5][0]);
  Serial.println("  ");

  Serial3.println("ATDATA");
  get_ok3();
  
  s2[0][1] = Serial3.parseInt();
  Serial.print(s2[0][1]);
  Serial.print("  ");
  s2[1][1] = Serial3.parseInt();
  Serial.print(s2[1][1]);
  Serial.print("  ");
  s2[2][1] = Serial3.parseInt();
  Serial.print(s2[2][1]);
  Serial.print("  ");
  s2[3][1] = Serial3.parseInt();
  Serial.print(s2[3][1]);
  Serial.print("  ");
  s2[4][1] = Serial3.parseInt();
  Serial.print(s2[4][1]);
  Serial.print("  ");
  s2[5][1] = Serial3.parseInt();
  Serial.print(s2[5][1]);
  Serial.println("  ");

  Serial3.println("ATDATA");
  get_ok3();

  s2[0][2] = Serial3.parseInt();
  Serial.print(s2[0][2]);
  Serial.print("  ");
  s2[1][2] = Serial3.parseInt();
  Serial.print(s2[1][2]);
  Serial.print("  ");
  s2[2][2] = Serial3.parseInt();
  Serial.print(s2[2][2]);
  Serial.print("  ");
  s2[3][2] = Serial3.parseInt();
  Serial.print(s2[3][2]);
  Serial.print("  ");
  s2[4][2] = Serial3.parseInt();
  Serial.print(s2[4][2]);
  Serial.print("  ");
  s2[5][2] = Serial3.parseInt();
  Serial.print(s2[5][2]);
  Serial.println("  ");
  
}

float median(float arr[3])
{
  float temp;
  for (int i = 0; i < 3; i++) 
  {     
    for (int j = i+1; j < 3; j++) 
    {     
      if(arr[i] > arr[j]) 
      {    
        temp = arr[i];    
        arr[i] = arr[j];    
        arr[j] = temp;    
      }     
    }     
  }

  return(arr[1]);
}

int get_color()//checks for green based on color vals
{
  get_vals();
 
  s1_gr_med = median(s1[2]) / median(s1[5];
  Serial.print("                    Median1                                                       ");
  Serial.println(s1_gr_med);


  s2_gr_med = median(s2[2]) / median(s2[5];
  Serial.print("                    Median 2                                                      ");
  Serial.println(s2_gr_med);
  
  bool rcolor = 0;
  bool lcolor = 0;
  
  if (s1_gr_med >= green_check)
  {
    //Serial.println("right green");
    rcolor = 1;
  }

  if (s2_gr_med >= green_check)
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
