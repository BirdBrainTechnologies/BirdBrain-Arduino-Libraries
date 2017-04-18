/*
SensorTrigger
Uses a sensor on port 1 (Light sensor suggested) to
turn the vibration motor and LED on and off

HARDWARE REQUIREMENTS
1 Vibration Motor
1 LED
1 Light sensor (or other sensor if you prefer)

SOFTWARE REQUIREMENTS
Hummingbird library

HOW TO USE
Attach an LED to LED port 1
Attach a vibration motor to vibration motor port 1
Attach a chosen sensor to sensor port 1

Verify and upload the code to your Hummingbird Duo. When finished, the status light should be on.

Trigger your sensor; if you chose the light sensor you can just cover the sensor
with your hand. The vibration motor and the LED you attached should turn on, while
the status LED should turn off.

Author: Justin Lee
*/

#include <Hummingbird.h>
Hummingbird hummingbird;

void setup()
{
  hummingbird.init();
}

void loop()
{
  int x = hummingbird.readSensorValue(1); //Read from sensor 1
  if(x>100)
  {
    hummingbird.setStatusLED(255); //turn on Status LED
    hummingbird.setVibration(1,0); //turn off vibration motor
    hummingbird.setLED(1,0); //turn off LED on port 1
  }
  else
  {
    hummingbird.setStatusLED(0); //turn off Status LED
    hummingbird.setVibration(1,255); //turn on vibration motor
    hummingbird.setLED(1,255); //turn on LED on port 1
  }
  delay(15); //wait 15 milliseconds
}
