//main rescue line code
#include "Header.h"
void setup() {
  //setup in here
  Serial.begin(9600);
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);


}
void loop() {
  //main loop in here
  
  
}
