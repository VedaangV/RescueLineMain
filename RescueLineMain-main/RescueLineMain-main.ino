//main rescue line code
#include "Header.h"
int x = 0;
const char* serialReq="ATDATA";
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
  get_ok(right);

  Serial3.begin(115200);
  Serial3.println("ATINTTIME=1");
  Serial3.println("ATTCSMD=1");

  while (Serial3.available() == 0)
  {
    Serial.println("Serial 3 not available");
  }
  Serial.println("serial 3 available");
  get_ok(left);
  

  //led pins
  pinMode(redPin, OUTPUT); pinMode(greenPin, OUTPUT); pinMode(bluePin, OUTPUT);
  //obstacle pins
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
 
}
void loop() {
//  Serial2.println(serialReq);
//  Serial3.println(serialReq);
  lineTrace();
  greensq();
}
