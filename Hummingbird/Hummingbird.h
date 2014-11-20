/* Hummingbird.h - Library for using the Hummingbird V2 as an Arduino while
   maintaining standard Hummingbird features like motor/servo control, LED fading,
   and sensor mappings. */

#ifndef Hummingbird_h
#define Hummingbird_h

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// Defining pins used by the ORB and User LEDs
#define LED1  PORTD0
#define LED2  PORTD1
#define LED3  PORTE2
#define LED4  PORTF7
#define REDLED1   PORTE6
#define GREENLED1 PORTD4
#define BLUELED1  PORTD6
#define REDLED2   PORTC6
#define GREENLED2 PORTD7
#define BLUELED2  PORTB7
#define ACTIVITY  PORTC7

class Hummingbird
{
  public:
  	Hummingbird();
  	void init();
  	void initOnlyMotorsAndServos();
    void init(boolean turnOnLEDFading, boolean turnOnVibrationSpeed, boolean turnOnLEDandVibrationOutputs);
    ~Hummingbird();
    void turnOnVibrationMotorSpeed();
    void turnOffVibrationMotorSpeed();
    void turnOnLEDFade();
    void turnOffLEDFade();
    void setLED(byte port, byte intensity);
    void setStatusLED(byte intensity);
    void setTriColorLED(byte port, byte red, byte green, byte blue);
    void setVibration(byte port, byte intensity);
    void setMotor(byte port, int velocity);
    void setServo(byte port, byte degrees);
    float readInputVoltage();
    int readSensorValue(byte port);
    int readDistance(byte port);
    int readTemperature(byte port);
  private:
    void transmitByteToTiny(byte cData);
    byte LEDFading;
    byte vibrationSpeed;
   // byte ledValuesBuffer[11];
   // byte ledValues[11];
   // byte nextLEDVal;
};

#endif
