/* 
BlinkLED
Simply blinks an LED
Author: Tom Lauwers (tom@hummingbirdkit.com)
*/

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
  // Turns on LED on port 1 and status LED to full brightness
  tweety.setLED(1,255);
  tweety.setStatusLED(255);
  delay(500); // wait 500 milliseconds
  // Turns them off
  tweety.setLED(1,0);
  tweety.setStatusLED(0);
  delay(500); // wait 500 milliseconds 
}
  
