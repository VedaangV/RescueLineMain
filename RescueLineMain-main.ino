//main rescue line code
#include "Header.h"
void setup() {
  //setup in here
  setup_qtr();
  Wire.begin();
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  bnoSetup();
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  delay(100);

}
void loop() {
  //main loop in here
  switch (getCase()) { //getCase() defined in FUNCTIONS.CPP

    case 1:
      //evac
      break;

    case 2:
      motorsStop();
      greensq();
      break;

    case 3:
      obstacle();
      break;

    default:
      lineTrace();
      break;


  }



}
