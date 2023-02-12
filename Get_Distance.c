//required variables
//ultrasonic
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile int pulseFirst = 1;

//isr that is needed
// attachInterrupt(digitalPinToInterrupt(2), distanceISR,CHANGE);

void distanceISR()
{
 if(pulseFirst)
 {
  pulseMsSt= micros(); 
  pulseFirst = 0;
 }
 else
 {
  pulseMsEd = micros();
  pulseFirst = 1;
 }
}

float getFrontDistance()
{
  long pulseTime = pulseMsEd - pulseMsSt;
  float distance;
  distance = pulseTime * 0.034 / 2;
  if(distance < 0)
  {
    distance = 0;
  }
  return(distance);
}
