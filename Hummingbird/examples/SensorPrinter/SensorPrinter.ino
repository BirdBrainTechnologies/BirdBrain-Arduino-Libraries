/*
SensorPrinter
Prints sensor values to serial. 

Also converts sensor values to appropriate units for sound, distance, and temperature. 
The functions to convert sensor readings are in their own functions and easy to copy and paste to your own program.

We assume you connect the sensors as follows:
Port 1 - Light or Rotary
Port 2 - Sound
Port 3 - Distance
Port 4 - Temperature

Author: Tom Lauwers (tom@hummingbirdkit.com)
*/

// Include Hummingbird library
#include <Hummingbird.h>

Hummingbird hummingbird; // Create a Hummingbird object

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Hummingbird or Arduino Leonardo only
  }
  hummingbird.init();
}

// Just converts a sensor reading (0 to 1023) to the voltage on that port
double convertToVoltage(int raw_sensor)
{
  return raw_sensor * 5.0/1023.0;
}

// The sound sensor doesn't really convert to units (like dB), but the following scales the values to 0 to 1,000
// The ordinary range (in raw readings) is 75 or below indicate no sound, and the sensor maxes out at a raw reading
// of around 325.
int convertToSound(int raw_sensor)
{
  int sound = (raw_sensor - 75)*4; // readings below 75 are just sensor noise and indicate no sound. Readings go up to around 320
  if(sound < 0)
    sound = 0;  
  return sound;
}

/* The distance sensor is by far the trickiest. Our approach was to use a tape measure to measure the distance to our test object (a cardboard box)
 We found the following values:
Distance (cm)  Reading
5	       820
9	       720
11	       600
14	       500
17	       420
19	       380
23	       340
27	       300
33	       260
41	       220
56	       180
77	       140
Infinity       120

We subtracted 120 from these readings (as this was the baseline noise level). Then we tried curve-fitting various curves. 
The closest approximation to the readings was a fifth-order polynomial, which is:
y = -0.000000000004789x5 + 0.000000010057143x4 - 0.000008279033021x3 + 0.003416264518201x2 - 0.756893112198934x + 90.707167605683000
Where y is distance and x is the raw sensor reading.

This approach seems to yield reasonably accurate results, however there is some variation in detected distance (large, flat, bright, objects appear "closer"
than darkly colored or curved objects). Accuracy is around +/-10 cm for readings above 40 cm, +/- 5cm for 20-40 cm, and +/- 3cm for 5-20 cm.
*/
 
double convertToDistance(int raw_sensor)
{
  double distance;
  // If we're below 130, then the sensor probably isn't detecting anything. Set value to 100 cm, max range
  if(raw_sensor < 130)
    distance = 100.0;
  else {
    raw_sensor = raw_sensor - 120; // subtract the noise floor of 120
    // If, after subtraction we're above 680, the sensor is saturated, indicates minimum sense-able distance (5 cm)
    if(raw_sensor > 680)
      distance = 5.0;
    // If not, we do a fifth order polynomial calculation that fits the data pretty well  
    // This takes around 1 millisecond. The raw data is included in the comments to allow other methods to be used that are less computationally intensive
    else {
      double sensor_val_square = (double)raw_sensor*(double)raw_sensor; // Minimizing the number of times we calculate this
      distance = sensor_val_square*sensor_val_square*raw_sensor*-0.000000000004789
               + sensor_val_square*sensor_val_square*0.000000010057143
               - sensor_val_square*raw_sensor*0.000008279033021 
               + sensor_val_square*0.003416264518201 
               - raw_sensor*0.756893112198934 
               + 90.707167605683000;
    }
  }
  return distance;    
}

// The temperature calculation is fairy straight forward and derived from the datasheet. We use an NTC temperature probe, which has a roughly linear response 
// in the range 0-50 Celsius. Beyond this the equation will grow increasingly inaccurate, but you probably shouldn't stick our
// temperature probe in the oven anyway.
// Convert to Celsius. To convert this to Fahrenheit, take the Celsius value, multiply it by 1.8, and add 32. 
double convertToTemperature(int raw_sensor)
{
  return (raw_sensor - 512.0)/9.6+25.0;
}

// Reads and prints both raw and converted sensor values every 200 milliseconds (plus time to print and compute)
void loop()
{
  // Read from port 1 - voltage
  int sensor_val = hummingbird.readSensorValue(1);
  Serial.print("Raw Port 1 Value "); // Printing seems to work better if we avoid + concatenation of variables and strings
  Serial.print(sensor_val);
  Serial.print("\n"); // This adds a carriage return to the end of the line, like hitting "enter"
  Serial.print("Voltage at Port 1: ");
  Serial.print(convertToVoltage(sensor_val));
  Serial.print("\n");
  
  // Read from port 2 - sound
  sensor_val = hummingbird.readSensorValue(2);
  Serial.print("Raw Port 2 Value ");
  Serial.print(sensor_val);
  Serial.print("\n");
  Serial.print("Sound value: ");
  Serial.print(convertToSound(sensor_val));
  Serial.print("\n");
  
  // Read from port 3 - distance
  sensor_val = hummingbird.readSensorValue(3);  
  Serial.print("Raw Port 3 Value ");
  Serial.print(sensor_val);
  Serial.print("\n");
  Serial.print("Distance (CM): ");
  Serial.print(convertToDistance(sensor_val));
  Serial.print("\n");
  
  // Read from port 4 - temperature
  sensor_val = hummingbird.readSensorValue(4);  
  Serial.print("Raw Port 4 Value ");
  Serial.print(sensor_val);
  Serial.print("\n");
  
  Serial.print("Temperature (Celsius): ");
  Serial.print(convertToTemperature(sensor_val));
  Serial.print("\n");
 
  // Internal sensor can detect the voltage of the motor power port, useful
  // if you're running on batteries and need to detect low battery
  Serial.print("Motor port voltage: ");
  Serial.print(hummingbird.readInputVoltage());
  Serial.print("\n\n\n");    
  
  // Wait 200 millisecond  
  delay(200);    
}
