#include <Hummingbird.h>

Hummingbird hummingbird;

int motorSpeed;
byte serialRead[5];

// This is not very good - seems like blocking until read misses bytes from time to time, so probably should change the code
// Allowed me to test everything for now though

void setup()
{
//  hummingbird.setLED(2,50);  
//  hummingbird.setLED(3,100);
//  hummingbird.setLED(4,255);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  hummingbird.init();
  
 // TCCR1B = 0x0C;
 // sei();
 // hummingbird.turnOnLEDFade();
}

byte blockingSerialRead()
{
   while(Serial.available()==0);
   return Serial.read(); 
}

void loop()
{
  serialRead[0] = blockingSerialRead();
  switch(serialRead[0]) {
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
      break;
    case 'S':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      hummingbird.setServo(serialRead[1], serialRead[2]);
      break;
    case 'L':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      if(serialRead[1] == 0)
        hummingbird.setStatusLED(serialRead[2]);
      else
        hummingbird.setLED(serialRead[1], serialRead[2]);
      break;
    case 'V':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      hummingbird.setVibration(serialRead[1], serialRead[2]);
      break;  
    case 'M':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      serialRead[3] = blockingSerialRead()-'0';
      motorSpeed = serialRead[3];
      if(serialRead[2] == 0)
        motorSpeed = -1 * serialRead[3]; // Set direction
      hummingbird.setMotor(serialRead[1], motorSpeed);
      break;  
    case 'O':
      serialRead[1] = blockingSerialRead()-'0';
      serialRead[2] = blockingSerialRead()-'0';
      serialRead[3] = blockingSerialRead()-'0';  
      serialRead[4] = blockingSerialRead()-'0';
      hummingbird.setTriColorLED(serialRead[1], serialRead[2], serialRead[3], serialRead[4]);
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
      break;
    default:
      Serial.print("\nWhat you say?\n");
      break;
  }
}

