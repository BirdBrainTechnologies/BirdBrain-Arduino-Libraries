/*
LEDThermometer
Sets the Color of the tri-color LED based on temperature.

Connect a Tri-color LED to port 1
Connect a temperature sensor to port 4

We suggest putting the LED in a styrofoam ball or other diffusive material so the colors
mix well.

Expansion idea: Add a servo with a popsicle stick and create a semi-circular dial marked
with temperatures from 10-40 C. Have the servo go to the right angle to point at the correct
temperature.

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
  // Get the temperature in celsius, convert it to an int
  int temperature = (int)convertToTemperature(hummingbird.readSensorValue(4));  

  // If the temperature is below 10 C, it's cold! LED should be blue
  if(temperature < 10)
  {
    hummingbird.setTriColorLED(1,0,0,255);
  }
  // If we're between 10 and 25, LED should be greenish-blue - more green at higher temps
  else if(temperature < 25)
  {
    temperature = (temperature -10)*17; // Make the range of temperature 0 to 255   
    // Green goes up with temperature, blue goes down
    hummingbird.setTriColorLED(1,0, temperature, 255-temperature); 
  }
  // If we're between 25 and 40, LED should be redish-green, more red at higher temps
  else if(temperature < 40)
  {
    temperature = (temperature - 25)*17; // Make range of temperature 0 to 255
    // Red goes up with temperature, green goes down
    hummingbird.setTriColorLED(1, temperature, 255-temperature, 0);  
  }
  // If we're above 40, it's hot! Make the LED red
  else
  {
    hummingbird.setTriColorLED(1, 255,0,0);
  }
}

// The temperature calculation is fairy straight forward and derived from the datasheet. We use an NTC temperature probe, which has a roughly linear response 
// in the range 0-50 Celsius. Beyond this the equation will grow increasingly inaccurate, but you probably shouldn't stick our
// temperature probe in the oven anyway.
// Convert to Celsius. To convert this to Fahrenheit, take the Celsius value, multiply it by 1.8, and add 32. 
double convertToTemperature(int raw_sensor)
{
  return (raw_sensor - 512.0)/9.6+25.0;
}
