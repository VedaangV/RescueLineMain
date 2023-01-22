
int messageByte = 11111111;
int checksumByte = 0;
int dataByte = 0;
bool started = 0;
unsigned int red = 0;
unsigned int green = 0;
unsigned int blue = 0;
unsigned int bright = 0;
int getbyte()
{
  while(Serial3.available() <= 0)
  {
   
  }
  return Serial3.read();
}
void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
  Serial3.begin(2400);
  
  
  Serial.println("Starting");
  while(started == 0)
  {
    dataByte = messageByte;
    messageByte = getbyte();
    if(messageByte == 0b00000100 && dataByte == 0b11101101)
    {
      started = 1;
    }
    Serial.print(messageByte);
    Serial.print(" ");
    Serial.println(dataByte);
    
  }
  Serial3.write(0b00000100);
  Serial3.end();
  Serial3.begin(57600);
  Serial.println("Setup_Complete");
  
  while(Serial3.available() < 2)
  {
    delay(10);
  }
  messageByte = getbyte();
  getbyte();
  getbyte();
  delay(1);
  Serial3.write(0b01000011);
  Serial3.write(0b00000100);//change mode 4
  Serial3.write(0b10111000);//10111000
  while((messageByte = getbyte()) != 0b11011100)
  {
    delay(0.5);
  }
  red = getbyte();
      red += (getbyte() <<8);
      green = getbyte();
      green += (getbyte() <<8);
      blue = getbyte();
      blue += (getbyte() <<8);
      bright = getbyte();
      bright += (getbyte() <<8);
      checksumByte = getbyte();
      Serial.print(red);
      Serial.print("  ");
      Serial.print(green);
      Serial.print("  ");
      Serial.println(blue);
      Serial3.write(0b00000010);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*lightif(Serial3.available() > 2)
  {
      messageByte = getbyte();
      dataByte = getbyte();
      checksumByte = getbyte();
      Serial.println(dataByte);
      Serial3.write(0b00000010);
  }*/
  if(Serial3.available() > 3)
  {
      //Serial.println(getbyte());
      messageByte = getbyte();
      red = getbyte();
      red += (getbyte() <<8);
      green = getbyte();
      green += (getbyte() <<8);
      blue = getbyte();
      blue += (getbyte() <<8);
      bright = getbyte();
      bright += (getbyte() <<8);
      checksumByte = getbyte();
      Serial.print(red);
      Serial.print("  ");
      Serial.print(green);
      Serial.print("  ");
      Serial.println(blue);
      Serial3.write(0b00000010);
      //delay(10);
  }
  
}
