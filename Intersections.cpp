//unfinished
int rcolor = 0;
int lcolor = 0;


float v = 0;
float b = 0;
float g = 0;
float y = 0;
float o = 0;
float r = 0;


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
}

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200);
  /*Serial2.println("ATINTTIME=1");
  get_ok();
  Serial2.println("ATBURST=1");
  get_ok();*/
}

int get_color()
{
    //while(!Serial2.available());
  Serial2.println("ATDATA");
  get_ok();

  //ratio = 4.5
  
  //Serial.println(Serial2.available());
  //Serial.println(Serial2.read());
  
  /*Serial.print("v:");
  Serial.print(Serial2.parseInt());
  
  Serial.print(",b:");
  Serial.print(Serial2.parseInt());

  Serial.print(",g:");
  Serial.print(Serial2.parseInt());

  Serial.print(",y:");
  Serial.print(Serial2.parseInt());

  Serial.print(",o:");
  Serial.print(Serial2.parseInt());

  Serial.print(",r:");
  Serial.println(Serial2.parseInt());*/
  
  //Serial.print("v:");
  v = Serial2.parseInt();
  //Serial.print(v);
  
  //Serial.print(",b:");
  b = Serial2.parseInt();
  //Serial.print(b);

  //Serial.print(",g:");
  g = Serial2.parseInt();
  //Serial.print(g);

  //Serial.print(",y:");
  y = Serial2.parseInt();
  //Serial.print(y);

  //Serial.print(",o:");
  o = Serial2.parseInt();
  //Serial.print(o);

  //Serial.print(",r:");
  r = Serial2.parseInt();
  //Serial.println(r);

  /*Serial.print("g/v:");
  Serial.print(g/v);
  
  Serial.print(",g/b:");
  Serial.print(g/b);
  
  Serial.print(",g/y:");
  Serial.print(g/y);
  
  Serial.print(",g/o:");
  Serial.print(g/o);
  
  Serial.print(",g/r:");
  Serial.println(g/r);*/
  
  float temp_color = g/r;
  float green_check = 15; //check against certain ratio (ex: green/red)

  //Serial.println(temp_color);

  /*Serial.print("Green/Red: ");
  Serial.print(g/r);
  Serial.print(" \t Green/Blue: ");
  Serial.print(g/b);*/

  if(temp_color > green_check)
  {
    Serial.println("Green");
    return(1);
    /*if(g/b > edge_check)
    {
      Serial.println("Edge");
      return(0);
    }
    else
    {
      Serial.print("Green: ");
      Serial.print(g);
      Serial.print("          green/blue: ");
      Serial.println(g/b);
      return(1);
    }*/
  }

  else
  {
    Serial.println("No");
    return 0;
  }
  
}


void greensq()
{
  if (rcolor == 1 || lcolor == 1)
  {
    //perhaps insert code to prevent over-turning???
    int green_val = (rcolor << 1) + lcolor;

    switch (green_val)
    {
      case 3:
        enc_turn(180);
        break;
      case 2:
        enc_turn(90);
        break;
      case 1:
        enc_turn(90);
        break;
      default:
        break;
    }
    
  }
}


void loop() 
{
  get_color();
}
