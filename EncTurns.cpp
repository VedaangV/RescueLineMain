void enc_turn(int deg, int speed)
{
  int target = 0;
  motorsStop();
  yaw = getYaw();

  //right turn
  if (deg > 0)
  {
    target = yaw + deg;
    if (target > 360)
    {
      target = target - 360;
    }

    while (yaw > target + 1 || yaw < target - 1)
    {
      rightMotorRun(-speed);
      leftMotorRun(speed);
      yaw = getYaw();
      Serial.print("Yaw: ");
      Serial.println(yaw);
    }
    motorsStop();
  }

  //left turn
  if (deg < 0)
  {
    if (yaw < abs(deg))
    {
      target = yaw + (360 - abs(deg));
    }

    else
    {
      target = yaw - abs(deg);
    }
    while (yaw > target + 2 || yaw < target - 2)
    {
      rightMotorRun(speed);
      leftMotorRun(-speed);
      yaw = getYaw();
    }

    while (yaw > target)
    {
      lturn(speed);
      yaw = getYaw();
    }
    motorsStop();
  }

}
