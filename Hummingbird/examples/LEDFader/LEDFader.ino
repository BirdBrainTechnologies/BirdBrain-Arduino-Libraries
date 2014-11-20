/*
LEDFader
Slowly fades the status LED and LED 1 on and off
Author: Tom Lauwers (tom@hummingbirdkit.com)
*/

// Make sure to include the Hummingbird library
#include <Hummingbird.h>

Hummingbird hummingbird; // Need this for all Hummingbird programs

void setup()
{
  hummingbird.init(); // Need this for all Hummingbird programs
}

void loop()
{
  for(int i = 0;i<256;i++) //loop brightness from 0 to 255
  {
    hummingbird.setStatusLED(i);
    hummingbird.setLED(1,i);
    delay(4);
  }
  for(int i = 255;i>=0;i--) //now go in reverse to fade back down
  {
    hummingbird.setStatusLED(i);
    hummingbird.setLED(1,i);
    delay(4);
  }
  delay(50); //So that the LED visibly stays off for a bit
}
