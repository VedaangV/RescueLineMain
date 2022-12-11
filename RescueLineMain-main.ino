//main rescue line code
#include "Header.h"
void setup() {
  //setup in here
  Serial.begin(9600);
  Wire.begin();
  //ultrasonic isr
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
   attachInterrupt(digitalPinToInterrupt(2), distanceISR,CHANGE);

}
void loop() {
  //main loop in here
  
  
}
