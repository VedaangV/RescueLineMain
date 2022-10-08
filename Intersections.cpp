//BASIC LOGIC FOR GREENSQ


int get_color()
{
  int temp_color; //temporary color readings
  int green_check; //check against certain ratio (ex: green/red)

  if(temp_color > green_check)
  {
    return(1);
  }

  else
  {
    return(0);
  }
  
}


void greensq()
{
  int rcolor = get_color();
  int lcolor = get_color();

  if (rcolor == 1 || lcolor == 1)
  {
    //perhaps insert code to prevent over-turning???
    int green_val = (rcolor << 1) + lcolor;

    /*switch (green_val)
    {
      case 3:
        enc_turn(180);
        break;

      case 2:
        enc_turn(90);
        break;

      case 3:
        enc_turn(90);
        break;

      default:
        break;
    }*/
    
  }
}
