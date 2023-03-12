  //main rescue line code
#include "Header.h"
void setup() {
  //setup in here
  setup_qtr();
  Wire.begin();
  Serial.begin(115200); Serial2.begin(115200); Serial3.begin(115200);
  Serial2.println("ATINTTIME=1"); get_ok2();
  Serial3.println("ATINTTIME=1"); get_ok3();
  bnoSetup();
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  //led pins
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
}
void loop() {
 greensq();
 obstacle();
 linetrace();
}
