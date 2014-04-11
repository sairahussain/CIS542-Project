#include "mbed.h"
 //#include "Servo.h"
#include "MRF24J40.h"

#define MYID 3

/*Servo servoRIGHT(p22);   //Right side of servo connected to p22 pwmOut pin
Servo servoLEFT(p23);   //Left side of servo connected to p23  pwmOut pin*/
MRF24J40 mrf(p11,p12,p13,p14,p21);
/* MOSI P11 - CONNECT TO SDI (5)
   MISO P12 - CONNECT TO SDO (7)
   SCK P13  - CONNECT TO SCK (6)
   CS P14   - CONNECT TO #CS (8)
   RESET P21- CONNECT TO #RESET (2)
*/

Serial pc(USBTX, USBRX);
Timer timer1;
uint8_t rxBuffer[128];
uint8_t txBuffer[128] = {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00 };
uint8_t rxLen = 0;
bool flag = 0;

int main()
{
      pc.baud(115200);
      
      //uint8_t count = 0;
      //uint8_t IDrcvd= 0;
    
     while(1)
      {
        rxLen =  mrf.Receive(rxBuffer, 128);
        if (rxBuffer[8] == 'P' && rxBuffer[9] == MYID){
                flag = 1;
             //  timer1.start(); //start the timer
               pc.printf("\r\nPing received ! \r\n");  
        }
        else if (rxBuffer[8] == 'S' && rxBuffer[9] == MYID){
                flag = 0;
              //  timer1.stop(); //stop the timer
                pc.printf("\r\nFound me !-- stopped messages \r\n");
        }
        if (flag == 1) {
           
            wait(0.5);
            
                 pc.printf("timer set !\r\n ");
             //    timer1.reset(); // reset the timer
                 pc.printf("\r\nTXD\r\n");
                 txBuffer[8] = MYID;
                 mrf.Send(txBuffer, 9);
           
        } //end of if  flag
        
     }// end of while
}// end of main
