//main rescue line code
#include "Header.h"
int x = 0;
const char* serialReq = "ATDATA";
void setup() {
  //setup in here
  motorSetup();
  setup_qtr();
  Wire.begin();
  Serial.begin(115200);
  bnoSetup();
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  delay(100);
  init_color(left);
  init_color(right);
  //led pins
  pinMode(redPin, OUTPUT); pinMode(greenPin, OUTPUT); pinMode(bluePin, OUTPUT);
  //obstacle pins
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  // rescue kit stuff
  evacSetup();

}
void loop() {
  Serial2.println(serialReq);
  Serial3.println(serialReq);
  greensq();
  if( x % 3 == 0){
     obstacle();

  }
  lineTrace();
  //qtr_print();
  x++;
  
 
  
  
  
}
