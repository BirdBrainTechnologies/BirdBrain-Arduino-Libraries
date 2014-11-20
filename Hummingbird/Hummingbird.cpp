

/* Hummingbird.cpp - Library for using the Hummingbird V2 as an Arduino while
   maintaining standard Hummingbird features like motor/servo control, LED fading,
   and sensor mappings.
   Created by Tom Lauwers, November 14, 2013
   Released under the terms of the MIT license*/

#include "Hummingbird.h"

volatile byte ledValuesBuffer[11];
volatile byte ledValues[11];
volatile byte nextLEDVal;

// Default constructor, does nothing as we use init to initialize everything
Hummingbird::Hummingbird()
{

}

// Calls init with LED fading and vibration motors enabled
void Hummingbird::init()
{
	init(true, true, true);
}

void Hummingbird::initOnlyMotorsAndServos()
{
	init(false, false, false);
}

void Hummingbird::init(boolean turnOnLEDFading, boolean turnOnVibrationSpeed, boolean turnOnLEDandVibrationOutputs)
{
  // Set all pins appropriately (LEDs, vibration motors)
  if(turnOnLEDandVibrationOutputs)
  {
	  pinMode(2, OUTPUT); // LED 2
	  pinMode(3, OUTPUT); // LED 1
	  pinMode(4, OUTPUT); // Tri-color green 1
	  pinMode(5, OUTPUT); // Tri-color red 2
	  pinMode(6, OUTPUT); // Tri-color green 2
	  pinMode(7, OUTPUT); // Tri-color red 1

	  pinMode(11, OUTPUT); // Tri-color blue 2
	  pinMode(12, OUTPUT); // Tri-color blue 1
	  pinMode(13, OUTPUT); // Status/'L' LED
	  // A couple of pins are not in the standard Arduino library, so we need to define them with standard C calls
	  DDRE |= _BV(PORTE2); // LED 3
	  DDRF |= _BV(PORTF7); // LED 4
  }
  // Set up SPI
  /* Set MOSI, SS, and SCK output */
  DDRB |= _BV(PORTB2)|_BV(PORTB1) | _BV(PORTB4);

  /* Enable SPI, Master, set clock rate fck/16, clock phase trailing */
  SPCR = _BV(SPE)| _BV(MSTR)| _BV(SPR0) | _BV(CPHA);
  PORTB |= _BV(PORTB4); // Start SS active high to prevent accidental communication

  delay(70); // Give the Tiny time to start up before you send any commands to it
  // Turn off motors and servos in case they were left on by previous program
  transmitByteToTiny(0xFF);
  transmitByteToTiny(0xFE);

  if(turnOnLEDFading)
	turnOnLEDFade();
  else
  	turnOffLEDFade();

  if(turnOnVibrationSpeed)
  	turnOnVibrationMotorSpeed();
  else
  	turnOffVibrationMotorSpeed();

}

// Get rid of Hummingbird object
Hummingbird::~Hummingbird()
{
  // Turn off motors and servos in case they were left on by previous program
  transmitByteToTiny(0xFF);
  transmitByteToTiny(0xFE);
}

// Note calling this function will override Arduino's standard timer 1 setup
// Essentially this means the Servo library will not operate correctly
void Hummingbird::turnOnVibrationMotorSpeed()
{
  // Configure counter such that we use fast
  // PWM with 8-bit resolution at a frequency of
  // 8000000/(1*255) = ~31 KHz

  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B = _BV(CS10) | _BV(WGM12);

  // Set ports to output
  pinMode(9, OUTPUT); // Vibration port 2
  pinMode(10, OUTPUT); // Vibration port 1

  // Reset PWM just in case it got carried through the bootloader
  OCR1A = 0;
  OCR1B = 0;

  // Tell setVibration function to use PWM to control speed of vibration motor
  vibrationSpeed = 1;
}

