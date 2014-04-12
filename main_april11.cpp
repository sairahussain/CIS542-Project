#include "mbed.h"
#define INF 9000
#define NODES 8
#include <stdlib.h>
using namespace std;

#include "types.h"
#include "heap.h"


Serial pc(USBTX, USBRX);

std::deque<size_t> Dijkstra(size_t sourceNode,size_t destNode);

int main() {
    // Initially all paths are infinity
    pc.baud(115200);     
   
    setNodes();
     
    std::deque<size_t> shortestPath = Dijkstra(1,5);
    int i,size;
    size = shortestPath.size();
    for(i =size-1; i >=0; --i)
    {
            pc.printf("\n\rNode %d ", shortestPath.at(i));
    }
}

// If the returned queue is empty, it means there is no direct path between source and destination
std::deque<size_t> Dijkstra(size_t sourceNode, size_t destNode)
{
    //requires forall size_t sourceNode, destNode:: 1 <= sourceNode <= Nodes && 1 <= destNode <= Nodes
    //ensures forall size_t sourceNode, destNode:: 1 <= shortestPath.size() <= Nodes
    //ensures forall sizet sourceNode, destNode:: shortestPath.at(0).NodeId == sourceNode && shortestPath.at(shortestPath.size()-1).NodeId == destNode
        
    routeMap routes; 
    // For Initializing RouteMap
   
    for(size_t i=1;i<NODES;i++)
    {        
        std::deque<size_t> defaultQueue;
        std::pair <size_t,size_t> routePair;
        routePair = std::make_pair (i,i);   //1,2,3,.... to 1,2,3.....
        routes.insert(routePair);
    }
  
    
    // Assigning every node's tentative distance to infinity and 0 for source node
    Heap currentStatus(sourceNode);
    
    // For Source Node shortest path is node itself
    //routes[sourceNode-1].push_back(sourceNode);
    
    size_t currentNode,currNeighbor,newCost,oldCost,temp,queueSize;    
    currentNode = sourceNode;
    
    
    map<size_t,Plan>::iterator it;
    while(currentStatus.indexMap[destNode-1] != -1)
    {
        
        for(it = nodeGraph[currentNode-1].begin(); it!= nodeGraph[currentNode-1].end();it++)
        {  
            //invariant forall size_t n: 1 <= n <= i && currentStatus[n-1] != -2: 
            //                          (currentStatus[currentNode-1])+ p[currentNode-1][n-1].cost <= /old(currentStatus[n-1]) ? currentStatus[n-1] ==       
           currNeighbor = it->first+1;
           
            
            // If the current neighbor is already visited, skip
            if(currentStatus.indexMap[currNeighbor-1] != -1)
            {
                newCost = nodeGraph[currentNode-1][currNeighbor-1].cost + currentStatus.at(currentNode).cost;
                oldCost = currentStatus.at(currNeighbor).cost;
                
               if( newCost < oldCost)
                {
                    
                    currentStatus.decreaseCost(currNeighbor,newCost);
                    
                    //currentStatus[i-1] = p[currentNode-1][i-1].cost + currentStatus[currentNode-1];
                    
                    
                    // Copying Elements of routes[CurrentNode-1] to routes[currNeighbor-1] - it will overwrite
                  /*  queueSize = routes[currentNode-1].size();
                    for(size_t j=1; j <= queueSize;j++)
                    {
                        //invariant routes[currentNode-1].size() == queueSize
                        //invariant routes[i-1].size() == j-1
                        //invariant forall size_t a: 0 <= a < j <= queueSize: routes[currentNodes-1].at(queueSize - a - 1) == routes[i-1].at(j-a-1)
                        
                        temp = routes[currentNode-1].at(0);
                        routes[currentNode-1].pop_front();
                        routes[currNeighbor-1].push_back(temp);
                        routes[currentNode-1].push_back(temp);             
                    }*/
                    routes[currNeighbor]= currentNode;
                    
                }//end if
            }
        } // for ends here
        
        currentStatus.extractMin();
        currentStatus.indexMap[currentNode-1] = -1;
        currentNode = currentStatus.node[1].nodeID;
        
    }//while ends here
    
    size_t myNode;
    myNode = destNode;
    deque<size_t> myPath;
  //  for (map<size_t,size_t>::iterator it=routes.begin(); it!=routes.end(); it++)
  //      pc.printf("\n\rnode->  %d       predecessor->  %d\r\n", it->first, it->second );
     
    while (myNode!=sourceNode) {
        
        myPath.push_back(myNode);
        myNode = routes[myNode];
        
    }
    myPath.push_back(myNode);
    
    return myPath;
}
void setNodes()
{
    Plan p;
    
    map<size_t,Plan> temp;
    
    /* For Node 1 */
    // Neighbour is node 3
    p.cost = 1;
    p.direction = 'S';
    temp.insert(make_pair(3-1,p));
    
    nodeGraph.push_back(temp);
    
    /* For Node 2 */
    temp.clear();
    p.cost = 3;
    p.direction = 'E';
    temp.insert(make_pair(3-1,p));
    
    nodeGraph.push_back(temp);
    
    /* For Node 3 */
    temp.clear();
    p.cost = 1;
    p.direction = 'N';
    temp.insert(make_pair(1-1,p));
    
    p.cost = 3;
    p.direction = 'W';
    temp.insert(make_pair(2-1,p));
    
    p.cost = 5;
    p.direction = 'E';
    temp.insert(make_pair(4-1,p));
    
    p.cost = 2;
    p.direction = 'S';
    temp.insert(make_pair(7-1,p));
    
    nodeGraph.push_back(temp);
    
    /* For Node 4 */
    temp.clear();
    p.cost = 5;
    p.direction = 'W';
    temp.insert(make_pair(3-1,p));
    
    temp.clear();
    p.cost = 7;
    p.direction = 'S';
    temp.insert(make_pair(5-1,p));
    
    nodeGraph.push_back(temp);
    
    /* For Node 5 */
    temp.clear();
    p.cost = 7;
    p.direction = 'N';
    temp.insert(make_pair(4-1,p));
    
    p.cost = 2;
    p.direction = 'S';
    temp.insert(make_pair(6-1,p));
    
    nodeGraph.push_back(temp);
    
    /* For Node 6 */
    temp.clear();
    p.cost = 2;
    p.direction = 'N';
    temp.insert(make_pair(5-1,p));

    p.cost = 3;
    p.direction = 'W';
    temp.insert(make_pair(7-1,p));
    nodeGraph.push_back(temp);
    
    /* For Node 7 */
    temp.clear();
    p.cost = 2;
    p.direction = 'N';
    temp.insert(make_pair(3-1,p));
    
    p.cost = 3;
    p.direction = 'E';
    temp.insert(make_pair(6-1,p));
    nodeGraph.push_back(temp);
    
}




