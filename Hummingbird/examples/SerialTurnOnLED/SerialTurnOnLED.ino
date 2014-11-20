/*
SerialTurnOnLED
Takes Serial Input to turn the Status LED on and off
Additional Helper Methods:
byte blockingSerialRead() - Reads from serial only when something
 has been inputted
*/

#include <Hummingbird.h>
Hummingbird hummingbird;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  hummingbird.init(); //initialize Hummingbird ports
}

// Reads from serial input when information is entered
byte blockingSerialRead()
{
   while(Serial.available()==0); // wait for Serial to be available
   return Serial.read(); //Return user input
}

void loop()
{  
  Serial.print("Enter 'y' to turn on the LED and 'n' to turn it off.");
  byte x = blockingSerialRead();
  switch(x){
    case 'n': //if "n" is entered
      hummingbird.setStatusLED(0); //turn off LED
      Serial.print("LED TURNED OFF\n");
      break;
    case 'y': //if "y" is entered
      hummingbird.setStatusLED(100); //turn on LED
      Serial.print("LED TURNED ON\n");
      break;   
    default: // if invalid input is entered
      Serial.print("try again\n");
      break;
    }
}