// Makes the vibration motors just turn on/off without speed control
// but frees up timer 1 for other stuff
void Hummingbird::turnOffVibrationMotorSpeed()
{
  // Tell setVibration function to use digitalWrite to control vibration motor
  vibrationSpeed = 0;

  // disconnect waveforms from pins
  TCCR1A &= ~(_BV(COM1A1) | _BV(COM1B1));
  // Set ports to output
  pinMode(9, OUTPUT); // Vibration port 2
  pinMode(10, OUTPUT); // Vibration port 1
  // Turn them off in case the bootloader left them on
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);

}

// This initializes fading on the Hummingbird LEDs
// Called by default, but can be turned off
// Uses Timer 3
void Hummingbird::turnOnLEDFade()
{
	for(int i = 0; i < 11; i++)
		ledValuesBuffer[i] = 0;

	nextLEDVal = 255;
	TCCR3A = 0x00; // Normal mode, no bells or whistles
	TCCR3B = _BV(WGM32) | _BV(CS32) | _BV(CS30); // Set the clock prescalar to 256, and turn on CTC mode and set top to OCR3A  _BV(WGM33) |

	// Initialize timer to 0, top to 256, turning this into an 8 bit counter (256 to allow one extra output compare B interrupt to trigger at 255)
	TCNT3 = 0;
	OCR3A = 256;

	// Turn on Timer 3 OVF and OCRA interrupts
	TIMSK3 = _BV(OCIE3A) | _BV(OCIE3B);

	LEDFading = 1;
}

// This turns off fading off the Hummingbird LEDs, releasing timer 3 for other uses
void Hummingbird::turnOffLEDFade()
{
	TCCR3B = 0x00; // stop the clock
	// Turn off Timer 3 OCRB and OCRA interrupts
	TIMSK3 = 0x00;
	LEDFading = 0;
}

void Hummingbird::setLED(byte port, byte intensity)
{
  if(LEDFading == 0) {
	  switch(port)
	  {
		  case 1:
			if(intensity != 0)
			  digitalWrite(3, HIGH);
			else
			  digitalWrite(3, LOW);
			break;
		  case 2:
			if(intensity != 0)
			  digitalWrite(2, HIGH);
			else
			  digitalWrite(2, LOW);
			break;
		  case 3:
			if(intensity != 0)
			  PORTE |= _BV(PORTE2);
			else
			  PORTE &= ~_BV(PORTE2);
			break;
		  case 4:
			if(intensity != 0)
			  PORTF |= _BV(PORTF7);
			else
			  PORTF &= ~_BV(PORTF7);
			break;
		  default:
			break;
	  }
	}
  else {
	  switch(port)
	  {
		  case 1:
			ledValuesBuffer[6] = intensity;
			break;
		  case 2:
			ledValuesBuffer[7] = intensity;
			break;
		  case 3:
			ledValuesBuffer[8] = intensity;
			break;
		  case 4:
			ledValuesBuffer[9] = intensity;
			break;
		  default:
			break;
	  }
  }
}

void Hummingbird::setStatusLED(byte intensity)
{
	if(LEDFading == 0) {
		if(intensity != 0)
		  digitalWrite(13, HIGH);
		else
		  digitalWrite(13, LOW);
	}
	else {
		ledValuesBuffer[10] = intensity;
	}
}

void Hummingbird::setTriColorLED(byte port, byte red, byte green, byte blue)
{
	if(LEDFading == 0) {
		if(port == 1) {
			if(red != 0)
				digitalWrite(7, HIGH);
			else
				digitalWrite(7, LOW);
			if(green != 0)
				digitalWrite(4, HIGH);
			else
				digitalWrite(4, LOW);
			if(blue != 0)
				digitalWrite(12, HIGH);
			else
				digitalWrite(12, LOW);
		}
		else {
			if(red != 0)
				digitalWrite(5, HIGH);
			else
				digitalWrite(5, LOW);
			if(green != 0)
				digitalWrite(6, HIGH);
			else
				digitalWrite(6, LOW);
			if(blue != 0)
				digitalWrite(11, HIGH);
			else
				digitalWrite(11, LOW);
		}
	}
	else {
		if(port == 1) {
			ledValuesBuffer[0] = red;
			ledValuesBuffer[1] = green;
			ledValuesBuffer[2] = blue;
		}
		else {
			ledValuesBuffer[3] = red;
			ledValuesBuffer[4] = green;
			ledValuesBuffer[5] = blue;
		}
	}
}


