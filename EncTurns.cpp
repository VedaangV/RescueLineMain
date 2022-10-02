void setupBNO() 
{//set up for BNO-055
  if (!bno.begin(0x08)) // turn off the magnetometer
  {
    Serial.println("No BNO055 found");
    while (1);
  }
  //bno.setExtCrystalUse(true);
}

void getBNO() 
{
  //tca select channel
  rot = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
}


int get_rot() 
{
  getBNO();
  return rot.x();
}

void enc_turn(int deg) 
{
  const int exSpeed = 90;
  getBNO();

  // calculate difference from the intended rotation
  int difference = rot.x() - deg;
  if (difference < -180) difference += 360;
  if (difference > 180) difference -= 360;
  if (abs(difference) <= 1) return;

  // figure out the speed
  int speed = map(difference, -180, 180, exSpeed, -exSpeed) + (difference > 0 ? -minSpeed : minSpeed);
  //int speed = (difference > 0 ? -exSpeed : exSpeed);
  //Serial.println(difference);

  go_motor(speed);
  enc_turn(deg);
}
