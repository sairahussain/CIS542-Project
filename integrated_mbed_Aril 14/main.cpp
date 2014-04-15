#include <deque> 
#include "mbed.h"
#include "MRF24J40.h"

#include "PriorityQueue.h"
#include "Dijkstra.h"


#define Node1 1
#define Node2 2
#define Node3 3
#define Node4 4
#define QueueSize 4
 
 PwmOut servoRIGHT(p23);
PwmOut servoLEFT(p24);
void forward();
void stop();
void right();
void left();
 
//Move the bot forward with a speed of 0.22 m/s
void forward() { 
     servoLEFT.write(0.08);
     servoRIGHT.write(0.07);
     
    }

//Stop the bot 
void stop() {
     servoLEFT.write(0.075);
     servoRIGHT.write(0.075);
    
}
void right() {
     servoLEFT.write(0.08);
     servoRIGHT.write(0.075);
     wait(1.5);
     forward();
}
void left() {
     servoLEFT.write(0.075);
     servoRIGHT.write(0.08);
     wait(1.5);
     forward();
}
 
 
 
 
 // RF tranceiver to link with handxheld.
 MRF24J40 mrf(p11, p12, p13, p14, p21);
 
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
     size_t currentNode;
     int8_t lastGreater = 0;
     std::deque<uint16_t> avgQueue;
     
     pc.baud(115200);
    
     Dijkstra d;
     d.setNodes();  
     
     deque<size_t> shortestPath = d.findShortest(1,5);
     currentNode = shortestPath.front();
     shortestPath.pop_front();
     
     timer.start();
     forward();
     while(1)
    {
         // Check if any data was received.
         rxLen = mrf.Receive(rxBuffer, 128);
        if(rxLen) 
        {
             uint8_t rssi = 0;
             // Send to serial.
             // IMPORTANT: The last two bytes of the received data
             // are the checksum used in the transmission. 
             rssi = mrf.ReadLong(0x302 + rxLen);
             
            if(rssi >= 170)
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
                            if(lastGreater == 1)
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
                               // currentNode++; // the node has been passed. Move to next node
                               if(shortestPath.size() >= 1)
                                  {
                                      size_t currNode = currentNode;
                                      currentNode = shortestPath.front();
                                      shortestPath.pop_front();
                                      size_t neighbor = currentNode;
                                      printf("direction to go for next node: %c",d.nodeGraph[currNode-1][neighbor-1].direction);
                                  }
                               else
                                  break;
                                
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
    /*Dijkstra d;
    d.setNodes();  
     
    deque<size_t> shortestPath = d.findShortest(1,5);
    pc.printf("\n------\n");
    size_t currNode;
    size_t neighbor;
    
    for(int i=0;i<shortestPath.size()-1;i++)
    {
        currNode = shortestPath.at(i);
        neighbor = shortestPath.at(i+1);
        pc.printf("Node %d\r\n",currNode);
        pc.printf("Next Node %d\r\n",neighbor);
        pc.printf("Direction: %c\r\n ",d.nodeGraph[currNode-1][neighbor-1].direction); 
    } */   
 }
 

