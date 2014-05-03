#include "boebot.h"
#include "mbed.h"
PwmOut servoRIGHT(p23);
PwmOut servoLEFT(p24);
Navigate :: Navigate(){}

Navigate :: ~Navigate()
{
}
void Navigate :: forward() {
     servoLEFT.write(0.076);
     servoRIGHT.write(0.074);

    }

void Navigate :: stop() {
     servoLEFT.write(0.075);
     servoRIGHT.write(0.075);

}

void Navigate :: right() {
     servoLEFT.write(0.076);
     servoRIGHT.write(0.075);
    
}
void Navigate :: left() {
     servoLEFT.write(0.075); 
     servoRIGHT.write(0.074); //move right wheel clockwise.
    
} 

