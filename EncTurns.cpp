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
  Serial.print("Degree: ");
  Serial.println(deg);
  
  Serial.print("target: ");
  Serial.println(target);
  int calc_speed = ((deg * -1) * speed) / abs(deg);
  Serial.print("\n\n\n\n\n\n\n\n\n\n\nRIGHT MOTOR: ");
  Serial.println(calc_speed);
  Serial.print("LEFT MOTOR: ");
  Serial.println(-calc_speed);

  while (yaw > target + 2 || yaw < target - 2)
    {
      Serial.println(yaw);
      rightMotorRun(calc_speed);
      leftMotorRun(-calc_speed);
      yaw = getYaw();
    }
    
  motorsStop();
}
