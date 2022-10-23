void enc_turn(int deg, int speed)
{
  int target = 0;
  motorsStop();
  yaw = getYaw();
  target = yaw + deg;

  if (target < 0)
  {
    target+= 360;
  }

  else if (target > 360)
  {
    target -= 360;
  }

  int calc_speed = ((deg * -1) * speed) / abs(deg);

  while (yaw > target + 2 || yaw < target - 2)
    {
      rightMotorRun(calc_speed);
      leftMotorRun(-calc_speed);
      yaw = getYaw();
    }
    
  motorsStop();
}