Heap::Heap()
{
}

Heap::Heap(size_t sourceNodeID)
{
    // Node at first position is reserved for implementation of binary heap
    node[0].nodeID = 0;
    node[0].cost = 0;
    heapSize = NODES;
    // All nodes except sourceNode would have cost of infinity with source node having value of 0
    for(size_t i=1;i<=NODES;i++)
    {
        node[i].nodeID = i;
        indexMap[i-1] = i;
        if(sourceNodeID == i)
            node[i].cost = 0;
        else
            node[i].cost = INF;
    }
    // Heapify all elements
    for(size_t j=heapSize/2;j>=1;j--)
        heapify(j);
}

void Heap::extractMin()
{
    // increase the starting index
    swapNodes(1,heapSize);
    heapSize--;
    
    for(int j=heapSize/2;j>=1;j--)
        heapify(j);
}

bool Heap::decreaseCost(size_t nodeID,size_t newCost)
{
    if(indexMap[nodeID-1] != -1)
    {
        node[indexMap[nodeID-1]].cost = newCost;
        return true;
    }
    return false;
}

void Heap::heapify(size_t parent)
{
   
    
    size_t smallest,left,right;
    
    left = 2*parent;
    right = 2*parent + 1;
    if(left <= heapSize && node[left].cost < node[parent].cost)
        smallest = left;
    else
        smallest = parent;
   
    if(right <= heapSize && node[right].cost < node[smallest].cost)
        smallest = right;
    if(smallest != parent)
    {
        swapNodes(parent,smallest);
        heapify(smallest);
    }
    
  
}


void Heap::swapNodes(size_t leftIndex,size_t rightIndex)
{
    heapNode temp = node[leftIndex];
    node[leftIndex] = node[rightIndex];
    node[rightIndex] = temp;
    
    indexMap[node[leftIndex].nodeID-1] = leftIndex;
    indexMap[node[rightIndex].nodeID-1] = rightIndex;
}


heapNode Heap::at(size_t nodeID)
{
    return (node[indexMap[nodeID-1]]);
}
