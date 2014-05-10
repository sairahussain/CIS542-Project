#include "mbed.h"
#include <cstdlib>
#include "MRF24J40.h"
#include "boebot.h"
#include "Dijkstra.h"
#include <deque> 
const int QueueSize = 4; //Size of the Queue that stores the average values of RSSI

/* Parametrized constructor of ZigBee whose parameters are the pins connections of mbed which will be connected suitably to the ZigBee */
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

uint8_t txBuffer[128] = {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00 }; //Transmission Buffer


int main (void)
{
  uint8_t rxLen;                     //stores receive buffer length
  uint8_t nodeID;                    //The node ID's received from different ZigBee modules
  bool nextNode = true;              //Flag which is set to indicate that ping messages have to be sent to the currentNode being tracked
  uint8_t count = 0;                  //Counts the number of rssi values summed up for calculating rssi_avg
  uint16_t rssi_sum = 0;
  uint16_t rssi_avg = 0;             //average value of RSSI received from the ZigBee module
  size_t currentNode;                //currentNode being tracked
  int8_t lastGreater = 0;            //Flag to check if the peak for RSSI values is detected or not
  std::deque<uint16_t> avgQueue;     //Deque for storing the rssi_avg values
  unsigned int Source = 1;
  unsigned int Dest = 5;
  pc.baud(115200);                   //baud rate for the serial communication = 115200
  Navigate s;                        //object for Navigation of Boebot
  Graph g;                           //object for Graph consisting map of all nodes
  g.setNodes();                      //initializing the map of nodes
  Dijkstra d;                        //object for Dijkstra class
  std::deque<unsigned> shortestPath = d.findShortest(Source,Dest,g);
  if (shortestPath.empty())
    {
      pc.printf(" No Path found from Source Node: %d to Destination Node: %d\r\n", Source, Dest);
      return 0;
    }
  currentNode = shortestPath.front();    //front of the shortest path queue returns the 1st node to be tracked in the path
  shortestPath.pop_front();              //remove the current node being tracked from the queue
  
  timer.start();                         //start the timer for sending out ping signals by the boebot to the nodes in the network
  s.forward();                           //Boebot is given forward motion command
  while(1)
    {
      /*Receiving RSSI values from the nodes in the network */
      // Check if any data was received.
      rxLen = mrf.Receive(rxBuffer, 128);
      if(rxLen) 
        {
	  uint8_t rssi = 0;
	  // Send to serial.
	  // IMPORTANT: The last two bytes of the received data
	  // are the checksum used in the transmission. 
	  rssi = mrf.ReadLong(0x302 + rxLen);   //read the rssi value from the memory location specified by the address
          
	  if(rssi >= 170)    //setting threshold of rssi as 170 to filter out signals sent by nodes placed farther away
            {
	      pc.printf("\nRSSI: %d\n", rssi);
	      pc.printf("RX\r\n");
	      nodeID = rxBuffer[8];   //storing the nodeID received from the zigbee in nodeID
	      pc.printf("Current Node being tracked: %d Queue Size: %d\r\n", currentNode, avgQueue.size());
	      if(nodeID == currentNode)   //checking if the nodeID received is equal to the currentNode being tracked
                {
		  pc.printf("\r\nP, NodeID = %d\r\n", nodeID);
		  nextNode = false;        //setting the nextNode as false
		  count++;                 //increasing count for taking average
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
		      /*reset values of rssi_sum and rssi_avg*/
		      count = 0;
		      rssi_sum = 0;
		      rssi_avg = 0;
                      
		      int sizeQ =  avgQueue.size();
		      pc.printf("avgQueue: ");
                      
		      for(int i = 0; i < sizeQ; i++)
			pc.printf(" %d", avgQueue.at(i));
		      pc.printf("\r\n");
		      
		      /*  If the last element in the queue is less than the second last element, it indicates
		       *  RSSI value has indeed decreased since the node being tracked has been reached or has been crossed.
		       *  This trend of decrease in the last value compared to second last value in the queue, is checked twice, by using lastGreater variable
		       *  so that it is confirmed that the node being tracked has indeed been crossed, and the next node in the path is then tracked
		       */
		      
		      if(sizeQ == QueueSize && (avgQueue.at(sizeQ - 1) < avgQueue.at(sizeQ-2)))
                        {
			  if(lastGreater == 1)
                            {
			      pc.printf("TXD\r\n");
			      
			      txBuffer[8] = 'S';              // Flag to indicate the current node to stop sending the pings
			      txBuffer[9] = currentNode;
			      mrf.Send(txBuffer, 10);         // Sending the stop ping message to the current node
                              
			      //Reseting the values
			      lastGreater = 0;
			      nextNode = true;
			      pingCount = 0;
                            }
			  else
			    //incrementing value of LastGreater if the peak of rssi value is detected
			    lastGreater++;
                        }
		      else
			// if the peak is not detected consecutively, reset the lastGreater counter to 0 and track again
			lastGreater = 0;
		      
		    } 
                }    
            }                  
	  pc.printf("\r\n");                
	  
        }
      
      /* After every 1000 ms ping message is sent by boebot to the Zigbee nodes */
      if(timer.read_ms() >= 1000) 
        {
	  if(nextNode)
            {
	      pingCount++;            //counts the number of pings sent
	      timer.reset();          //reset the timer after every 1000 ms so that it starts reading again from 0 to 1000
              
	      // UART.
	      pc.printf("Ping node %d\r\n", currentNode);
	      
	      txBuffer[8] = 'P';
	      txBuffer[9] = currentNode;
	      
              
	      mrf.Send(txBuffer, 10);
              
	      for(int i = 0; i<10; i++) {
		pc.printf(" %d \t " , txBuffer[i]);
	      }
	      
	      /* if pingCount reaches 50, and the current node has not responded, it assumes the node dead and sets nextNode to false */
	      if(pingCount == 50)
                {
		  pc.printf("\n\rNo response from node %d being tracked \n\r", currentNode);
		  nextNode = false;
		  pingCount = 0;
                }
            }
        }
      
    }
  
}