void Hummingbird::setVibration(byte port, byte intensity)
{
	if(vibrationSpeed == 0)
	{
	  if(port == 1)
	  {
		if(intensity != 0)
			digitalWrite(9, HIGH);
		else
			digitalWrite(9, LOW);
  	  }
	  else if(port == 2)
	  {
		if(intensity != 0)
			digitalWrite(10, HIGH);
		else
			digitalWrite(10, LOW);
  	  }
	}
	else {
	  if(port == 1)
		OCR1A = intensity;
	  else if(port == 2)
		OCR1B = intensity;
	}
}


void Hummingbird::setMotor(byte port, int velocity)
{
  int direction = 0;
  if(velocity < 0)
  {
    velocity = -velocity;
    direction = 1;
  }

  transmitByteToTiny(port+3 + direction*2);
  delayMicroseconds(50);
  transmitByteToTiny(velocity);
}

void Hummingbird::setServo(byte port, byte servoAngle)
{
  if(servoAngle > 180)
  	servoAngle = 180;

  transmitByteToTiny(port-1); // The tiny regards servo ports as number 0 to 3
  delayMicroseconds(50);
  servoAngle = ((unsigned int)(servoAngle))*225/180;
  transmitByteToTiny(servoAngle);
}


float Hummingbird::readInputVoltage()
{
  int power = analogRead(A5);
  return (power* (10.0 / 1023)); // The voltage at the port should be 1/2 the voltage of the input power source - will only work up to 10V
}


int Hummingbird::readSensorValue(byte port)
{
  int value = 0;
  switch(port)
  {
    case 1:
     value = analogRead(A1);
    break;
   case 2:
     value = analogRead(A2);
    break;
   case 3:
     value = analogRead(A3);
    break;
   case 4:
     value = analogRead(A4);
    break;
   case 5:
     value = analogRead(A5);
    break;
   default:
    break;
  }
 return value;

}

int Hummingbird::readDistance(byte port)
{
  return 0;

}

int Hummingbird::readTemperature(byte port)
{
  return 0;

}

void Hummingbird::transmitByteToTiny(byte cData)
{

	// Set SS pin low as Hummingbird doesn't talk on SPI to anything else
	PORTB &= ~_BV(PORTB4);
	delayMicroseconds(20);
	/// Start transmission
	SPDR = cData;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	delayMicroseconds(40);
	PORTB |= _BV(PORTB4);
}



