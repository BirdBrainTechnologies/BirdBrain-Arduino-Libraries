/*
SlowMotionServo
Uses Hummingbird API to slowly move the servo
motor at Port 1 back and forth

HARDWARE REQUIREMENTS
Hummingbird Duo
1 Servo Motor

SOFTWARE REQUIREMENTS
Hummingbird library

HOW TO USE
Attach the servo motor to servo motor port 1 on the Hummingbird Duo

Verify and upload the code - once finished, the servo motor should start moving back
and forth.
If it doesn't work, make sure you've hooked up AC power for the servos!

Author: Justin Lee
*/

#include <Hummingbird.h>
Hummingbird hummingbird;

/*
Initialize Hummingbird ports
*/
void setup()
{
  hummingbird.init();
}

void loop()
{
  //slowly move servo forward total possible amount
  for(int i = 0;i<180;i++)
  {
    hummingbird.setServo(1,i);
    delay(15); //delay by 15 milliseconds (slows servo)
  }
  
  //slowly move servo backwards total possible amount
  for(int i = 180;i>0;i--)
  {
    hummingbird.setServo(1,i);
    delay(15); //delay by 15 milliseconds (slows servo)
  }
}
