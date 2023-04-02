//main rescue line code
#include "Header.h"
void setup() {
  //setup in here
  setup_qtr();
  Wire.begin();
  Serial.begin(115200);
  bnoSetup();
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  delay(100);

  Serial2.begin(115200);
  Serial2.println("ATINTTIME=1");
  Serial2.println("ATTCSMD=1");
  while (Serial2.available() == 0)
  {
    Serial.println("Serial 2 not available");
  }
  Serial.println("serial 2 available");
  get_ok2();

  Serial3.begin(115200);
  Serial3.println("ATINTTIME=1");
  Serial3.println("ATTCSMD=1");

  while (Serial3.available() == 0)
  {
    Serial.println("Serial 3 not available");
  }
  Serial.println("serial 3 available");
  get_ok3();
  delay(2000);

  //led pins
  pinMode(A9, OUTPUT); //blue
  pinMode(A10, OUTPUT); //green
  pinMode(A11, OUTPUT); //red
  
  Serial2.println("ATDATA"); //command to request data
  get_ok2();

  Serial3.println("ATDATA"); //command to request data
  get_ok3();

}
void loop() {
  lineTrace();
  greensq();
   
  Serial2.println("ATDATA"); //command to request data
  get_ok2();

  Serial3.println("ATDATA"); //command to request data
  get_ok3();
}
