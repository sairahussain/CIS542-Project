#include "mbed.h"
#include "MRF24J40.h"
#include "boebot.h"

#include "Dijkstra.h"

#include <deque> 

const int QueueSize = 4;
 
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

 uint8_t rxBuffer[128];  //receive Buffer
 uint8_t rxLen;          //stores receive buffer length 
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
     Navigate s;
     Graph g;
    g.setNodes();
     Dijkstra d(1,5);
     std::deque<unsigned> shortestPath = d.findShortest(g);
     currentNode = shortestPath.front();
     shortestPath.pop_front();
     
     timer.start();
     s.forward();
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
             rssi = mrf.ReadLong(0x302 + rxLen);   //read the rssi value from the memory location specified by the address
             
            if(rssi >= 170)    //setting threshold of rssi as 170
            {
                pc.printf("\nRSSI: %d\n", rssi);
                pc.printf("RX\r\n");
                nodeID = rxBuffer[8];   //storing the nodeID received from the zigbee in nodeID
                pc.printf("Current Node being tracked: %d Queue Size: %d\r\n", currentNode, avgQueue.size());
                if(nodeID == currentNode)   //checking if the nodeID received is equal to the currentNode being tracked
                {
                    pc.printf("\r\nP, NodeID = %d\r\n", nodeID);
                    nextNode = false;        //setting the nextNode as false
                    count++;                 //increasing count
                    rssi_sum = rssi_sum + rssi;  //calculating sum of rssi
                    /*adding four values in rssi and finding the average of rssi*/
                    if(count == 4)
                    {
                          rssi_avg = rssi_sum / 4; 
                        pc.printf("AVG RSSI: %d\r\n", rssi_avg);
                        /*recording 4 values of rssi_avg*/
                        if(avgQueue.size() <= QueueSize)  
                        {
                            avgQueue.push_back(rssi_avg);
                        }
                        /*if the queue has reached 4 values, then remove the first element from front of queue */
                        if(avgQueue.size() > QueueSize) 
                            avgQueue.pop_front();
                        count = 0;
                        rssi_sum = 0;
                        rssi_avg = 0;
                       
                        int sizeQ =  avgQueue.size();
                        pc.printf("avgQueue: ");
                        
                        for(int i = 0; i < sizeQ; i++)
                            pc.printf(" %d", avgQueue.at(i));
                        pc.printf("\r\n");
                        
                        if(sizeQ == QueueSize && (avgQueue.at(sizeQ - 1) < avgQueue.at(sizeQ-2)))
                        {
                            if(lastGreater == 1)
                            {
                                pc.printf("TXD\r\n");
                                // Send counter value.
                                txBuffer[8] = 'S';
                                txBuffer[9] = currentNode;
                                mrf.Send(txBuffer, 10);
                                  
                                for(int i = 0; i<10; i++) {
                                  pc.printf("= %d \t " , txBuffer[i]);
                                }
                               
                              /* if(shortestPath.size() >= 1)
                                  {
                                      size_t currNode = currentNode;
                                      currentNode = shortestPath.front();
                                      shortestPath.pop_front();
                                      size_t neighbor = currentNode;
                                      printf("direction to go for next node: %d",d.nodeGraph[currNode-1][neighbor-1].direction);
                                  }
                               else
                                  break;
                               */ 
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
    
 }
 

