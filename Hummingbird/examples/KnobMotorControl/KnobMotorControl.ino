/*
KnobMotorControl
Control the motor speed with the knob

HARDWARE REQUIREMENTS
Hummingbird Duo
Gear motor
Knob sensor (in a pinch, a light sensor could work here)

SOFTWARE REQUIREMENTS
Hummingbird library

HOW TO USE
Attach a gear motor to motor port 1 on the Hummingbird Duo
Attached a rotary sensor to sensor port 1

The rotary sensor controls the speed and direction of the motor. 

If it doesn't work, make sure you've hooked up AC power for the motor!

Author: Tom Lauwers (tom@hummingbirdkit.com)
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
  int knob_sensor = hummingbird.readSensorValue(1); // Reads a value from 0 to 1023
  // Motor takes speeds from -255 to 255, so we need convert our value to this range
  int motor_speed = knob_sensor/2 - 255;
  // edge case if the sensor is 1022 or 1023
  if(motor_speed == 256)
   motor_speed = 255; 
  hummingbird.setMotor(1, motor_speed);
}
