/*
SimpleServo
Uses Hummingbird API to move all four servos

HARDWARE REQUIREMENTS
Hummingbird Duo
1 to 4 Servos

SOFTWARE REQUIREMENTS
Hummingbird library

HOW TO USE
Attach servos to any or all servo ports on the Hummingbird Duo

Verify and upload the code - once finished, the servo motor should start moving back
and forth.
If it doesn't work, make sure you've hooked up AC power for the servos!

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
  // Moves the servos in a cascading sequence
    hummingbird.setServo(4,180);
    hummingbird.setServo(1,0);
    delay(500); //delay by 200 ms

    hummingbird.setServo(1,180);
    hummingbird.setServo(2,0);
    delay(500); //delay by 200 ms

    hummingbird.setServo(2,180);
    hummingbird.setServo(3,0);
    delay(500); //delay by 200 ms

    hummingbird.setServo(3,180);
    hummingbird.setServo(4,0);
    delay(500); //delay by 200 ms
}
