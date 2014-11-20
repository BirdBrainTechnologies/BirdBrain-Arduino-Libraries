/* Simple fade LED example program. This is meant as a 
starting point for folks to write their own Hummingbird
Arduino programs. */

// Includes Hummingbird library
#include <Hummingbird.h>

// Makes a Hummingbird object, tweety
Hummingbird tweety;

void setup()
{
  // If you want to use Hummingbird API functions like setLED,
  // you must initialize the Hummingbird
  tweety.init(); 
}

void loop()
{
// Fades them on
  for(int i = 0; i < 255; i++)
  {
    tweety.setLED(1,i);
    tweety.setStatusLED(i);
    delay(2);  
  }
  // Fades them off
  for(int i = 255; i >=0; i--)
  {
    tweety.setLED(1,i);
    tweety.setStatusLED(i);
    delay(2);  
  } 
}
  
