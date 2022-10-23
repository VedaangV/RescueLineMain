
int incomingByte = 0;
int messageByte = 11111111;
int checksumByte = 0;
int sensorType = 0;
int infoByte = 0;
int dataByte = 0;
int speedByte = 0b00000000;
int startup = 0;
unsigned int red = 0;
unsigned int green = 0;
unsigned int blue = 0;
unsigned int bright = 0;

int getbyte()
{
  while(Serial1.available() <= 0)
  {
    delay(1);
  }
  return Serial1.read();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(2400);

  
  uint8_t payloadLength = 0;
  Serial.println("Starting");
  //delay(2000);

  pinMode(13, OUTPUT);
  digitalWrite(12,LOW);
  delay(500);
  digitalWrite(12,HIGH);
  
  while(startup == 0)
  {
    if(Serial1.available() > 0)
    {
      Serial.print("Byte read: ");
      incomingByte = getbyte();
      messageByte = incomingByte;
      Serial.println(messageByte,BIN);
      //if command message
      if((messageByte>>6) == 0b01)
      {
        Serial.println("command");
        switch(messageByte)
        {
          //sensor type
          case 0b01000000:
            sensorType = getbyte();
            checksumByte = getbyte();
            break;
           
          case 0b01001001:
            getbyte();
            getbyte();
            checksumByte = getbyte();
            break;
          case 0b01010010:
            for(int i = 0; i < 4; i++)
            {
              speedByte<<8;
              speedByte = speedByte|getbyte();
              //Serial.println(speedByte,BIN);
            } 
            checksumByte = getbyte();
            break; 
        }
        //check checksum
      }
      //if name info message
      else if((messageByte>>6) == 0b10)
      {
        Serial.println("info");
        infoByte = getbyte();
        switch(infoByte)
        {
          //name
          case 0b00000000:
            payloadLength = pow(2,((messageByte&0b00111000)>>3));
            for(int i = 0; i <= payloadLength; i++)
            {
              getbyte();
            }
            checksumByte = getbyte();
            break;
          //raw pct
          case 0b00000001:
            for(int i = 0; i < 8; i++)
            {
              getbyte();
            }
            checksumByte = getbyte();
            break;
          case 0b00000011:
            for(int i = 0; i < 8; i++)
            {
              getbyte();
            }
            checksumByte = getbyte();
            break;
          //format
          case 0b10000000:
            for(int i = 0; i < 4; i++)
            {
              getbyte();  
            }
            checksumByte = getbyte();
            break;
            //si units
          case 0b00000100:
            payloadLength = pow(2,((messageByte&0b00111000)>>3));
            for(int i = 0; i <= payloadLength; i++)
            {
              getbyte();
            }
            checksumByte = getbyte();
            break;
      }
    }
    else if((messageByte>>6) == 0b00)
    {
      Serial.println("ACK");
      startup = 1;
      Serial1.write(0b00000100);
    }
    Serial.println(checksumByte,BIN);
    }
  }
  Serial1.end();
  Serial1.begin(57600);
  Serial.println("Setup_Complete- changing to rgb raw"); //mode 4
  while(Serial1.available() < 2)
  {
    delay(10);
  }
  messageByte = getbyte();
  getbyte();
  getbyte();
  delay(1);
  Serial1.write(0b01000011);
  Serial1.write(0b00000100);//change mode 4
  Serial1.write(0b10111000);//10111000
  while(Serial1.available())
  {
    Serial1.read();
  }
  delay(1);
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available() > 3)
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
      Serial1.write(0b00000010);
      //delay(10);
  }
}