// Overflow on timer 3, executes in roughly 35 us
// Basically transfers over the LED values, turns all LEDs on unless their value is set to 0
ISR(TIMER3_COMPA_vect)
{
	byte i;
	// Transfer set_orb values into this cycle's buffer
	for(i = 0; i < 11; i++) {
		ledValues[i] = ledValuesBuffer[i];
	}

	// Turn on all eleven elements unless their value is 0
	if(ledValues[0] != 0)
		PORTE |= _BV(REDLED1);
	else {
		PORTE &= (0xFF - _BV(REDLED1));
		ledValues[0] = 255;
	}

	if(ledValues[1] != 0)
		PORTD |= _BV(GREENLED1);
	else {
		PORTD &= (0xFF - _BV(GREENLED1));
		ledValues[1] = 255;
	}

	if(ledValues[2] != 0)
		PORTD |= _BV(BLUELED1);
	else {
		PORTD &= (0xFF - _BV(BLUELED1));
		ledValues[2] = 255;
	}
	if(ledValues[3] != 0)
		PORTC |= _BV(REDLED2);
	else {
		PORTC &= (0xFF - _BV(REDLED2));
		ledValues[3] = 255;
	}

	if(ledValues[4] != 0)
		PORTD |= _BV(GREENLED2);
	else {
		PORTD &= (0xFF - _BV(GREENLED2));
		ledValues[4] = 255;
	}

	if(ledValues[5] != 0)
		PORTB |= _BV(BLUELED2);
	else {
		PORTB &= (0xFF - _BV(BLUELED2));
		ledValues[5] = 255;
	}
	if(ledValues[6] != 0)
		PORTD |= _BV(LED1);
	else {
		PORTD &= (0xFF - _BV(LED1));
		ledValues[6] = 255;
	}

	if(ledValues[7] != 0)
		PORTD |= _BV(LED2);
	else {
		PORTD &= (0xFF - _BV(LED2));
		ledValues[7] = 255;
	}

	if(ledValues[8] != 0)
		PORTE |= _BV(LED3);
	else {
		PORTE &= (0xFF - _BV(LED3));
		ledValues[8] = 255;
	}

	if(ledValues[9] != 0)
		PORTF |= _BV(LED4);
	else {
		PORTF &= (0xFF - _BV(LED4));
		ledValues[9] = 255;
	}

	if(ledValues[10] != 0)
		PORTC |= _BV(ACTIVITY);
	else {
		PORTC &= (0xFF - _BV(ACTIVITY));
		ledValues[10] = 255;
	}

	nextLEDVal = 255;
	// Find the smallest one of the eleven and set the next output compare to occur then
	// Set nextLEDVal to the smallest of the eleven values
	for(i = 0; i < 11; i++) {
		if(ledValues[i] < nextLEDVal) {
			nextLEDVal = ledValues[i];
		}
	}
	OCR3B = nextLEDVal;
}

// Output compare B interrupt on timer 3, determine which LEDs to turn off, executes in 30 us or so
ISR(TIMER3_COMPB_vect)
{

	byte i;
	// If output compare is 255, nothing needs to be done, so exit

	if(nextLEDVal == 255) {
		return;
	}

	// If an LED value equals the next led value, toggle the pin to off
	if(ledValues[0] == nextLEDVal) {
		PORTE &= (0xFF - _BV(REDLED1));
		ledValues[0] = 255;
	}

	if(ledValues[1] == nextLEDVal) {
		PORTD &= (0xFF - _BV(GREENLED1));
		ledValues[1] = 255;
	}

	if(ledValues[2] == nextLEDVal) {
		PORTD &= (0xFF - _BV(BLUELED1));
		ledValues[2] = 255;
	}
	if(ledValues[3] == nextLEDVal) {
		PORTC &= (0xFF - _BV(REDLED2));
		ledValues[3] = 255;
	}

	if(ledValues[4] == nextLEDVal) {
		PORTD &= (0xFF - _BV(GREENLED2));
		ledValues[4] = 255;
	}

	if(ledValues[5] == nextLEDVal) {
		PORTB &= (0xFF - _BV(BLUELED2));
		ledValues[5] = 255;
	}
	if(ledValues[6] == nextLEDVal) {
		PORTD &= (0xFF - _BV(LED1));
		ledValues[6] = 255;
	}

	if(ledValues[7] == nextLEDVal) {
		PORTD &= (0xFF - _BV(LED2));
		ledValues[7] = 255;
	}
	if(ledValues[8] == nextLEDVal) {
		PORTE &= (0xFF - _BV(LED3));
		ledValues[8] = 255;
	}

	if(ledValues[9] == nextLEDVal) {
		PORTF &= (0xFF - _BV(LED4));
		ledValues[9] = 255;
	}
	if(ledValues[10] == nextLEDVal) {
		PORTC &= (0xFF - _BV(ACTIVITY));
		ledValues[10] = 255;
	}

	// Set the next led value to the smallest of the eleven
	nextLEDVal = 255;
	for(i = 0; i < 11; i++) {
		if(ledValues[i] < nextLEDVal) {
			nextLEDVal = ledValues[i];
		}
	}
	OCR3B = nextLEDVal;
}
