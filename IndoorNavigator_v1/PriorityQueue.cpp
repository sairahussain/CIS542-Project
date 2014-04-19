#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()
{
}

PriorityQueue::PriorityQueue(size_t sourceNodeID)
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

void PriorityQueue::extractMin()
{
    // increase the starting index
    swapNodes(1,heapSize);
    heapSize--;
    
    heapify(1);
}

bool PriorityQueue::decreaseCost(size_t nodeID,size_t newCost)
{
    if(indexMap[nodeID-1] != -1)
    {
        node[indexMap[nodeID-1]].cost = newCost;
        return true;
    }
     
    size_t thisNode = indexMap[nodeID-1];
    size_t parentNode = thisNode/2;
    
    
    //percolate the node with the decreased value up
    while(node[thisNode].cost < node[parentNode].cost && thisNode > 1)
    {
       swapNodes(thisNode, parentNode);
       thisNode = parentNode;
       parentNode = thisNode/2;
        
    }
    
    
    return false;
}

void PriorityQueue::heapify(size_t parent)
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


void PriorityQueue::swapNodes(size_t leftIndex,size_t rightIndex)
{
    heapNode temp = node[leftIndex];
    node[leftIndex] = node[rightIndex];
    node[rightIndex] = temp;
    
    indexMap[node[leftIndex].nodeID-1] = leftIndex;
    indexMap[node[rightIndex].nodeID-1] = rightIndex;
}


heapNode PriorityQueue::at(size_t nodeID)
{
    return (node[indexMap[nodeID-1]]);
}