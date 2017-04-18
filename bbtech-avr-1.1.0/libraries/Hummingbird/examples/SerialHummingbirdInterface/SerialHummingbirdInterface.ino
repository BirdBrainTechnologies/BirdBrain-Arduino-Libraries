/*
SerialHummingbirdInterface
Allows all Hummingbird ports to be controlled over a virtual COM port.

Author: Tom Lauwers (tom@hummingbirdkit.com)
*/

// Include Hummingbird library
#include <Hummingbird.h>

Hummingbird hummingbird; // Create a Hummingbird object

int motorSpeed; // Need a variable to store motor speed into
byte serialRead[5]; // Buffer for receiving serial data

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Hummingbird or Arduino Leonardo only
  }
  hummingbird.init();
}

// Function to make serial block until a character shows up
byte blockingSerialRead()
{
   while(Serial.available()==0);
   return Serial.read(); 
}

void loop()
{
  // Block to read the first byte
  serialRead[0] = blockingSerialRead();
  
  switch(serialRead[0]) {
    // Print all sensor values. Input voltage prints the voltage at the motor power port (<1V when no power is plugged in there)
    case 's':
      Serial.print(hummingbird.readSensorValue(1));
      Serial.print("\n");
      Serial.print(hummingbird.readSensorValue(2));
      Serial.print("\n");
      Serial.print(hummingbird.readSensorValue(3));
      Serial.print("\n");
      Serial.print(hummingbird.readSensorValue(4));
      Serial.print("\n");
      Serial.print(hummingbird.readInputVoltage());
      Serial.print("\n");      
      break;
    // Sets a servo - first argument is port (ASCII 1-4), second is angle  
    case 'S':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead();
      if(serialRead[1] >= 1 && serialRead[1] <=4)
        hummingbird.setServo(serialRead[1], serialRead[2]);
      else
        Serial.print("Error\n");
      break;
    // Sets the single color LED - port (ASCII 1-4), then brightness (0-255)
    case 'L':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead();
      if(serialRead[1] == 0)
        hummingbird.setStatusLED(serialRead[2]);
      else if(serialRead[1] < 5)
        hummingbird.setLED(serialRead[1], serialRead[2]);
      else
        Serial.print("Error\n");  
      break;
      // Sets the vibration motors - port (ASCII 1-2), then intensity (0-255)
    case 'V':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead();
      if(serialRead[1] == 1 || serialRead[1] == 2)
        hummingbird.setVibration(serialRead[1], serialRead[2]);
      else
        Serial.print("Error\n");        
      break;  
     // Sets motor speed and direction - port (ASCII 1-2), direction (ASCII 0 is reverse, all others is forward), speed (0-255) 
    case 'M':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      serialRead[3] = blockingSerialRead();
      motorSpeed = serialRead[3];
      if(serialRead[1] == 1 || serialRead[1] == 2)
      {
        if(serialRead[2] == 0)
          motorSpeed = -1 * serialRead[3]; // Set direction
        hummingbird.setMotor(serialRead[1], motorSpeed);
      }
      else
        Serial.print("Error\n");           
      break;  
    // Sets tri-color LED color and brightness - port (ASCII 1-2), the r,g,b brightness (0-255)  
    case 'O':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead();
      serialRead[3] = blockingSerialRead();  
      serialRead[4] = blockingSerialRead();
      if(serialRead[1] == 1 || serialRead[1] == 2)
        hummingbird.setTriColorLED(serialRead[1], serialRead[2], serialRead[3], serialRead[4]);
      else
        Serial.print("Error\n");           
      break; 
   // Lets you configure if timers are used to enable vibration motor speed control and LED brightness   
    case 'C':
      serialRead[1] = blockingSerialRead();
      if(serialRead[1] == 'L')
        hummingbird.turnOnLEDFade();
      else if(serialRead[1] == 'V')
        hummingbird.turnOnVibrationMotorSpeed();
      else if(serialRead[1] == 'l')
        hummingbird.turnOffLEDFade();
      else if(serialRead[1] == 'v')
        hummingbird.turnOffVibrationMotorSpeed();
      else
        Serial.print("Error\n");           
      break;
    default:
      Serial.print("Tweet?\n");
      break;
  }
}

