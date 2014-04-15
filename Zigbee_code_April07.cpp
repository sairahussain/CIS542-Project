#include "mbed.h"
#include "MRF24J40.h"
#include "NodeId.h"
#include <deque> 

#define QueueSize 4
 // RF tranceiver to link with handxheld.
 MRF24J40 mrf(p11, p12, p13, p14, p21);
 
 // LEDs
 DigitalOut led1(LED1);
 DigitalOut led2(LED2);
 DigitalOut led3(LED3);
 DigitalOut led4(LED4);
 
 // Timer.
 Timer timer;
 
 // Serial port for showing RX data.
 Serial pc(USBTX, USBRX); 
 
 // Send / receive buffers.
 // IMPORTANT: The MRF24J40 is intended as zigbee tranceiver; it tends
 // to reject data that doesn't have the right header. So the first 
 // 8 bytes in txBuffer look like a valid header. The remaining 120
 // bytes can be used for anything you like.

 uint8_t rxBuffer[128];
 uint8_t rxLen; 
 uint8_t nodeID;
 bool nextNode = true;
 uint8_t pingCount = 0;
 
 uint8_t txBuffer[128] = {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00 };
 
 
 int main (void)
 {
     uint8_t count = 0;
     uint16_t rssi_sum = 0;
     uint16_t rssi_avg = 0;
     size_t currentNode = 1;
     int8_t lastGreater = 0;
     std::deque<uint16_t> avgQueue;
     
     pc.baud(115200);
     timer.start();
     while(1)
    {
         // Check if any data was received.
         rxLen = mrf.Receive(rxBuffer, 128);
        if(rxLen) 
        {
             // Toggle LED 1 upon each reception of data.
             led1 = led1^1;
             uint8_t rssi = 0;
             // Send to serial.
             // IMPORTANT: The last two bytes of the received data
             // are the checksum used in the transmission. 
             rssi = mrf.ReadLong(0x302 + rxLen);
            
             
            if(rssi >= 145)
            {
                pc.printf("\nRSSI: %d\n", rssi);
                pc.printf("RX\r\n");
        
                nodeID = rxBuffer[8];
                pc.printf("Current Node being tracked: %d Queue Size: %d\r\n", currentNode, avgQueue.size());
                //pc.printf("******1*********\r\n"); 
                if(nodeID == currentNode)
                {
                   // pc.printf("******2*********\r\n");
                    pc.printf("\r\nP, NodeID = %d\r\n", nodeID);
                    nextNode = false;
                    count++;
                    rssi_sum = rssi_sum + rssi;
                    
                    if(count == 4)
                    {
                    
                        //pc.printf("******3*********\r\n");
                        rssi_avg = rssi_sum / 4;
                        pc.printf("AVG RSSI: %d\r\n", rssi_avg);
                        if(avgQueue.size() <= QueueSize)
                        {
                            avgQueue.push_back(rssi_avg);
                           // pc.printf("Queue Size: %d\r\n", sizeQ);
                        }
                        if(avgQueue.size() > QueueSize) 
                            avgQueue.pop_front();
                        count = 0;
                        rssi_sum = 0;
                        rssi_avg = 0;
                        //pc.printf("******3.1*********\r\n");
                      
                       // pc.printf("2 Queue Size: %d\r\n", sizeQ);
                        
                        int sizeQ =  avgQueue.size();
                        pc.printf("avgQueue: ");
                        
                        for(int i = 0; i < sizeQ; i++)
                            pc.printf(" %d", avgQueue.at(i));
                        pc.printf("\r\n");
                        
                        if(sizeQ == QueueSize && (avgQueue.at(sizeQ - 1) < avgQueue.at(sizeQ-2)))
                        {
                            //pc.printf("******4*********\r\n");
                            if(lastGreater == 3)
                            {
                                //pc.printf("******5*********\r\n");
                                pc.printf("TXD\r\n");
                                // Send counter value.
                                txBuffer[8] = 'S';
                                txBuffer[9] = currentNode;
                                //txBuffer[10] = '!';
                                
                                mrf.Send(txBuffer, 10);
                                  
                                for(int i = 0; i<10; i++) {
                                  pc.printf("= %d \t " , txBuffer[i]);
                                }
                                currentNode++; // the node has been passed. Move to next node
                                lastGreater = 0;
                                nextNode = true;
                                pingCount = 0;
                            }
                            else 
                                lastGreater++;
                        }
                        else
                            lastGreater = 0;
                            
                         pc.printf("avgQueue: ");
                        for(int i = 0; i < sizeQ; i++)
                            pc.printf(" %d", avgQueue.at(i));
                        pc.printf("\r\n");
                        } 
                }    
            }                  
            pc.printf("\r\n");                

        }
        
        if(timer.read_ms() >= 1000) 
        {
            if(nextNode)
            {
                pingCount++;
                timer.reset();
                // Toggle LED 2.
                led2 = led2^1;
                  
                // UART.
                pc.printf("Ping node %d\r\n", currentNode);
                // Send counter value.
                txBuffer[8] = 'P';
                txBuffer[9] = currentNode;
                //txBuffer[10] = '!';
                
                mrf.Send(txBuffer, 10);
                  
                for(int i = 0; i<10; i++) {
                  pc.printf(" %d \t " , txBuffer[i]);
                }
                
                if(pingCount == 50)
                {
                    pc.printf("\n\rNo response from node %d being tracked \n\r", currentNode);
                    nextNode = false;
                }
            }
        }
    
    }
        
 }
 
